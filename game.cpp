#include "game.h"
#include <QVector>
#include <iostream>

Game::Game()
{
    board.resize(board_size, QVector<int>(board_size, 0));
    solved = 0;
}

void Game::readBoard(const QString &s)
{
    int i = 0;
    for(auto &c : s)
    {
        if(c.isDigit())
        {
            board[i/board_size][i%board_size] = c.digitValue();
            i++;
        }
    }
}

bool Game::checkSolution() const
{
    QVector<bool> seen(10);

    // check rows
    for(int i = 0; i < board_size; i++)
    {
        for(auto &x : seen) x = 0;
        for(int j = 0; j < board_size; j++)
        {
            int x = board[i][j];
            if(seen[x] || x == 0) return 0;
            seen[x] = 1;
        }
    }
    // check columns
    for(int i = 0; i < board_size; i++)
    {
        for(auto &x : seen) x = 0;
        for(int j = 0; j < board_size; j++)
        {
            int x = board[j][i];
            if(seen[x] || x == 0) return 0;
            seen[x] = 1;
        }
    }
    // check 3x3 squares
    for(int i = 0; i < board_size; i++)
    {
        for(auto &x : seen) x = 0;
        for(int j = 0; j < board_size; j++)
        {
            int x = board[grid_size*(i/grid_size) + j/grid_size][grid_size*(i%grid_size) + j%grid_size];
            if(seen[x] || x == 0) return 0;
            seen[x] = 1;
        }
    }

    return 1;
}

void Game::solve(int pos)
{
    while( pos < board_size*board_size && board[pos/board_size][pos%board_size] != 0)
        pos++;
    if(pos >= board_size*board_size)
    {
        solved = 1;
        return;
    }

    for(int i = 0; i < 9; i++)
    {
        board[pos/board_size][pos%board_size] = i+1;
        if(isValid(pos/board_size, pos%board_size))
            solve(pos+1);
        if(solved) return;
    }
    board[pos/board_size][pos%board_size] = 0;
}

bool Game::isValid(int i, int j) const
{
    QVector<bool> seen(10, 0);

    // check row
    for(int k = 0; k < board_size; k++)
    {
        int x = board[i][k];
        if(seen[x] && x!=0) return 0;
        seen[x] = 1;
    }

    // check column
    for(auto &x : seen) x = 0;
    for(int k = 0; k < board_size; k++)
    {
        int x = board[k][j];
        if(seen[x] && x != 0) return 0;
        seen[x] = 1;
    }
    // check 3x3 squares

    for(auto &x : seen) x = 0;
    int sq = 3*(i/3) + j/3;
    for(int k = 0; k < board_size; k++)
    {
        int x = board[grid_size*(sq/3) + k/grid_size][grid_size*(sq%grid_size) + k%grid_size];
        if(seen[x] && x != 0) return 0;
        seen[x] = 1;
    }

    return 1;
}
