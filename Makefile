SRC=Source/
LIB=-lpdcurses -g
BLD=Build
all: $(BLD)/main.o $(BLD)/ships.o $(BLD)/arrange.o $(BLD)/shoot.o $(BLD)/table.o
	gcc $(BLD)/main.o $(BLD)/ships.o $(BLD)/arrange.o $(BLD)/shoot.o $(BLD)/table.o -o $(BLD)/sea_battle.exe $(LIB)
$(BLD)/main.o: $(SRC)/main.c $(SRC)/header.h $(BLD)/ships.o $(BLD)/arrange.o $(BLD)/shoot.o $(BLD)/table.o
	gcc -c $(SRC)/main.c -o $(BLD)/main.o $(LIB) 
$(BLD)/ships.o: $(SRC)/ships.c $(SRC)/ships.h
	gcc -c $(SRC)/ships.c -o $(BLD)/ships.o $(LIB) 
$(BLD)/arrange.o: $(SRC)/arrange.c $(SRC)/arrange.h
	gcc -c $(SRC)/arrange.c -o $(BLD)/arrange.o $(LIB)
$(BLD)/shoot.o: $(SRC)/shoot.c $(SRC)/shoot.h
	gcc -c $(SRC)/shoot.c -o $(BLD)/shoot.o $(LIB)
$(BLD)/table.o: $(SRC)/table.c $(SRC)/table.h
	gcc -c $(SRC)/table.c -o $(BLD)/table.o $(LIB)
	
