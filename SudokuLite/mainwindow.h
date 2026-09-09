#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "sudokucore.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
	void onCellChanged(int row, int col); //Ввод цифр
	void onLoadFileClicked(); //кнопка "загрузить"
	void onClearClicked(); //кнопка "очистить"
    void onSolveClicked();
    void onHintClicked();
    void onCheckUniqueClicked();

private:
    Ui::MainWindow *ui;
    SudokuCore m_gameCore;
    QLineEdit* m_cells[9][9]; //массив указателей на ячейки экрана
    bool m_isUpdating = false;
    int m_errorsCount = 0;
	
    void updateScreenFromCore(); //синхронизация экрана и ядра
};
#endif // MAINWINDOW_H
