//
// Created by Zach on 10/26/2017.
//
#include <iostream>
#include "Triple.h"
using namespace std;

Triple::Triple(double **boardVals, double p1, double p2) {
    board = boardVals;
    percent1 = p1;
    percent2 = p2;
}

double **Triple::getBoardHeur() {
    return board;
}

double Triple::getP1() {
    return percent1;
}

double Triple::getP2() {
    return percent2;
}

void Triple::print() {
    cout << "Percent 1: " << percent1 << endl;
    cout << "Percent 2: " << percent2 << endl;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            cout << board[row][col] << " " ;
        }
        cout << endl;
    }
    cout << endl;
}

