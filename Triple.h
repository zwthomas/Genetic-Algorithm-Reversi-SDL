//
// Created by Zach on 10/26/2017.
//

#ifndef GA_REVERSI_TRIPLE_H
#define GA_REVERSI_TRIPLE_H


class Triple {
public:
    Triple(double** boardVals, double p1, double p2);
    double** getBoardHeur();
    double getP1();
    double getP2();
    void print();

protected:

private:
    double** board;
    double percent1;
    double percent2;


};


#endif //GA_REVERSI_TRIPLE_H
