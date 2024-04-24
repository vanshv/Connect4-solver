#include "position.hpp"
#include "transposition_table.hpp"
#include "helper.hpp"
#include <sys/time.h>
#include <iostream>

class Solver
{
public:
    // explored nodes.
    long long nodeCount;
    vector<int> columnOrder = {3, 2, 4, 1, 5, 0, 6};
    TranspositionTable tt;

    int negamax(Position &P, int alpha, int beta)
    {
        nodeCount++;

        // draw condition
        if (P.moves == WIDTH * HEIGHT)
            return 0;

        // win condition
        for (int x = 0; x < WIDTH; x++)
            if (P.canPlay(x) && P.isWinningMove(x))
                return (WIDTH * HEIGHT + 1 - P.moves) / 2;

        int max = (WIDTH * HEIGHT - 1 - P.moves) / 2;
        int val = tt.get(P.key());
        if (val){
            max = val + MIN_SCORE - 1;
        }

        if (beta > max)
        {
            beta = max;
            if (alpha >= beta)
            {
                return beta;
            }
        }

        //  find best move
        for (int x = 0; x < WIDTH; x++)
        {
            if (P.canPlay(columnOrder[x]))
            {
                Position P2(P);
                P2.playMove(columnOrder[x]);
                int score = -negamax(P2, -beta, -alpha);

                if (score >= beta)
                {
                    return score;
                }
                if (score > alpha)
                {
                    alpha = score;
                }
            }
        }

        tt.put(P.key(), alpha - MIN_SCORE + 1);
        return alpha;
    }

    // 98746231 -> prime
    Solver() : nodeCount{0}, tt(98746231)
    { 
        reset();
    }

    void reset()
    {
        tt.reset();
    }

    int solve(Position &P)
    {
        nodeCount = 0;
        return negamax(P, -WIDTH * HEIGHT / 2, WIDTH * HEIGHT / 2);
    }
};

int main()
{
    Solver solver;
    string line;

    for (int l = 1; getline(cin, line); l++)
    {
        Position P;
        P.playSeq(line);
        solver.reset();
        solver.solve(P);
        cout << l << " " << line << " " << solver.nodeCount << " " << "\n";
    }
}