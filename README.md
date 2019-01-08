# Course Final Work : Tic-Tac-Toe
Create a simple Tic-Tac-Tow - game.
Game board 3x3 or greater (Use Global memory)
In program create four tasks:

    player 1 (X)
    Player 2 (O)
    Draw board
    Analyze game status (if either have won) (Optional)

Game board is created as shared memory 
Game logic can be made with:

    Semaphores (Might require many different)
    MBoxes - Use MBox like token
    Event Flags
        OSFlagCreate()
        OSFlagPend()
        OSFlagPost()
