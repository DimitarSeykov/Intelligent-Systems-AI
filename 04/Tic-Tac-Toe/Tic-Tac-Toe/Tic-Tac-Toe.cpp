#include <iostream>
#include <vector>
#include <utility>
#include <limits.h>
#include <algorithm>

using namespace std;

typedef vector<char> vc;
typedef vector<vector<char>> vvc;

const int N = 3;
const char EMPTY = '-';
const char X = 'X';
const char O = 'O';
enum WinType {
    none = -1,
    tie,
    maximizer,
    minimizer,

};

class Game {
public:
    Game() {
        board.resize(N);
        for (int i = 0; i < N; i++) {
            board[i].resize(3);
            for (int j = 0; j < N; j++) {
                board[i][j] = '-';
            }
        }

    }

    void play() {
        printBoard();
        while (isFinished().first == none) {
            userInputPosition();
            if (isFinished().first == none) {
                makeMove();
            }
            printBoard();
        }
    }

    void makeMove() {
        int bestPossible = INT_MAX;
        int row = -1, col = -1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = O;
                    int score = minMax(board, true, INT_MIN, INT_MAX);
                    board[i][j] = EMPTY;
                    if (score < bestPossible) {
                        bestPossible = score;
                        row = i;
                        col = j;
                    }
                }
            }
        }
        board[row][col] = O;
    }

    int minMax(vvc& currentBoard, bool isMaximiser, int alpha, int beta) {
        Game g;
        g.board = currentBoard;
        auto finished = g.isFinished();
        if (finished.first != none) {
            return finished.second;
        }

        if (isMaximiser) {
            int bestPossible = INT_MIN;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (currentBoard[i][j] == EMPTY) {
                        currentBoard[i][j] = X;
                        int score = minMax(currentBoard, false, alpha, beta);
                        currentBoard[i][j] = EMPTY;
                        bestPossible = max(score, bestPossible);
                        alpha = max(alpha, bestPossible);
                        if (alpha >= beta) {
                            return bestPossible;
                        }
                    }
                }
            }
            return bestPossible;
        } else {
            int bestPossible = INT_MAX;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (currentBoard[i][j] == EMPTY) {
                        currentBoard[i][j] = O;
                        int score = minMax(currentBoard, true, alpha, beta);
                        currentBoard[i][j] = EMPTY;
                        bestPossible = min(score, bestPossible);
                        beta = min(beta, bestPossible);
                        if (alpha >= beta) {
                            return bestPossible;
                        }
                    }
                }
            }
            return bestPossible;
        }
    }

    void printBoard() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    pair<WinType, int> isFinished() {
        int row = isFinishedRows(),
            col = isFinishedColumns(),
            diag = isFinishedDiag();

        int empty = findNumberOfEmptySpaces();

        WinType winner = none;

        if (row != -1) {
            winner = (board[row][0] == X) ? maximizer : minimizer;
        } else if (col != -1) {
            winner = (board[0][col] == X) ? maximizer : minimizer;
        } else if (diag != -1) {
            if (diag == 0) {
                winner = (board[0][0] == X) ? maximizer : minimizer;
            } else {
                winner = (board[0][N - 1] == X) ? maximizer : minimizer;
            }
        } else if (empty == 0) {
            winner = tie;
        }

        empty++;

        if (winner == minimizer) {
            empty *= (-1);
        }
        else if (winner == tie) {
            empty = 0;
        }

        return { winner, empty };
    }

    vvc board;
    int isFinishedRows() {
        for (int i = 0; i < N; i++) {
            int cnt = 1;
            for (int j = 1; j < N; j++) {
                if (board[i][j] == board[i][j - 1] and board[i][j] != EMPTY) {
                    cnt++;
                }
            }
            if (cnt == N) {
                return i;
            }
        }
        return -1;
    }

    int isFinishedColumns() {
        for (int i = 0; i < N; i++) {
            int cnt = 1;
            for (int j = 1; j < N; j++) {
                if (board[j][i] == board[j - 1][i] and board[j][i] != EMPTY) {
                    cnt++;
                }
            }
            if (cnt == N) {
                return i;
            }
        }
        return -1;
    }

    int isFinishedDiag() {
        int cntMain = 1;
        int cntSec = 1;
        for (int i = 1; i < N; i++) {
            if (board[i][i] == board[i - 1][i - 1] and board[i][i] != EMPTY) {
                cntMain++;
            }
            int sec = N - i - 1;
            if (board[i][sec] == board[i - 1][sec + 1] and board[i][sec] != EMPTY) {
                cntSec++;
            }
        }

        if (cntMain == N) {
            return 0;
        }
        else if (cntSec == N) {
            return 1;
        }

        return -1;
    }

    int findNumberOfEmptySpaces() {
        int res = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] == EMPTY) {
                    res++;
                }
            }
        }

        return res;
    }

    void userInputPosition() {
        int i = -1,
            j = -1;

        do {
            cin >> i >> j;
        } while (board[i][j] != EMPTY and i < N and j < N);
        board[i][j] = X;
    }
};

int main()
{
    Game g;
    g.play();
    
}
