#include "mainwindow.h"
#include "qframe.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <algorithm>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "game.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createBoardUi();
    // 530070000600195000098000060800060003400803001700020006060000280000419005000080079
    // 827154396965327148341689752593468271472513689618972435786235914154796823239841560
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createBoardUi()
{
    int board_size = game.getBoard().size();
    int grid_size = board_size/3;
    cells.resize(board_size, QVector<QLineEdit*>(board_size, nullptr));
    QRegularExpression expr("[1-9]{0,1}");

    for(int i = 0; i < board_size; i++)
    {
        // horizontal lines
        if(i%grid_size == 0 && i != 0)
            for(int j = 0; j < board_size + board_size/grid_size - 1; j++)
            {
                QFrame *hline = new QFrame;
                hline->setFrameShape(QFrame::HLine);
                ui->GridLayout->addWidget(hline, i - 1 + i/grid_size, j);
            }
        for(int j = 0; j < board_size; j++)
        {
            // vertical lines
            if(j%grid_size == 0 && j != 0)
                for(int t = 0; t < board_size + board_size/grid_size - 1; t++)
                {
                    QFrame *vline = new QFrame;
                    vline->setFrameShape(QFrame::VLine);
                    ui->GridLayout->addWidget(vline, t, j - 1 + j/grid_size);
                }
            QLineEdit *cell = new QLineEdit;
            cell->setValidator(new QRegularExpressionValidator(expr));
            cell->setAlignment(Qt::AlignCenter);
            cells[i][j] = cell;
            ui->GridLayout->addWidget(cell, i + i/3, j + j/3);
            cell->show();
        }
    }
}

void MainWindow::displayBoard(bool newGame)
{
    auto &board = game.getBoard();
    QPalette gray, white;
    gray.setColor(QPalette::Base,Qt::lightGray);
    white.setColor(QPalette::Base,Qt::white);

    for(int i = 0; i < board.size(); i++)
        for(int j = 0; j < board.size(); j++)
        {
            int x = board[i][j];
            if(x)
            {    
                cells[i][j]->setText(QString::number(x));
                if(newGame)
                {
                    cells[i][j]->setPalette(gray);
                    cells[i][j]->setReadOnly(1);
                }

            }
            else if(newGame)
            {
                cells[i][j]->setText(QString());
                if(newGame)
                {
                    cells[i][j]->setPalette(white);
                    cells[i][j]->setReadOnly(0);
                }
            }

        }
}

void MainWindow::updateBoard()
{
    auto &board = game.getBoard();
    int board_size = board.size();
    for(int i = 0; i < board_size; i++)
        for(int j = 0; j < board_size; j++)
        {
            board[i][j] = cells[i][j]->text().size() ? cells[i][j]->text().toInt() : 0;
        }
}


void MainWindow::on_pushButton_clicked()
{
    updateBoard();
    ui->label->setText(game.checkSolution() ? "Status: sovled!" : "Status: unsolved");
}


void MainWindow::on_pushButton_2_clicked()
{
    auto s = ui->plainTextEdit->toPlainText();
    if(s.size() != 81)
    {
        QMessageBox box;
        box.setText("The game code must be exactly 81 characters long");
        box.exec();
    }
    for(auto &c : s)
        if(!c.isDigit())
            c = '0';
    game.readBoard(s);
    displayBoard();
}


void MainWindow::on_pushButton_3_clicked()
{
    game.solved = 0;
    game.solve();
    if(!game.checkSolution())
        ui->label->setText("Status: unsolvable");
    displayBoard(0);
}

