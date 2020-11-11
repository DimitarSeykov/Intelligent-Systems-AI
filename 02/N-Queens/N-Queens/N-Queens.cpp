#include <iostream>
#include <time.h>
#include <cstdlib>
#include <vector>
#include <limits>

using namespace std;

const int BOARD_SIDE = 5;

typedef vector<vector<int>> vvi;

template <class T>
void printVector(vector<T>& v) {
    for (T el : v) {
        cout << el << " ";
    }
    cout << endl;
}

template <class T>
void printMatrix(vector<vector<T>>& v) {
    for (auto row : v) {
        for (T el : row) {
            cout << el << " ";
        }
        cout << endl;
    }
    cout << endl;
}

class Game {
public:
    void generateInitialBoard() {
        
        initSizes();

        for (int i = 0; i < BOARD_SIDE; i++) {

            vector<int> candidates;
            vector<int> conflitsCurrentRow(BOARD_SIDE);
            int minConflicts = INT_MAX;
            for (int j = 0; j < BOARD_SIDE; j++) {
                int conflicts = conflictsColumns[j] +
                                conflictsMainDiag[getMainDiag(i, j)] +
                                conflictsSecDiag[getSecDiag(i, j)];
                conflitsCurrentRow[j] = conflicts;
                if (conflicts < minConflicts) {
                    minConflicts = conflicts;
                }
            }

            for (int j = 0; j < conflitsCurrentRow.size(); j++) {
                if (conflitsCurrentRow[j] == minConflicts) {
                    candidates.push_back(j);
                }
            }

            int idx = rand() % (candidates.size()) + 0;
            int pos = candidates[idx];

            queens[i] = pos;
            board[i][pos] = 1;
            
            conflictsColumns[pos]++;
            conflictsMainDiag[getMainDiag(i, pos)]++;
            conflictsSecDiag[getSecDiag(i, pos)]++;
            //printMatrix(board);
        }

        // printVector(queens);
        // printVector(conflictsColumns);
        // printVector(conflictsMainDiag);
        // printVector(conflictsSecDiag);
    }

    int getConflicts() {

    }
    
    vvi board;
    vector<int> queens;
    vector<int> conflictsMainDiag;
    vector<int> conflictsSecDiag;
    vector<int> conflictsColumns;

private:

    int getMainDiag(int x, int y) {
        return (BOARD_SIDE - 1 - x + y);
    }

    int getSecDiag(int x, int y) {
        return x + y;
    }

    void initSizes() {
        board.resize(BOARD_SIDE);
        for (int i = 0; i < BOARD_SIDE; i++) {
            board[i].assign(BOARD_SIDE, 0);
        }

        queens.assign(BOARD_SIDE, 0);
        conflictsColumns.assign(BOARD_SIDE, 0);
        conflictsMainDiag.assign(2 * BOARD_SIDE - 1, 0);
        conflictsSecDiag.assign(2 * BOARD_SIDE - 1, 0);
    }
};

int main()
{
    srand(time(nullptr));
    
    Game game;
    game.generateInitialBoard();
    printMatrix(game.board);
}

