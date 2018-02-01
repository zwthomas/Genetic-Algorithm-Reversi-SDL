//
// Created by Craven on 10/31/2017.
//
#include <iostream>
#include <utility>
#include <stdlib.h>
#include "Heuristic.h"

#define SIZE 8
#define MAX_RAND 300
using namespace std;

Heuristic::Heuristic(bool fill) {
    double **boardHeuristic;
    boardHeuristic = new double *[SIZE];
    for (int row = 0; row < SIZE; row++) {
        boardHeuristic[row] = new double[SIZE];
    }

    double p1 = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);

    HEUR = new pair<double **, double>(boardHeuristic, p1);
}


Heuristic::Heuristic() {

        double **boardHeuristic;
        boardHeuristic = new double *[SIZE];
        for (int row = 0; row < SIZE; row++) {
            boardHeuristic[row] = new double[SIZE];
        }

        // Fill it with heuristic values
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                boardHeuristic[row][col] = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);
            }
        }

        double p1 = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);

        HEUR = new pair<double **, double>(boardHeuristic, p1);
}
Heuristic::~Heuristic() {
    for (int row = 0; row < SIZE; row++) {
        delete [] (*HEUR).first[row];
    }

    delete [] (*HEUR).first;

    delete HEUR;
} 

void Heuristic::displayHeuristic() {
    cout << "Percent 1: " << HEUR->second << endl;
    cout << "Percent 2: " << 1 - HEUR->second << endl;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            cout << HEUR->first[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

double Heuristic::getP() {
    return (*HEUR).second;
}

double **Heuristic::getBoardHeuristic() {
    return (*HEUR).first;
}

void Heuristic::setP(double p) {
    (*HEUR).second = p;
}

std::pair<Heuristic *, Heuristic *> Heuristic::crossover(Heuristic *h1, Heuristic *h2) {
    Heuristic *new1 = new Heuristic(false);
    Heuristic *new2 = new Heuristic(false);

    int n = (rand() % SIZE);
    int m = (rand() % (SIZE - n));
    //cout << n << " " << m << endl;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < SIZE; col++) {
            (*new1).setBoardVal(row, col, (*h1).getBoardVal(row,col));
            (*new2).setBoardVal(row, col, (*h2).getBoardVal(row,col));
        }
    }

    for (int row = n; row < n + m; row++) {
        for (int col = 0; col < SIZE; col++) {
            (*new1).setBoardVal(row, col, (*h2).getBoardVal(row,col));
            (*new2).setBoardVal(row, col, (*h1).getBoardVal(row,col));
        }
    }
    for (int row = n + m; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            (*new1).setBoardVal(row, col, (*h1).getBoardVal(row,col));
            (*new2).setBoardVal(row, col, (*h2).getBoardVal(row,col));
        }
    }

    if ((double) (rand() % MAX_RAND) / (MAX_RAND - 1) > .5) {
        //cout << "Replacing board" << endl;
        (*new1).setP((*h1).getP());
        (*new2).setP((*h2).getP());
        double choice = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);
        double value;
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                value = ((*h1).getBoardVal(row, col) + (*h2).getBoardVal(row, col)) / 2;

                if (choice > .5) (*new1).setBoardVal(row, col, value);
                else (*new2).setBoardVal(row, col, value);

            }
        }
    } else {
        (*new1).setP((*h2).getP());
        (*new2).setP((*h1).getP());
    }
    return pair<Heuristic*, Heuristic*>(new1, new2);
}



void Heuristic::setBoardVal(int row, int col, double val) {
    (*HEUR).first[row][col] = val;
}

double Heuristic::getBoardVal(int row, int col) {
    return (*HEUR).first[row][col];
}

void Heuristic::mutate() {
    double p = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);
    if (p <= .05) {
        //cout << "mutating" << endl;
        HEUR->second = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);


        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if ((double) (rand() % MAX_RAND) / (MAX_RAND - 1) < .05) {
                    HEUR->first[row][col] = (double) (rand() % MAX_RAND) / (MAX_RAND - 1);
                }
            }
        }
    }
}

Heuristic* Heuristic::copyHeuristic() {
    Heuristic* hCopy = new Heuristic(false);
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            hCopy->setBoardVal(row, col, getBoardVal(row, col));
        }
    }

    hCopy->setP(getP());
    return hCopy;
}







