#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем центральный виджет и сетку
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(2);

    // Ограничиваем ввод: только одна цифра от 1 до 9
    QRegularExpression regExp("[1-9]");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);

    // Циклом создаем 81 ячейку для Судоку
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit *cell = new QLineEdit(this);
	    m_cells[row][col] = cell;

            // Настраиваем внешний вид ячейки
            cell->setFixedSize(45, 45);             // Размер квадратика
            cell->setAlignment(Qt::AlignCenter);     // Цифра строго по центру
            cell->setValidator(validator);           // Запрещаем вводить буквы и нули
            cell->setFont(QFont("Arial", 16, QFont::Bold)); // Делаем шрифт крупным

	    // Выделяем сектора
	    int top = (row%3==0) ? 3 : 1;
	    int left = (col%3==0) ? 3 : 1;
	    int bottom = (row==8) ? 3 : 1;
	    int right = (col==8) ? 3 : 1;

	    cell->setStyleSheet(QString(
			"QLineEdit {""border-style: solid;" "border-width: %1px %2px %3px %4px;"
			"border-color: #444444 #444444 #444444 #444444;" "background-color: #FFFFFF;""}"
	    ).arg(top).arg(right).arg(bottom).arg(left));

            // Добавляем ячейку в сетку (строка, столбец)
            gridLayout->addWidget(cell, row, col);

        connect(cell, &QLineEdit::textChanged, [this, row, col]()
	    {
	    	this->onCellChanged(row, col);
	    });
        }
    }

    mainLayout->addLayout(gridLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *btnLoad = new QPushButton("Загрузить файл", this);
    QPushButton *btnClear = new QPushButton("Очистить поле", this);
    buttonLayout->addWidget(btnLoad);
    buttonLayout->addWidget(btnClear);
    mainLayout->addLayout(buttonLayout);

    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadFileClicked);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    setCentralWidget(centralWidget);
    setWindowTitle("Судоку");
    setFixedSize(sizeHint());
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Обработка ввода пользователя
void MainWindow::onCellChanged(int row, int col)
{
    QLineEdit *cell = m_cells[row][col];
    int top = (row % 3 == 0) ? 3 : 1;
    int left = (col % 3 == 0) ? 3 : 1;
    int bottom = (row == 8) ? 3 : 1;
    int right = (col == 8) ? 3 : 1;

    QString baseStyle = QString(
        "border-style: solid; border-color: #2c3e50; "
        "border-width: %1px %2px %3px %4px; "
        ).arg(top).arg(right).arg(bottom).arg(left);

    // Если ячейка пустая — возвращаем базовый белый цвет
    if (cell->text().isEmpty())
    {
        m_gameCore.setCell(row, col, 0);
        cell->setStyleSheet(baseStyle + "background-color: #FFFFFF; color: #000000;");
        return;
    }
    int val = cell->text().toInt();

    cell->blockSignals(true);

    if (m_gameCore.isValidMove(row, col, val))
    {
        m_gameCore.setCell(row, col, val);
        cell->setStyleSheet(baseStyle + "background-color: #C2F0C2; color: #1E6B30;");
    } else
    {
        cell->setStyleSheet(baseStyle + "background-color: #FFC2C2; color: #B31C1C;");
    }

    cell->blockSignals(false);
}
//Загрузка через всплывающее окно выбора файла
void MainWindow::onLoadFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть Судоку", "", "Text Files (*.txt)");
	if (fileName.isEmpty())
		return;
	try{
		m_gameCore.loadFromFile(fileName.toStdString());
		updateScreenFromCore();
		QMessageBox::information(this, "Успех", "Судоку успешно загружено");
	}
	catch(const std::exception& e)
	{
		QMessageBox::critical(this, "Ошибка", e.what());
	}
}
//Очистка 
void MainWindow::onClearClicked()
{
	m_gameCore.clearBoard();
	updateScreenFromCore();
}

//Обновление экрана
void MainWindow::updateScreenFromCore()
{
	for (int r=0; r<9; r++)
	{
		for (int c=0; c<9; c++)
		{
			int val = m_gameCore.getCell(r, c);
            m_cells[r][c]->blockSignals(true);
			if(val==0)
			{
				m_cells[r][c]->clear();
				m_cells[r][c]->setStyleSheet(m_cells[r][c]->styleSheet() + "background-color: #FFFFF; color: black;");
			}
			else
			{
				m_cells[r][c]->setText(QString::number(val));
				m_cells[r][c]->setStyleSheet(m_cells[r][c]->styleSheet() + "background-color: #F2F2F2; color: #444444;");
			}
            m_cells[r][c]->blockSignals(false);
		}
	}
}

