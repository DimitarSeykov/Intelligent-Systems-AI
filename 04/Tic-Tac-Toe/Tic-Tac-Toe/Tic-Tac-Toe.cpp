#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>

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

int getRandomNumber(int ub, int lb = 0) {
    return rand() % ub + lb;
}

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
        cout << isFinished().first << " " << isFinished().second << endl;
    }

    void makeMove() {
        int i = -1, j = -1;
        do {
            i = getRandomNumber(N), 
            j = getRandomNumber(N);
        } while (board[i][j] != EMPTY);
        board[i][j] = O;
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


    void userInput() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cin >> board[i][j];
            }
        }
        cout << endl;
    }

    void userInputPosition() {
        int i = -1,
            j = -1;

        do {
            cin >> i >> j;
        } while (board[i][j] != EMPTY);
        board[i][j] = X;
    }
};

int main()
{
    srand(time(nullptr));

    Game g;
    g.play();
    
}

/*
XOX
XXO
OXO

XOX
OXX
XOO

XXO
-OX
O-X

-OX
-OX
XO-

-OX
--X
OOX

-OX
---
OOX
*/
