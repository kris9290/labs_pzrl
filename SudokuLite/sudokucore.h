#ifndef SUDOKUCORE_H
#define SUDOKUCORE_H

#include <vector>
#include <bitset>
#include <stdexcept>
#include<string>

struct CellPos{
	int row;
	int col;
	int getSectorIndex() const{
		return (row/3)*3 + (col/3);
	}
	CellPos(int r = 0, int c = 0) : row(r), col(c) {}
};

class SudokuCore{
public:
	SudokuCore();
	void setCell(int row, int col, int value); //устаговить значение 1-9 или очистить (0)
	int getCell(int row, int col) const;
	void clearBoard(); //сбросить поле
	
	std::bitset<10> getCellCandidates(int row, int col) const;
	bool isValidMove(int row, int col, int value) const; 

	void loadFromFile(const std::string& filename);
    bool solve();
    bool hasUniqueSolution();
private:
	std::vector<std::vector<int>> m_board;

	std::vector<std::bitset<10>> m_rowMasks;
	std::vector<std::bitset<10>> m_colMasks;
	std::vector<std::bitset<10>> m_sectorMasks;

	void updateMasks();
    int countSolutions(int &limit);
};
#endif
