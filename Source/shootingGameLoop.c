#include "shootingGameLoop.h"

GameResults shootingGameLoop(CoreGameData *cgdata) {
    WindowParametres WBackGround;
    WindowParametres WInfoPlayer;
    WindowParametres WInfoComputer;
    WindowParametres WBoardPlayer;
    WindowParametres WBoardComputer;
    // WindowParametres WHelp;
    initWindowsShooting(&cgdata->gdArray[PLAYER_0].board, &WBackGround, &WInfoPlayer, &WInfoComputer, &WBoardPlayer, &WBoardComputer);


    wbkgdset(WBackGround.ptrWin, COLOR_PAIR(200));
    wclear(WBackGround.ptrWin);
    // mvprintw(0, 0, "Lines: %d, cols: %d", LINES, COLS);
    wrefresh(WBackGround.ptrWin);

    PlayerStats statisticsPlayer;
    initPlayerStats(&statisticsPlayer, &cgdata->gdArray[PLAYER_0].ships);
    PlayerStats statisticsComputer;
    initPlayerStats(&statisticsComputer, &cgdata->gdArray[PLAYER_1].ships);

    // поле для стрельбы человека по компьютеру                                      
    ShotBoard shotBoardPlayer;
    initShotBoard(&shotBoardPlayer, &cgdata->gdArray[PLAYER_0].board);
    // поле для стрельбы компьютера по человеку
    ShotBoard shotBoardComputer;
    initShotBoard(&shotBoardComputer, &cgdata->gdArray[PLAYER_1].board);

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
                moveCursor_Shooting(&cgdata->gdArray[PLAYER_0].board, &cursorPostion, key);
                updateGraphics_Shoting(WBoardComputer, shotBoardPlayer, cursorPostion);
	            break;
	        case '\n': ;
                ShotResult shotResultPlayer = {0};
                if (checkShotPos(shotBoardPlayer, cursorPostion)){
                    shotResultPlayer = makeShot(&cgdata->gdArray[PLAYER_1].ships, shotBoardPlayer, cursorPostion);
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
                    shotResultComputer = makeShot(&cgdata->gdArray[PLAYER_0].ships, shotBoardComputer, computerShot);
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