#ifndef GAME_H
#define GAME_H
#include <QVector>

class Game
{
public:
    Game();
    void readBoard(const QString &s);
    bool checkSolution() const;
    QVector<QVector<int>>& getBoard() {return board;}
    void solve(int pos = 0);
    bool solved;
private:
    QVector<QVector<int>> board;
    const int board_size = 9;
    const int grid_size = board_size/3;   
    bool isValid(int, int) const;

};

#endif // GAME_H
