#include "sudokucore.h"
SudokuCore::SudokuCore()
{
	m_board.assign(9, std::vector<int>(9,0));
	m_rowMasks.resize(9);
	m_colMasks.resize(9);
	m_sectorMasks.resize(9);
}

void SudokuCore::setCell(int row, int col, int value)
{
	if (value<0 || value>9 || row<0 || row>=9 || col<0 || col>=9)
	{
		throw std::out_of_range("Неверное значение в ячейке или ее координаты");
	}

	m_board[row][col] = value;
	updateMasks();
}

int SudokuCore::getCell(int row, int col) const
{
	if (row<0 || row>=9 || col<0 || col>=9)
	{
		throw std::out_of_range("Неверные координаты ячейки");
	}
	return m_board[row][col];
}

void SudokuCore::clearBoard()
{
	for(int row=0; row<9; row++){
		for(int col=0; col<9; col++){
			m_board[row][col] = 0;
		}
	}
	updateMasks();
}

std::bitset<10> SudokuCore::getCellCandidates(int row, int col) const
{
	if (row<0 || row>=9 || col<0 || col>=9)
	{
		throw std::out_of_range("Неверные координаты ячейки");
	}
	if (m_board[row][col]!=0)
		return std::bitset<10>();
	CellPos cell(row, col);
	int sector = cell.getSectorIndex();
	std::bitset<10> candidates = m_rowMasks[row] | m_colMasks[col] | m_sectorMasks[sector];
	std::bitset<10> res = ~candidates;
	res.set(0, false);
	return res;
}

bool SudokuCore::isValidMove(int row, int col, int value) const
{
	if (value==0)
		return true;
	if (value<1 || value>9 || row<0 || row>=9 || col<0 || col>=9)
	{
		throw std::out_of_range("Неверное значение в ячейке или ее координаты");
	}
	if (m_board[row][col] == value)
		return true;
	CellPos cell(row, col);
	int sector = cell.getSectorIndex();
	if (m_rowMasks[row].test(value) || m_colMasks[col].test(value) || m_sectorMasks[sector].test(value))
	{
		return false;
	}
	return true;
}

void SudokuCore::updateMasks()
{
	for (int i = 0; i < 9; i++)
	{
		m_rowMasks[i].reset();
		m_colMasks[i].reset();
		m_sectorMasks[i].reset();
	}
	for (int row=0; row<9; row++){
		for (int col=0; col<9; col++){
			int value = m_board[row][col];
			if (value!=0){
				CellPos cell(row, col);
				int sector = cell.getSectorIndex();
				m_rowMasks[row].set(value);
				m_colMasks[col].set(value);
				m_sectorMasks[sector].set(value);
			}
		}

	}
}

