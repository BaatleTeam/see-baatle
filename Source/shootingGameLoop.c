#include "shootingGameLoop.h"

GameResults shootingGameLoop(ShipsInfo *ShipsPlayer, ShipsInfo *ShipsComputer, Board *BoardPlayer, Board *BoardComputer) {
    WindowParametres WBackGround;
    WindowParametres WInfoPlayer;
    WindowParametres WInfoComputer;
    WindowParametres WBoardPlayer;
    WindowParametres WBoardComputer;
    // WindowParametres WHelp;
    initWindowsShooting(ShipsPlayer, BoardPlayer, &WBackGround, &WInfoPlayer, &WInfoComputer, &WBoardPlayer, &WBoardComputer);


    wbkgdset(WBackGround.ptrWin, COLOR_PAIR(200));
    wclear(WBackGround.ptrWin);
    // mvprintw(0, 0, "Lines: %d, cols: %d", LINES, COLS);
    wrefresh(WBackGround.ptrWin);

    PlayerStats statisticsPlayer;
    initPlayerStats(&statisticsPlayer, ShipsPlayer);
    PlayerStats statisticsComputer;
    initPlayerStats(&statisticsComputer, ShipsComputer);

    // поле для стрельбы человека по компьютеру                                      
    ShotBoard shotBoardPlayer;
    initShotBoard(&shotBoardPlayer, BoardPlayer);
    // поле для стрельбы компьютера по человеку
    ShotBoard shotBoardComputer;
    initShotBoard(&shotBoardComputer, BoardComputer);

    Coordinate cursorPostion = (Coordinate){0, 0};
    Coordinate computerShot = (Coordinate){-1, -1};

    DrawWInfo_Shoting(&WInfoPlayer, &statisticsPlayer);
    DrawWInfo_Shoting(&WInfoComputer, &statisticsComputer);
    DrawWBoard_Shoting_Default(WBoardPlayer);
    updateGraphics_Shoting(WBoardComputer, shotBoardPlayer, cursorPostion);

    GameResults results = {.playerStatus = UNKONOWN};

    int key;
    bool isAnyBodyWins = FALSE;

    while((key = getch()) != KEY_F(5) && isAnyBodyWins != TRUE) {
        switch(key){
            // case 9:
                // drawEndGameScreen(WBackGround.ptrWin, 9, 61, PLAYER_WINS);
            //     break;
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
                moveCursor_Shooting(*BoardPlayer, &cursorPostion, key);
                updateGraphics_Shoting(WBoardComputer, shotBoardPlayer, cursorPostion);
	            break;
	        case '\n': ;
                ShotResult shotResultPlayer = {0};
                if (checkShotPos(shotBoardPlayer, cursorPostion)){
                    shotResultPlayer = makeShot(*ShipsComputer, shotBoardPlayer, cursorPostion);
                    updateStats(&statisticsPlayer, &statisticsComputer, shotResultPlayer);
                }
                else {
                    // Some msg to player
                    break;
                }
                updateGraphics_Shoting(WBoardComputer, shotBoardPlayer, cursorPostion);
                DrawWInfo_Shoting(&WInfoPlayer, &statisticsPlayer);
                DrawWInfo_Shoting(&WInfoComputer, &statisticsComputer);

                if (shotResultPlayer.isHit){
                    if (isPlayerWins(&statisticsPlayer)){
                        results.playerStatus = PLAYER_WINS;
                        isAnyBodyWins = true;
                    }
                    else 
                        break; // Если игрок попал, то он ходит снова.
                }
                ShotResult shotResultComputer = {0};
                do {
                    computerShot = generateShotCoordinate(&shotBoardComputer, computerShot, &statisticsComputer);
                    // fprintf(db_out, "Coord: (%d, %d)\n", computerShot.x, computerShot.y);
                    shotResultComputer = makeShot(*ShipsPlayer, shotBoardComputer, computerShot);
                    updateStats(&statisticsComputer, &statisticsPlayer, shotResultComputer);

                    DrawWBoard_Shoting(WBoardPlayer, shotBoardComputer);
                    DrawWInfo_Shoting(&WInfoPlayer, &statisticsPlayer);
                    DrawWInfo_Shoting(&WInfoComputer, &statisticsComputer);
                    if (isPlayerWins(&statisticsComputer)){
                        results.playerStatus = PLAYER_LOSE;
                        isAnyBodyWins = true;
                    }
                } while (shotResultComputer.isHit); // Если попал компьютер, то он ходит снова.
                break;
    	}
	}
    

    clearWindowParametres(&WBackGround);
    clearWindowParametres(&WInfoComputer);
    clearWindowParametres(&WInfoPlayer);
    clearWindowParametres(&WBoardComputer);
    clearWindowParametres(&WBoardPlayer);


    clearShotBoard(&shotBoardPlayer);
    clearShotBoard(&shotBoardComputer);

    // TODO
    // DEDUG MODE ONLY 
    if (results.playerStatus == UNKONOWN) {
        results.playerStatus = PLAYER_WINS;
    }

    return results;
}


void freeDataAfterShootingLoop(ShipsInfo *ShipsPlayer, ShipsInfo *ShipsComputer, Board *BoardPlayer, Board *BoardComputer) {
    free(ShipsPlayer->Ships);
    free(ShipsComputer->Ships);

    for (int i = 0; i < BoardPlayer->Height; i++)
        free(BoardPlayer->field[i]);
    free(BoardPlayer->field);
    for (int i = 0; i < BoardComputer->Height; i++)
        free(BoardComputer->field[i]);
    free(BoardComputer->field);
}