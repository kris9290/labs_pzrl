#include <iostream>
#include "sudokucore.h"

void printBoard(const SudokuCore& game){
	for (int r = 0; r < 9; r++){
		if (r%3==0 && r!=0) std::cout<<"------+------+------\n";
		for (int c = 0; c < 9; c++){
			if (c%3==0 && c!=0) std::cout<<"| ";
			int val = game.getCell(r, c);
			std::cout<<(val==0 ? ". " : std::to_string(val) + " ");
		}
		std::cout<<'\n';
	}
}
int main(){
	std::cout<<"--- Консольный модуль Судоку ---\n";
	SudokuCore game;
	try{
		game.loadFromFile("/home/kris9290/SudokuLight/test.txt");
		std::cout<<"Исходное поле:\n";
		printBoard(game);
		if (game.solve()){
			std::cout<<"\nРешение найдено:\n";
			printBoard(game);
		}
	}
	catch(const std::exception& e){
		std::cout<<"Ошибка выполнения: "<<e.what()<<'\n';
	}
	return 0;
}


