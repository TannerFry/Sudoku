sudoku: sudoku.o
	g++ -std=c++11 -o sudoku sudoku.o
sudoku.o: sudoku.cpp
	g++ -std=c++11 -c sudoku.cpp
run:
	bash run.sh