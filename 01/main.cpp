#include <bits/stdc++.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <cstdlib>


using namespace std;

typedef pair<int, int> pii;


vector<int**> TARGET_STATES;
const int BOARD_SIDE = 3;

void printMatrix(int mat[][BOARD_SIDE], const int SIZE){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

template <class T>
void printVector(vector<T>& v){
    for(T el : v){
        cout << el << " ";
    }
    cout << endl;
}

struct State {
    State(){
        for(int i = 0; i < BOARD_SIDE; i++){
            for(int j = 0; j < BOARD_SIDE; j++){
                mat[i][j] = 0;
            }
        }

    }

    State(int other[][BOARD_SIDE]){
        for(int i = 0; i < BOARD_SIDE; i++){
            for(int j = 0; j < BOARD_SIDE; j++){
                mat[i][j] = other[i][j];
                if(other[i][j] == 0){
                    zeroAt = {i, j};
                }
            }
        }
    }

    int getManhattanDistance()const{
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



    /*bool operator==(const State& other)const{
        cout << "==" << endl;
        for(int i = 0; i < BOARD_SIDE; i++){
            for(int j = 0; j < BOARD_SIDE; j++){
                if(this->mat[i][j] != other.mat[i][j]){
                    return false;
                }
            }
        }
        return true;
    }*/

    void solveAStar();

    int mat[BOARD_SIDE][BOARD_SIDE];
    pii zeroAt = {0, 0};
    vector<int> sequence;
};


struct Comp{
    bool operator()(State s1, State s2)const {
        return (s1.sequence.size() + s1.getManhattanDistance()) < (s2.sequence.size() + s2.getManhattanDistance());
    }
};

set<State> used;

bool isValidPos(int mat[][BOARD_SIDE], int x, int y){
    if(x < 0 or y < 0){
        return false;
    }
    if(x >= BOARD_SIDE or y >= BOARD_SIDE){
        return false;
    }

    return true;
}

State generateNewState(State& oldState, pii swapPos){
    State newState(oldState.mat);
    newState.sequence = oldState.sequence;
    newState.sequence.push_back(newState.mat[swapPos.first][swapPos.second]);
    pii zeroAt = oldState.zeroAt;
    swap(newState.mat[zeroAt.first][zeroAt.second], newState.mat[swapPos.first][swapPos.second]);
    newState.zeroAt = swapPos;

    return newState;
}

void State::solveAStar(){
    priority_queue<State, vector<State>, Comp> pq;
    pq.push(*this);
    used.insert(*this);

    while(!pq.empty()){
        State t = pq.top();
        //cout << "M dist: " << t.getManhattanDistance() << endl;

        if(t.getManhattanDistance() == 0){
            printMatrix(t.mat, BOARD_SIDE);
            printVector(t.sequence);
            break;
        }

        printMatrix(t.mat, BOARD_SIDE);
        cout << endl;
        //printVector(t.sequence);

        int i = t.zeroAt.first,
            j = t.zeroAt.second;
        //cout << i << " " << j << endl;


        if(isValidPos(t.mat, i - 1, j)){
            State newState = generateNewState(t, {i - 1, j});
            //cout << "here1" << endl;
            //printMatrix(newState.mat, BOARD_SIDE);
            //cout << used.count(newState) << endl;
            if(!used.count(newState)){
                pq.push(newState);
                used.insert(newState);
            }
        }
        if(isValidPos(t.mat, i + 1, j)){
            State newState = generateNewState(t, {i + 1, j});
            if(!used.count(newState)){
                //cout << "here2" << endl;
                pq.push(newState);
                used.insert(newState);
            }
        }
        if(isValidPos(t.mat, i, j - 1)){
            State newState = generateNewState(t, {i, j - 1});
            //cout << "here3" << endl;
            if(!used.count(newState)){
                pq.push(newState);
                used.insert(newState);
            }
        }
        if(isValidPos(t.mat, i, j + 1)){
            State newState = generateNewState(t, {i, j + 1});
            //cout << "here4" << endl;
            if(!used.count(newState)){
                pq.push(newState);
                used.insert(newState);
            }
        }

        pq.pop();
    }

}

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
    int test3_hard[3][3] = {{1, 8, 2}, {0, 4, 3}, {7, 6, 5}};
    // int test4[4][4] =
    State a(test3_hard);
    a.solveAStar();
    //cout << a.getManhattanDistance() << endl;


}
