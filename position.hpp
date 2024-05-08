#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int WIDTH = 7;
int HEIGHT = 6;
int MIN_SCORE = -(WIDTH * HEIGHT) / 2 + 3;

class PositionOld
{
public:
    // 0 if cell is empty
    // 1 for first player
    // 2 for second player.
    vector<vector<int>> board;
    // number of stones per column
    vector<int> height;
    int moves;

    // build an empty positon
    PositionOld()
    {
        board.assign(WIDTH, vector<int>(HEIGHT, 0));
        height.assign(WIDTH, 0);
        moves = 0;
    }

    bool canPlay(int col)
    {
        return height[col] < HEIGHT;
    }

    // 0-based index of a playable column.
    void playMove(int col)
    {
        board[col][height[col]] = 1 + (moves%2);
        height[col]++;
        moves++;
    }

    // 1-based index
    int playSeq(string seq)
    {
        for (int i = 0; i < seq.size(); i++)
        {
            int col = seq[i] - '1';
            // invalid move
            if (col < 0 || col >= WIDTH || !canPlay(col) || isWinningMove(col))
                return i;

            playMove(col);
        }
        return seq.size();
    }

    // 3 cases - horizontal, vertical, diagonal
    bool isWinningMove(int col)
    {
        int cur = 1 + (moves%2);

        // check for vertical alignments
        if (height[col] >= 3 && board[col][height[col] - 1] == cur && board[col][height[col] - 2] == cur && board[col][height[col] - 3] == cur)
        {
            return true;
        }

        for (int dy = -1; dy <= 1; dy++)
        {
            int counter = 0;
            for (int dx = -1; dx <= 1; dx += 2)
            {
                int x = col + dx;
                int y = height[col] + dx * dy;
                while (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && board[x][y] == cur)
                {
                    x += dx;
                    y += dx * dy;
                    counter++;
                }
            }
            if (counter >= 3)
                return true;
        }
        return false;
    }
};

class Position
{
public:
    long long currPos;
    long long mask;
    int moves;

    // build an empty positon
    Position()
    {
        currPos = 0;
        mask = 0;
        moves = 0;
    }

    // return a bitmask containg a single 1 corresponding to the top cell of a given column
    long long top_mask(int col)
    {
        return (1LL << (HEIGHT - 1)) << col * (HEIGHT + 1);
    }

    // return a bitmask containg a single 1 corresponding to the bottom cell of a given column
    long long bottom_mask(int col)
    {
        return 1LL << col * (HEIGHT + 1);
    }

    // return a bitmask 1 on all the cells of a given column
    long long column_mask(int col)
    {
        return ((1LL << HEIGHT) - 1) << col * (HEIGHT + 1);
    }

    bool canPlay(int col)
    {
        return (mask & top_mask(col)) == 0;
    }

    bool canWinNext()
    {
        return winning_position() & possible();
    }

    long long possibleNonLoosingMoves()
    {
        long long possible_mask = possible();
        long long opponent_win = opponent_winning_position();
        long long forced_moves = possible_mask & opponent_win;
        if (forced_moves)
        {
            if (__builtin_popcount(forced_moves) > 1) // check if there is more than one forced move
                return 0;                          // the opponnent has two winning moves and you cannot stop him
            else
                possible_mask = forced_moves; // enforce to play the single forced move
        }
        return possible_mask & ~(opponent_win >> 1); // avoid to play below an opponent winning spot
    }

    long long winning_position() 
    {
        return compute_winning_position(currPos, mask);
    }

    long long opponent_winning_position() 
    {
        return compute_winning_position(currPos ^ mask, mask);
    }

    long long possible() 
    {
        return (mask + bottomMask) & board_mask;
    }

    long long compute_winning_position(long long pos, long long mask)
    {
        int h = HEIGHT;
        // vertical
        // .
        // x
        // y
        // x
        long long r = (pos << 1) & (pos << 2) & (pos << 3);

        // horizontal
        long long p = (pos << (h + 1)) & (pos << 2 * (h + 1));
        r |= p & (pos << 3 * (h + 1));
        // . x y z

        r |= p & (pos >> (h + 1));
        // x . y z

        p = (pos >> (h + 1)) & (pos >> 2 * (h + 1));
        // x y . z
        r |= p & (pos << (h + 1));

        // x y z .
        r |= p & (pos >> 3 * (h + 1));

        // diagonal 1
        p = (pos << h) & (pos << 2 * h);
        r |= p & (pos << 3 * h);
        r |= p & (pos >> h);
        p = (pos >> h) & (pos >> 2 * h);
        r |= p & (pos << h);
        r |= p & (pos >> 3 * h);

        // diagonal 2
        p = (pos << (h + 2)) & (pos << 2 * (h + 2));
        r |= p & (pos << 3 * (h + 2));
        r |= p & (pos >> (h + 2));
        p = (pos >> (h + 2)) & (pos >> 2 * (h + 2));
        r |= p & (pos << (h + 2));
        r |= p & (pos >> 3 * (h + 2));

        return r & (board_mask ^ mask);
    }

    // 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0
    // 1 1 1 1 1 1 1
    long long bottomMask = 4432676798593;

    // 0 0 0 0 0 0 0
    // 1 1 1 1 1 1 1
    // 1 1 1 1 1 1 1
    // 1 1 1 1 1 1 1
    // 1 1 1 1 1 1 1
    // 1 1 1 1 1 1 1
    // 1 1 1 1 1 1 1
    long long board_mask = 279258638311359;

    long long key()
    {
        return currPos + mask;
    }

    void printBoard(int i)
    {
        for (int x = HEIGHT - 1; x >= 0; x--)
        {
            for (int y = 0; y < WIDTH; y++)
            {
                if (i % 2)
                {
                    if ((currPos >> (x + y * (HEIGHT + 1))) & 1)
                    {
                        cout << "x";
                    }
                    else if ((mask >> (x + y * (HEIGHT + 1))) & 1)
                    {
                        cout << "o";
                    }
                    else
                    {
                        cout << ".";
                    }
                }
                else
                {
                    if ((currPos >> (x + y * (HEIGHT + 1))) & 1)
                    {
                        cout << "o";
                    }
                    else if ((mask >> (x + y * (HEIGHT + 1))) & 1)
                    {
                        cout << "x";
                    }
                    else
                    {
                        cout << ".";
                    }
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    // 0-based index of a playable column.
    void playMove(int col)
    {
        currPos ^= mask;
        mask |= mask + bottom_mask(col);
        moves++;
    }

    // 1-based index
    int playSeq(string seq)
    {
        for (int i = 0; i < seq.size(); i++)
        {
            int col = seq[i] - '1';
            // invalid move
            if (col < 0 || col >= WIDTH || !canPlay(col) || isWinningMove(col))
                return i;

            playMove(col);
        }
        return seq.size();
    }

    bool isWinningMove(int col)
    {
        return winning_position() & possible() & column_mask(col);
    }
};

#endif