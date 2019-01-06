/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*					                    WIN32 PORT & LINUX PORT
*                          (c) Copyright 2004, Werner.Zimmermann@fht-esslingen.de
*                 (Similar to Example 1 of the 80x86 Real Mode port by Jean J. Labrosse)
*                                           All Rights Reserved
** *****************************************************************************************************
*		Further modified by 	mikael.jakas@puv.fi & 
* 					Jukka.matila@vamk.fi
* *****************************************************************************************************
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        4       /* Number of identical tasks                          */

#define HORIZONTAL 3
#define VERTICAL 3

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

typedef struct TaskCounter {
    INT8U count1;
    INT8U count2;
} TaskCounter;

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
OS_EVENT      *RandomSem;
OS_EVENT      *RanSem;
char          TaskData[N_TASKS];                      /* Parameters to pass to each task               */
char          board[HORIZONTAL][VERTICAL];
INT8U         err;
TaskCounter   count;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void *data);        /* Function prototype of Startup task           */
void DrawBoard(char board[HORIZONTAL][VERTICAL]);
void Player1(void *data);
void Player2(void *data);
void GameStatus(INT8U player, INT8U value);

INT8U PositionCalculation(char board[3][3], INT8U selection, char input);
INT8U GameLogic(char board[HORIZONTAL][VERTICAL]);
void GameStatus(INT8U player, INT8U value);


/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int  main(void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    RandomSem = OSSemCreate(1);
    RanSem = OSSemCreate(2);

    count.count1 = 0;
    count.count2 = 0;

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void TaskStart(void *pdata)
{
    INT16S key;
    INT8U symbol;


    pdata = pdata;                                         /* Prevent compiler warning                 */

    //OSTaskCreate(TicTacToe, (void *)&symbol, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(DrawBoard, (void *)&symbol, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Player1, (void *)&symbol, (void *)&TaskStk[1][TASK_STK_SIZE - 1], 2);
    OSTaskCreate(Player2, (void *)&symbol, (void *)&TaskStk[2][TASK_STK_SIZE - 1], 3);
    OSTaskCreate(GameStatus, (void *)&symbol, (void *)&TaskStk[3][TASK_STK_SIZE - 1], 4);

    for (;;) {

        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                exit(0);  	                           /* End program                              */
            }
        }

        OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
    }
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

void DrawBoard(char board[HORIZONTAL][VERTICAL])
{
    INT8U i;
    INT8U j;
    
    INT8U index;
    index = '1';

    #ifdef __WIN32__
        srand(GetCurrentThreadId());
    #endif
    #ifdef __LINUX__
        srand(getppid());
    #endif


    //Initialize board
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            board[i][j] = index;
            index++;
        }
    }

    //Draw board
    for ( i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            printf("%c", board[i][j]);
            if (j != 2)
                printf("|");
        }
        if (i != 2)
            printf("\n---------");
        printf("\n");
    }
    OSTimeDlyHMSM(0, 0, 1, 0);
}

void Player1(void *data){
    int counter = 0;
    int selection = 0;
    int checker = 0;
    char board[HORIZONTAL][VERTICAL];

    while (counter != 9){
    if (counter % 2 == 0)
    {
        printf("\nPlease input where you want to place X: ");
        scanf("%d", &selection);

    SCANX:
        while (selection < 0 || selection > 10)
        {
            printf("Invalid input, please try again: ");
            scanf("%d", &selection);
        }

        printf("\n");
        checker = PositionCalculation(board, selection, 'x');

        if (checker == -1)
        {
            selection = -1;
            goto SCANX;
        }

        DrawBoard(board);
        counter++;

        if (counter > 3)
        {
            if (GameLogic(board) == 1)
               GameStatus(1, 4);
        }
        
    }
}
}

void Player2(void *data){
    int counter = 0;
    int selection = 0;
    int checker = 0;
    char board[HORIZONTAL][VERTICAL];

    while(counter != 9){
        printf("\nPlease input where you want to place O: ");
        scanf("%d", &selection);

    SCANO:
        while (selection < 0 || selection > 10)
        {
            printf("Invalid input, please try again: ");
            scanf("%d", &selection);
        }
        printf("\n");

        checker = PositionCalculation(board, selection, 'o');
        if (checker == -1)
        {
            selection = -1;
            goto SCANO;
        }

        DrawBoard(board);
        counter++;

        if (counter > 3)
        {
            if (GameLogic(board) == 1)
                GameStatus(2,4);
        }
    }
}

void GameStatus(INT8U player, INT8U value){
    OSSemPend(RandomSem, 0, &err);
    if(player==1 && value > 3){
        printf("X WINS");
    }
    if(player==2 && value > 3){
        printf("O WINS");
    }
    else{
        printf("Draw");
    }
    OSSemPost(RandomSem);
}



/*function code*/

/*$PAGE*/
/*
*********************************************************************************************************
*                                      NON-TASK FUNCTIONS
*********************************************************************************************************
*/



//Determine X and O position
INT8U PositionCalculation(char board[3][3], INT8U selection, char input)
{
    INT8U column = selection % 3;
    INT8U row = selection / 3;

    if (column == 0)
    {
        column += 3;
        row -= 1;
    }

    if (board[row][column - 1] == 'o' || board[row][column - 1] == 'x')
        return -1;

    board[row][column - 1] = input;
    return 0;
}

//Win condition
INT8U GameLogic(char board[HORIZONTAL][VERTICAL])
{
    INT8U i;
    for (i = 0; i < HORIZONTAL; i++)
    {
        //Horizontal Check
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return 1;

        //VerticalCheck
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return 1;

        //Diagonal Check
        if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) || (board[0][2] == board[1][1] && board[1][1] == board[2][0]))
            return 1;
    }
}