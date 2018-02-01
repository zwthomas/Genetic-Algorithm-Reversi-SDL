//
// Created by Craven on 9/19/2017.
//

#ifndef REVERSI_RUNGAME_H
#define REVERSI_RUNGAME_H


#include <utility>
#include "Board.h"
#include "Triple.h"
#include "Heuristic.h"


class Game {
public:
    Game(Heuristic *player1, Heuristic *player2);
    ~Game();

    int run();
protected:

private:
    Board *board;
    double heuristic(Board *boardState, bool maxPlayer);
    int player1Ply;
    int player2Ply;
    bool gameover;
    Heuristic *player1Heur;
    Heuristic *player2Heur;
    int playersMove;
    bool validMove;
    bool moveRemaining(Board *state, int player);
    bool checkPosition(Board *b, int player, int xPos, int yPos, bool makeMove);
    void readInMove();
    double alphabeta(Board *state, int depth, double alpha, double beta, bool maximizingPlayer, int player, int startDepth);
    double sumBoardVals(double **boardH, Board *boardState, int player);
};


#endif //REVERSI_RUNGAME_H
