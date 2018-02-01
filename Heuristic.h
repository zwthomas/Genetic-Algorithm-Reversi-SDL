//
// Created by Craven on 10/31/2017.
//

#ifndef GA_REVERSI_HEURBOARD_H
#define GA_REVERSI_HEURBOARD_H



class Heuristic {
public:
    Heuristic();
    Heuristic(bool fill);
    ~Heuristic();
    void mutate();
    void displayHeuristic();
    double getP();
    double** getBoardHeuristic();
    void setP(double p);
    std::pair<Heuristic*, Heuristic*> crossover(Heuristic *h1, Heuristic *h2);
    void setBoardVal(int row, int col, double val);
    double getBoardVal(int row, int col);
    Heuristic* copyHeuristic();

protected:

private:
    std::pair<double **, double> *HEUR;

};


#endif //GA_REVERSI_HEURBOARD_H
