#include <iostream>
#include <time.h>
#include <cstdlib>
#include <vector>
#include <limits.h>
#include <utility>
#include <chrono>


using namespace std;

const int BOARD_SIDE = 10000;

typedef vector<vector<int>> vvi;
typedef pair<int, int> pii;

const int MOVES_FOR_RESTART = 70;

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
                int conflicts = getConflictsAtPosition(i, j);
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

            int idx = rand() % (candidates.size());
            int pos = candidates[idx];

            queens[i] = pos;
            board[i][pos] = 1;

            updateConflicts(i, pos, 1);
        }
        //cout << "initial conflicts: " << getTotalConflicts() << endl;
        //cout << "initial board: " << endl;
        //printMatrix(board);

        // printVector(queens);
        // printVector(conflictsColumns);
        // printVector(conflictsMainDiag);
        // printVector(conflictsSecDiag);
    }

    int getTotalConflicts() {
        return getConflictFromDirecton(conflictsMainDiag) +
               getConflictFromDirecton(conflictsSecDiag) +
               getConflictFromDirecton(conflictsColumns);
    }

    void makeMove() {
        int maxConflicts = 0;
        vector<int> candidates;
        for (int i = 0; i < queens.size(); i++) {
            int currentConflicts = getConflictsAtPosition(i, queens[i]);
            if (currentConflicts > maxConflicts) {
                maxConflicts = currentConflicts;
            }
        }

        for(int i = 0; i < queens.size(); i++){
            int currentConflicts = getConflictsAtPosition(i, queens[i]);
            if(currentConflicts == maxConflicts){
                candidates.push_back(i);
            }
        }

        int worstQueenIdx = candidates[rand() % candidates.size()];

        int minConflicts = INT_MAX;
        vector<int> moveCandidates;
        for (int i = 0; i < BOARD_SIDE; i++) {
            int currentConflicts = getConflictsAtPosition(worstQueenIdx, i);
            if (currentConflicts < minConflicts) {
                minConflicts = currentConflicts;
            }
        }

        for (int i = 0; i < BOARD_SIDE; i++) {
            int currentConflicts = getConflictsAtPosition(worstQueenIdx, i);
            if (currentConflicts == minConflicts) {
                moveCandidates.push_back(i);
            }
        }


        int movePos = moveCandidates[rand() % moveCandidates.size()];
        // cout << "candidates queens: " << candidates.size() << endl;
        // cout << "worstQueen: " << worstQueenIdx << endl;
        // cout << "moving candidates: " << moveCandidates.size() << endl;
        // cout << "move position: " << movePos << endl;

        updateConflicts(worstQueenIdx, queens[worstQueenIdx], -1);
        updateConflicts(worstQueenIdx, movePos, 1);

        swap(board[worstQueenIdx][queens[worstQueenIdx]], board[worstQueenIdx][movePos]);
        queens[worstQueenIdx] = movePos;

        // cout << "conflicts after move: " << getTotalConflicts() << endl;
        // printVector(queens);
        // printVector(conflictsColumns);
        // printVector(conflictsMainDiag);
        // printVector(conflictsSecDiag);
        // cout << endl;
        // printMatrix(board);
        // cout << endl;
    }

    void solveRandomRestart(){
        int moves = 0;
        int restarts = 0;
        while (getTotalConflicts() > 0) {
            if (moves > MOVES_FOR_RESTART) {
                generateInitialBoard();
                restarts++;
                //cout << moves <<  endl;
                // cout << "conflicts: " << getTotalConflicts() << endl;
                moves = 0;
                continue;
            }
            // cout << "moves " << moves << endl;
            makeMove();
            //printMatrix(game.board);
            moves++;
        }
        cout << "moves: " << moves << endl;
        cout << "restarts: " << restarts << endl;
        if(BOARD_SIDE <= 15){
            printMatrix(board);
        }
    }


private:
    vvi board;
    vector<int> queens;
    vector<int> conflictsMainDiag;
    vector<int> conflictsSecDiag;
    vector<int> conflictsColumns;

    int getMainDiag(int x, int y) {
        return (BOARD_SIDE - 1 - x + y);
    }

    int getSecDiag(int x, int y) {
        return x + y;
    }

    int getConflictsAtPosition(int x, int y) {
        return conflictsColumns[y] +
               conflictsMainDiag[getMainDiag(x, y)] +
               conflictsSecDiag[getSecDiag(x, y)] - 3;
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

    int getConflictFromDirecton(vector<int>& v) {
        int sum = 0;
        for (int conf : v) {
            if (conf > 1) {
                sum += (conf - 1);
            }
        }

        return sum;
    }

    void updateConflicts(int x, int y, int factor) {
        conflictsColumns[y] += factor;
        conflictsMainDiag[getMainDiag(x, y)] += factor;
        conflictsSecDiag[getSecDiag(x, y)] += factor;
    }
};

int main()
{
    srand(time(nullptr));

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    // freopen("output.txt", "w", stdout);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    Game game;
    game.generateInitialBoard();
    game.solveRandomRestart();

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;
}

