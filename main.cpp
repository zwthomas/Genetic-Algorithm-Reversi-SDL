#include <iostream>
#include <stdlib.h>
#include <list>
#include "Triple.h"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include "Heuristic.h"
#include "Game.h"
#include <fstream>
#include <string>
#include <algorithm>
#define SAVE_FILE "heuristicSaves.txt"
#define NUM_HEUR 5
#define SIZE 8
#define GENERATIONS 50
#define NUM_THREADS 5
using namespace std;


void testCrossover() {
    Heuristic *new1 = new Heuristic(false);
    Heuristic *new2 = new Heuristic(false);

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            new1->setBoardVal(row, col, 0);
            new2->setBoardVal(row, col, 1);
        }
    }
    new1->displayHeuristic();
    new2->displayHeuristic();

    pair<Heuristic *, Heuristic*> p = new1->crossover(new1,new2);
    p.first->displayHeuristic();
    p.second->displayHeuristic();

}
void testMutate() {
    Heuristic *new1 = new Heuristic();
    new1->displayHeuristic();
    for (int i = 0; i < 100; i++) {
        new1->mutate();
    }

    new1->displayHeuristic();
}

pair<Heuristic*, int>** createHeuristics(int numberHeuristics) {
    pair<Heuristic*, int>** boards = new pair<Heuristic*, int> *[numberHeuristics];
    for (int ndx = 0; ndx < numberHeuristics; ndx++) {
        boards[ndx] = new pair<Heuristic*, int> (new Heuristic(), 0);
    }

    return boards;
}

pair<Heuristic*, int>** readBoardsFromFile(int numberHeuristics) {
    ifstream fin;
    fin.open(SAVE_FILE, ios::in);
    string num;
    pair<Heuristic*, int> **boards = new pair<Heuristic*, int> *[numberHeuristics];
    for (int count = 0; count < numberHeuristics; count++) {
        Heuristic *h = new Heuristic(false);
        getline(fin, num, ' ');

        h->setP(atof(num.c_str()));
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                getline(fin, num, ' ');
                h->setBoardVal(row, col,atof(num.c_str()));
            }
        }
        boards[count] = new pair<Heuristic*, int> (h, 0);
        fin.ignore();
    }

    fin.close();
    return boards;
}


void remove(Heuristic **b, int length) {
    for (int ndx = 0; ndx < length; ndx++) {
        delete b[ndx];
    }

    delete [] b;
}

void writeBoardsToFile(pair<Heuristic*, int> **boards, string file) {
    ofstream fout;
    fout.open(file, ios::out | ios::trunc);

    for (int ndx = 0; ndx < NUM_HEUR; ndx++) {
        fout << boards[ndx]->first->getP() << " " << endl;
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                fout << boards[ndx]->first->getBoardVal(row,col) << " ";
                //SDL_Delay(100);
            }
            fout << endl;
        }
    }
    fout.close();
}

bool sorter(pair<Heuristic*, int> *v1, pair<Heuristic*, int> *v2) { return v1->second > v2->second;}

void crossOver(pair<Heuristic*, int> **boards) {
    sort(boards, boards + NUM_HEUR, sorter);

    int startingNdx, numToRemove = 0;
    while (numToRemove + 2 <= NUM_HEUR / 2) {
        numToRemove += 2;
        //cout << numToRemove << endl;
    }
//    cout << NUM_HEUR << endl;
    startingNdx = NUM_HEUR - numToRemove;

//    cout << "Starting: " << startingNdx << endl;
//    cout << "Should be: " << (NUM_HEUR - startingNdx) / 2 << " cross(es)" << endl;
    for (int ndx = startingNdx; ndx < NUM_HEUR; ndx++) {
        delete boards[ndx];
    }
    int n, m;
    for (int ndx = startingNdx; ndx < NUM_HEUR; ndx += 2) {
        n = (rand() % (startingNdx));
        m = (rand() % (startingNdx));
        //cout << "Crossing: " << n << "  " << m << endl;
        pair<Heuristic*, Heuristic*> newBoards = boards[n]->first->crossover(boards[n]->first, boards[m]->first);
        boards[ndx] = new pair<Heuristic*, int> (newBoards.first, 0);
        boards[ndx + 1] = new pair<Heuristic*, int> (newBoards.second, 0);
    }

    // Reset all win counts to zero and mutate
    for (int ndx = 0; ndx < NUM_HEUR; ndx++) {
        boards[ndx]->second = 0;
        boards[ndx]->first->mutate();
    }
}

pair<Heuristic*, int>** copyHeuristics(pair<Heuristic*, int> **b) {
    pair<Heuristic*, int>** boards = new pair<Heuristic*, int> *[NUM_HEUR];
    for (int ndx = 0; ndx < NUM_HEUR; ndx++) {
        boards[ndx] = new pair<Heuristic*, int> (b[ndx]->first->copyHeuristic(), 0);
    }
    return boards;
}

int playGames(void* input) {
    auto *in = (pair<pair<Heuristic*, int>**, int>*) input;
    auto *boards = in->first;
    int threadNum = in->second;
    int winner;
    for (int i = 0; i < NUM_HEUR / NUM_THREADS; i++) {
        for (int j = 0 ; j < NUM_HEUR;j++) {
            if ((NUM_HEUR / NUM_THREADS) * threadNum + i > NUM_HEUR || (NUM_HEUR / NUM_THREADS) * threadNum + i < 0) {
                cerr << "Seg fault" << endl;
            }
            Game g = Game(boards[(NUM_HEUR / NUM_THREADS) * threadNum + i]->first, boards[j]->first);
            winner = g.run();
            if (winner == 1) {
                boards[i]->second++;
            } else {
                boards[j]->second++;
            }
        }
    }
    return 0;
}

void combineScores(pair<Heuristic*, int> **boards, list<pair<Heuristic*, int>**> copies) {
    list<pair<Heuristic*, int>**>::iterator it = copies.begin();

    while (it != copies.end()) {
        for (int i = 0; i < NUM_HEUR; i++) {
            boards[i]->second += (*it)[i]->second;
            delete (*it)[i]->first; // Deletes the board
            delete (*it)[i];        // Deletes the pair
        }
        delete (*it++);             // Deletes the array of pairs
    }

}

int main() {
    srand((unsigned) time(nullptr));
    pair<Heuristic*, int> **boards;
    Heuristic* previous = new Heuristic();
    previous->setP(1);



    if (ifstream(SAVE_FILE)) {
        cout << "Loading heuristics" << endl;
        boards = readBoardsFromFile(NUM_HEUR);

    } else {
        cout << "Creating new heuristics " <<  endl;
        boards = createHeuristics(NUM_HEUR);
    }

    for (int gen = 1; gen <= GENERATIONS; gen++) {
        SDL_Thread *threads[NUM_THREADS];               // Holds threads
        list<pair<Heuristic*, int>**> copies;           // Holds board copies passed to threads


        cout << "Gen: " << gen << endl;
        cout << "Playing matches" << endl;

        cout << "Creating threads" << endl;
        // Creates the threads
        for (int threadNum = 0; threadNum < NUM_THREADS; threadNum++) {
            pair<Heuristic*, int>** copy = copyHeuristics(boards);      // Creates the copies for this thread
            copies.push_back(copy);                                     // Saves the copy to be deleted later

            // So we can delete the object we cast as a void pointer
            auto *holder = new pair<pair<Heuristic*, int>**, int>(copy, threadNum);
            threads[threadNum] = SDL_CreateThread(playGames,"Thread", (void*) holder);
            delete holder;
        }

        cout << "Waiting on threads" << endl;
        for(int i = 0; i < NUM_THREADS; i++) {
            SDL_WaitThread(threads[i], NULL);
        }
        cout << "Combnining scores" << endl;
        combineScores(boards, copies);

        cout << "Crossing " << endl;
        crossOver(boards);
        cout << "Saving\n" << endl;
        writeBoardsToFile(boards, SAVE_FILE);
        if (gen % 25 == 0) {
            writeBoardsToFile(boards, "gen" + to_string(gen) + ".txt");
        }

    }
    return 0;
}