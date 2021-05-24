#ifndef MATRIXGAMEPROBLEM_MATRIXGAMERESULT_H
#define MATRIXGAMEPROBLEM_MATRIXGAMERESULT_H

#include "matrix.h"

struct MatrixGameResult {
    db v = 0;
    dvector p, q;

    void AddDelta(db delta)
    {
        v += delta;
    }
};

std::ostream &operator<<(std::ostream &out, const MatrixGameResult &res) {
    out << "Game Cost = " << res.v << std::endl <<
        "p = { " << res.p << "}\n" <<
        "q = { " << res.q << "}\n";
    return out;
}


#endif //MATRIXGAMEPROBLEM_MATRIXGAMERESULT_H
