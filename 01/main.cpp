#include <bits/stdc++.h>

using namespace std;

#define pii pair<int, int>

vector<int**> TARGET_STATES;
const int BOARD_SIDE = 3;

void printMatrix(int** mat, const int SIZE){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

struct State {
    State(){
        mat = new int*[BOARD_SIDE];
        for(int i = 0; i < BOARD_SIDE; i++){
            mat[i] = new int[BOARD_SIDE];
            for(int j = 0; j < BOARD_SIDE; j++){
                mat[i][j] = 0;
            }
        }

    }

    State(int other[][BOARD_SIDE]){
        mat = new int*[BOARD_SIDE];
        for(int i = 0; i < BOARD_SIDE; i++){
            mat[i] = new int[BOARD_SIDE];
            for(int j = 0; j < BOARD_SIDE; j++){
                mat[i][j] = other[i][j];
                if(other[i][j] == 0){
                    zeroAt = {i, j};
                }
            }
        }
    }

    int getManhattanDistance(){
        int result = 0;
        for(int i = 0; i < BOARD_SIDE * BOARD_SIDE; i++){
            pii targetIndexes;
            pii stateIndexes;
            for(int j = 0; j < BOARD_SIDE; j++){
                for(int k = 0; k < BOARD_SIDE; k++){
                    if(mat[j][k] == i){
                        stateIndexes = {j, k};
                    }
                }
            }
            for(int j = 0; j < BOARD_SIDE; j++){
                for(int k = 0; k < BOARD_SIDE; k++){
                    if(TARGET_STATES[BOARD_SIDE][j][k] == i){
                        targetIndexes = {j, k};
                    }
                }
            }
            int current = abs(targetIndexes.first - stateIndexes.first) + abs(targetIndexes.second - stateIndexes.second);
            result += current;
        }
        return result;
    }

    int** mat;
    pii zeroAt = {0, 0};
    int moves = 0;
};

void initTargetStates(){
    TARGET_STATES.reserve(5);
    TARGET_STATES[3] = new int*[3];
    int cnt = 1;
    for(int i = 0; i < 3; i++){
        TARGET_STATES[3][i] = new int[3];
        for(int j = 0; j < 3; j++){
            TARGET_STATES[3][i][j] = (cnt++) % 9;
        }
    }

    /*for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << TARGET_STATES[3][i][j] << " ";
        }
        cout << endl;
    }*/

    TARGET_STATES[4] = new int*[4];
    cnt = 1;
    for(int i = 0; i < 4; i++){
        TARGET_STATES[4][i] = new int[3];
        for(int j = 0; j < 4; j++){
            TARGET_STATES[4][i][j] = (cnt++) % 16;
        }
    }

    /*
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << TARGET_STATES[4][i][j] << " ";
        }
        cout << endl;
    }*/

}

int main(){
    initTargetStates();
    int test3[3][3] = {{0, 1, 3}, {4, 2, 5}, {7, 8, 6}};
    int test3_simple[3][3] = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};
    State a(test3);
    cout << a.getManhattanDistance() << endl;


}
