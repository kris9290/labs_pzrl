#include<fstream>
#include "sudokucore.h"
SudokuCore::SudokuCore()
{
	m_board.assign(9, std::vector<int>(9,0));
	m_rowMasks.resize(9);
	m_colMasks.resize(9);
	m_sectorMasks.resize(9);
}

void SudokuCore::setCell(int row, int col, int value) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9 || value < 0 || value > 9) {
        throw std::out_of_range("Неверные координаты или значение");
    }

    int oldValue = m_board[row][col];
    CellPos cell(row, col);
    int sector = cell.getSectorIndex();

    // Если в ячейке уже была цифра, выключаем её старый бит в масках
    if (oldValue != 0) {
        m_rowMasks[row].reset(oldValue);
        m_colMasks[col].reset(oldValue);
        m_sectorMasks[sector].reset(oldValue);
    }

    // Записываем новое значение
    m_board[row][col] = value;

    // Если это не ноль, включаем новый бит в масках
    if (value != 0) {
        m_rowMasks[row].set(value);
        m_colMasks[col].set(value);
        m_sectorMasks[sector].set(value);
    }
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

bool SudokuCore::isValidMove(int row, int col, int value) const {
    if (value == 0) return true; // Ноль (стирание) разрешено всегда
    if (value < 1 || value > 9 || row < 0 || row >= 9 || col < 0 || col >= 9) return false;

    CellPos cell(row, col);
    int sector = cell.getSectorIndex();

    // Если этот бит уже занят в строке, столбце или секторе — ход запрещен!
    if (m_rowMasks[row].test(value) || m_colMasks[col].test(value) || m_sectorMasks[sector].test(value)) {
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

void SudokuCore::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
	if (!file.is_open()){
		throw std::runtime_error("Не удалось открыть файл " + filename);
	}

	clearBoard();

	char ch;
	int row = 0;
	int col = 0;

    while(file>>ch && row<9)
	{
		if(ch>='1' && ch<='9'){
			int value = ch - '0';
            setCell(row, col, value);
			col+=1;
		}
		else if (ch=='0'){
			setCell(row, col, 0);
			col+=1;
		}
		else{
            continue;
		}
		if (col==9){
			row+=1;
			col=0;
		}
	}
	file.close();
	if (row!=9){
		clearBoard();
		throw std::invalid_argument("Файл содержит неверный формат поля");
	}
}
bool SudokuCore::solve() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            // Ищем строго ПУСТУЮ ячейку
            if (m_board[row][col] == 0) {
                for (int num = 1; num <= 9; ++num) {
                    if (isValidMove(row, col, num)) {

                        setCell(row, col, num); // Включает маску этой цифры автоматически

                        if (solve()) return true; // Идем дальше

                        setCell(row, col, 0); // Шаг назад: выключает маску автоматически
                    }
                }
                return false; // Сюда ничего не подошло — тупик ветки
            }
        }
    }
    return true; // Пустых клеток нет — Судоку решено!
}

int SudokuCore::countSolutions(int &limit)
{
    if (limit<=0) return 0;
    for (int row = 0; row < 9; row++){
        for(int col = 0; col < 9; col++)
        {
            if (m_board[row][col]==0){
                int count = 0;
                for (int num = 1; num<=9; num++){
                    if (isValidMove(row, col, num)){
                        setCell(row, col, num);
                        count+=countSolutions(limit);
                        setCell(row, col, 0);
                        if (limit<=0) return count;
                    }
                }
                return count;
            }
        }

    }
    limit-=1;
    return 1;
}

bool SudokuCore::hasUniqueSolution(){
    int limit = 2;
    auto backup = m_board;
    int solutions = countSolutions(limit);
    m_board = backup;
    updateMasks();
    return solutions = 1;
}
