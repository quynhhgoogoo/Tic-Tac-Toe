/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*					WIN32 PORT & LINUX PORT
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
#define  N_TASKS                        2       /* Number of identical tasks                          */

#define HORIZONTAL 3
#define VERTICAL 3

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
char          TaskData[N_TASKS];                      /* Parameters to pass to each task               */

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void *data);        /* Function prototype of Startup task           */
void DrawBoard(char board[HORIZONTAL][VERTICAL]);
void TicTacToe();


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

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                                             /* Start multitasking                       */
    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart(void *pdata)
{
    INT16S key;
    INT8U symbol;


    pdata = pdata;                                         /* Prevent compiler warning                 */
    symbol = 'X';

    OSTaskCreate(TicTacToe, (void *)&symbol, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 1);
    //OSTaskCreate(DrawBoard, (void *)&symbol, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 2);

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


/*function code*/

/*$PAGE*/
/*
*********************************************************************************************************
*                                      NON-TASK FUNCTIONS
*********************************************************************************************************
*/

void TicTacToe(){
    INT8U selection;
    INT8U counter;
    INT8U checker;

    char board[HORIZONTAL][VERTICAL];
    DrawBoard(board);
}