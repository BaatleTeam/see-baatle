#include "shootingGameLoop.h"

GameResults shootingGameLoop(ShipsInfo *ShipsPlayer, ShipsInfo *ShipsComputer, Board *BoardPlayer, Board *BoardComputer) {
    WindowParametres *WBackGround = malloc(sizeof(WindowParametres));
    WindowParametres *WInfoPlayer = malloc(sizeof(WindowParametres));
    WindowParametres *WInfoComputer = malloc(sizeof(WindowParametres));
    WindowParametres *WBoardPlayer = malloc(sizeof(WindowParametres));
    WindowParametres *WBoardComputer = malloc(sizeof(WindowParametres));
    // WindowParametres *WHelp = malloc(sizeof(WindowParametres));
    initWindowsShooting(ShipsPlayer, BoardPlayer, WBackGround, WInfoPlayer, WInfoComputer, WBoardPlayer, WBoardComputer);


    wbkgdset(WBackGround->ptrWin, COLOR_PAIR(200));
    wclear(WBackGround->ptrWin);
    wrefresh(WBackGround->ptrWin);
    // mvprintw(0, 0, "Lines: %d, cols: %d", LINES, COLS);
    wrefresh(WBackGround->ptrWin);

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

    DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
    DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);
    DrawWBoard_Shoting_Default(*WBoardPlayer);
    updateGraphics_Shoting(*WBoardComputer, shotBoardPlayer, cursorPostion);

    GameResults results = {.playerStatus = UNKONOWN};

    int key;
    bool isAnyBodyWins = FALSE;

    while((key = getch()) != KEY_F(5) && isAnyBodyWins != TRUE) {
        switch(key){
            // case 9:
                // drawEndGameScreen(WBackGround->ptrWin, 9, 61, PLAYER_WINS);
            //     break;
        	case KEY_LEFT:
        	case KEY_RIGHT:
        	case KEY_UP:
        	case KEY_DOWN:
                moveCursor_Shooting(*BoardPlayer, &cursorPostion, key);
                updateGraphics_Shoting(*WBoardComputer, shotBoardPlayer, cursorPostion);
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
                updateGraphics_Shoting(*WBoardComputer, shotBoardPlayer, cursorPostion);
                DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
                DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);

                if (shotResultPlayer.isHit){
                    if (isPlayerWins(&statisticsPlayer)){
                        fprintf(db_out, "PLAYER WINS!\n");
                        results.playerStatus = PLAYER_WINS;
                        // drawEndGameScreen(WBackGround->ptrWin, 96, 8);
                        exit(0);
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

                    DrawWBoard_Shoting(*WBoardPlayer, shotBoardComputer);
                    DrawWInfo_Shoting(WInfoPlayer, &statisticsPlayer);
                    DrawWInfo_Shoting(WInfoComputer, &statisticsComputer);
                    if (isPlayerWins(&statisticsComputer)){
                        fprintf(db_out, "COMPUHTER WINS!\n");
                        results.playerStatus = PLAYER_LOSE;
                        exit(0);
                    }
                } while (shotResultComputer.isHit); // Если попал компьютер, то он ходит снова.
                break;
    	}
	}
    
    free(WBackGround);
    free(WInfoComputer);
    free(WInfoPlayer);
    free(WBoardComputer);
    free(WBoardPlayer);
    // free(WHelp);

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