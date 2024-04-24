#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int WIDTH = 7;
int HEIGHT = 6;
int MIN_SCORE = -(WIDTH * HEIGHT) / 2 + 3;
int MAX_SCORE = (WIDTH * HEIGHT + 1) / 2 - 3;

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
		board[col][height[col]] = 1 + moves % 2;
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
		int cur = 1 + moves % 2;

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

	// 3 cases - horizontal, vertical, diagonal
	bool isWinningMove(int col)
	{
		long long pos = currPos;
		pos |= (mask + bottom_mask(col)) & column_mask(col);

		// vertical;
		long long m = pos & (pos >> 1);
		// 1,2 ; 2,3 ; 3,4 are obtained
		// m & m>>2 makes sure 1, 2 and 3, 4 are set
		if (m & (m >> 2))
		{
			return true;
		}

		// horizontal
		m = pos & (pos >> (HEIGHT + 1));
		if (m & (m >> (2 * (HEIGHT + 1))))
		{
			return true;
		}

		// diagonal 1
		m = pos & (pos >> HEIGHT);
		if (m & (m >> (2 * HEIGHT)))
		{
			return true;
		}

		// diagonal 2
		m = pos & (pos >> (HEIGHT + 2));
		if (m & (m >> (2 * (HEIGHT + 2))))
		{
			return true;
		}

		return false;
	}
};

#endif