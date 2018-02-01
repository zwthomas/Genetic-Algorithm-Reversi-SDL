/*
 * Zachary Thomas
 * 9/19/2017
 *
 * Holds the game logic for Reversi.
 */

#include "Game.h"
#include <iostream>
#define SIZE 8
#define PLY 2

using namespace std;



/**
 * Runs full game. The moves are generated using alpha-beta pruning. The simple heuristic used is number of pieces. Runs
 * until both players do not have a move.
 */
int Game::run() {
    bool maxPlayer = true, movesLeftPlayer1, movesLeftPlayer2;
//    (*player1Heur).displayHeuristic();
//    (*board).printBoard();
//    cout << heuristic(board, false) << endl;
//    return;

    // Game loop
    while (!gameover) {

        movesLeftPlayer1 = true;
        movesLeftPlayer2 = true;

        // If player 1 has a move make it
        if (moveRemaining(board, 1)) {
            alphabeta(board, player1Ply, -1, 1, maxPlayer, 1, player1Ply);
        } else {
            movesLeftPlayer1 = false;
        }

        // If player 2 has a move make it
        if (moveRemaining(board, 2)) {
            alphabeta(board, player2Ply, -1, 1, !maxPlayer, 2, player2Ply);
        } else {
            movesLeftPlayer2 = false;
        }

        // If both players couldn't make move game is over
        if (!movesLeftPlayer1 && !movesLeftPlayer2) {
            gameover = true;
        }

    }

    // Display results
    std::pair<int, int> score = (*board).score();
    if (score.first > score.second)
        return 1;
    else
        return 2;


}

/**
 * For testing purposes. Allows moves to manually be entered.
 */
void Game::readInMove() {
    validMove = false;
    int inputRow;
    int inputCol;

    // Reads in moves until a valid move is entered
    while (!validMove) {
        // Prompts and read input
        std::cout << "Player " << playersMove << " enter the row and column to place your piece (both 0-7):"
                  << std::endl;
        std::cin >> inputRow >> inputCol;

        // Checks if valid
        if (inputCol < 8 && inputRow > -1 && inputRow < 8 && inputCol > -1 &&
            (*board).isMoveValid(playersMove, inputCol, inputRow)) {
            validMove = true;
        } else {
            std::cout << "Input values are not a valid move." << std::endl;
        }
    }
    std::cout << "Point accepted" << std::endl;
}

/**
 * Iterates over the board checking each position to see if there a piece belonging to player can be placed there.
 *
 * @param state The game board
 * @param player The player whose moves we are checking for
 * @return true if player has a least 1 move
 */
bool Game::moveRemaining(Board *state, int player) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (checkPosition(state, player, col, row, false)) return true;
        }
    }
    return false;
}

/**
 * Checks all positions off current position to see if the move is valid.
 *
 * @param b The game board
 * @param player The player whose turn it is
 * @param xPos The column of the move
 * @param yPos The row of the move
 * @param makeMove Boolean to tell if the piece should be placed.
 * @return true if piece can be placed here
 */
bool Game::checkPosition(Board *b, int player, int xPos, int yPos, bool makeMove) {
    if ((*b).checkUpperLeftDiagonal(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkUp(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkUpperRightDiagonal(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkRight(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkLowerRightDiagonal(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkDown(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkLowerLeftDiagonal(player, xPos, yPos, makeMove)) {
        return true;
    }

    if ((*b).checkLeft(player, xPos, yPos, makeMove)) {
        return true;
    }

    return false;
}

/**
 * Performs the alpha-beta pruning to find the best move.
 *
 * @param state The game board
 * @param depth Recursion depth
 * @param alpha Alpha value
 * @param beta Beta value
 * @param maximizingPlayer Tells us if me are trying to maximize of minimize
 * @param player The player whose turn it is
 * @param startDepth Saved start depth because this is a count down
 * @return heuristic from the search
 */
double Game::alphabeta(Board *state, int depth, double alpha, double beta, bool maximizingPlayer, int player, int startDepth) {
    int bRow, bCol;
    int otherPlayer;

    // Determines other player for recursive call
    if (player == 1) {
        otherPlayer = 2;
    } else {
        otherPlayer = 1;
    }

    // Base case
    if (depth == 0 || !moveRemaining(state, player)) {
        return heuristic(state, maximizingPlayer);
    }

    // Init v
    double v;
    if (maximizingPlayer)       v = -1;
    else                        v = 1;

    // Go through all the positions on the board
    for (int row = 0; row < (*state).size; row++) {
        for (int col = 0; col < (*state).size; col++) {

            // If the input player can make a move at this position
            if (checkPosition(state, player, col, row, false)) {

                Board *copy = new Board(state); // Copy the board
                (*copy).isMoveValid(player, col, row); // Have them make move on copy

                if (maximizingPlayer) {

                    // Recursive search
                    v = std::max(v, alphabeta(copy, depth - 1, alpha, beta, false, otherPlayer, startDepth));

                    // Prune
                    if (v >= beta) {
                        delete copy;
                        return v;
                    }

                    // Alpha needs updated
                    if (alpha < v) {

                        // If we are in the base case this is the new move we are going to make
                        if (depth == startDepth) {
                            bRow = row;
                            bCol = col;
                        }

                        alpha = v; // Update alpha
                    }

                } else {

                    // Recursive search
                    v = std::min(v, alphabeta(copy, depth - 1, alpha, beta, true, otherPlayer, startDepth));

                    // Prune
                    if (v <= alpha){
                        delete copy;
                        return v;
                    }

                    // Beta needs updated
                    if (v < beta) {

                        // If we are at first turn in the search this is the move we are going to make
                        if (depth == startDepth) {
                            bRow = row;
                            bCol = col;
                        }

                        beta = v; // Update beta
                    }
                }

                delete copy; // Delete the no longer needed board

            }
        }
    }

    // Make the best move
    if (depth == startDepth) {
        (*board).isMoveValid(player, bCol, bRow);
    }
    return v;
}
Game::~Game() {
    delete board;
}
Game::Game(Heuristic *player1, Heuristic *player2) {
    player1Heur = player1;
    player2Heur = player2;
    board = new Board();
    gameover = false;
    playersMove = 1;
    player1Ply = PLY;
    player2Ply = PLY;
}

double Game::heuristic(Board *boardState, bool maxPlayer) {
    double heur;
    double numPiece, bVal;
    int num1 = (*boardState).playerPieces(1), num2 = (*boardState).playerPieces(2);
    if (maxPlayer) {
        numPiece = ((num1 - num2) / (num1 + num2)) * ((*player1Heur).getP());
        bVal = (1 - (*player1Heur).getP()) * sumBoardVals((*player1Heur).getBoardHeuristic(), boardState, 1);
        heur = numPiece + bVal;
    } else {
        numPiece = ((num2 - num1) / (num1 + num2)) * (*player2Heur).getP();
        bVal = (1 - (*player2Heur).getP()) * sumBoardVals((*player2Heur).getBoardHeuristic(), boardState, 2);
        heur = -1 * (numPiece + bVal);
    }

    if (heur > 1) cout << "ERROR" << endl;

    return heur;
}

double Game::sumBoardVals(double **boardH, Board *boardState, int player) {
    double playerSum = 0;
    double totalSum = 0;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            totalSum += boardH[row][col];
            if ((*boardState).get(row, col) == player)
                playerSum += boardH[row][col];
        }
    }
    return playerSum / totalSum;
}


