#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <set>
#include <queue>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <cstdlib>


using namespace std;

typedef pair<int, int> pii;


vector<vector<vector<int>>> TARGET_STATES;
const int BOARD_SIDE = 3;

void printMatrix(int mat[][BOARD_SIDE], const int SIZE){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

void printMatrix(const vector<vector<int>>& mat, const int SIZE){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

template <class T>
void printVector(vector<T>& v){
    for(T el : v){
        cout << el << " ";
    }
    cout << endl;
}

int globalID = 0;

struct State {

    State(int other[][BOARD_SIDE]){
        mat.resize(BOARD_SIDE);
        for (int i = 0; i < BOARD_SIDE; i++) {
            mat[i].resize(BOARD_SIDE);
            for (int j = 0; j < BOARD_SIDE; j++) {
                mat[i][j] = other[i][j];
                if (other[i][j] == 0) {
                    zeroAt = { i, j };
                }
            }
        }
        this->heuristicDistance = getManhattanDistance();
        ID = globalID++;
    }

    State(vector<vector<int>> other){
        mat.resize(BOARD_SIDE);
        for(int i = 0; i < BOARD_SIDE; i++){
            mat[i].resize(BOARD_SIDE);
            for(int j = 0; j < BOARD_SIDE; j++){
                mat[i][j] = other[i][j];
                if(other[i][j] == 0){
                    zeroAt = {i, j};
                }
            }
        }
        this->heuristicDistance = getManhattanDistance();
        ID = globalID++;
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

    bool operator<(const State& other)const {
        return this->heuristicDistance > other.heuristicDistance;
    }

    void solveAStar();

    vector<vector<int>> mat;
    pii zeroAt = {0, 0};
    vector<int> sequence;
    int heuristicDistance = 0;
    int ID = -1;
};


set<vector<vector<int>>> used;


bool isValidPos(int x, int y){
    if(x < 0 or y < 0){
        return false;
    }
    if(x >= BOARD_SIDE or y >= BOARD_SIDE){
        return false;
    }

    return true;
}

State generateNewState(State oldState, pii swapPos){
    State newState(oldState.mat);
    newState.sequence = oldState.sequence;
    newState.sequence.push_back(newState.mat[swapPos.first][swapPos.second]);
    pii zeroAt = oldState.zeroAt;
    swap(newState.mat[zeroAt.first][zeroAt.second], newState.mat[swapPos.first][swapPos.second]);
    newState.zeroAt = swapPos;
    newState.heuristicDistance = newState.getManhattanDistance() + newState.sequence.size();

    return newState;
}

void pushNewState(priority_queue<State>& pq, State newState){
    cout << "new id: " << newState.ID << endl;
    printMatrix(newState.mat, BOARD_SIDE);
    cout << "in set: " << used.count(newState.mat) << "\n\n";
    cout << "dist: " << newState.getManhattanDistance() + newState.sequence.size() << endl;
    if(!used.count(newState.mat)){
        pq.push(newState);
        used.insert(newState.mat);
    }
    cout << "pq size: " <<  pq.size() <<  endl;
}

void State::solveAStar(){
    priority_queue<State> pq;
    pq.push(*this);
    used.insert((*this).mat);
    printMatrix((*this).mat, BOARD_SIDE);
    cout << endl;

    while(!pq.empty()){
        State t = pq.top();

        if(t.getManhattanDistance() == 0){
            printMatrix(t.mat, BOARD_SIDE);
            printVector(t.sequence);
            break;
        }

        cout << "ID: " << t.ID << endl;
        printMatrix(t.mat, BOARD_SIDE);
        int i = t.zeroAt.first,
            j = t.zeroAt.second;
        // cout << i << " " << j << endl;
        pq.pop();
        cout << "---------considered-----------\n\n";

        if(isValidPos(i - 1, j)){
            pushNewState(pq, generateNewState(t, {i - 1, j}));
        }

        if(isValidPos(i + 1, j)){
            pushNewState(pq, generateNewState(t, {i + 1, j}));
        }

        if(isValidPos(i, j - 1)){
            pushNewState(pq, generateNewState(t, {i, j - 1}));
        }

        if(isValidPos(i, j + 1)){
            pushNewState(pq, generateNewState(t, {i, j + 1}));
        }
        cout << "-----------------------------\n\n" << endl;
        
        //cout << "size ater pop: " << pq.size() << endl;
        //cout << "-----------------------------\n\n" << endl;

    }

}

void initTargetStates(){
    TARGET_STATES.resize(5);
    int cnt = 1;
    for(int i = 0; i < 3; i++){
        vector<int> temp;
        for(int j = 0; j < 3; j++){
            temp.push_back((cnt++) % 9);
        }
        TARGET_STATES[3].push_back(temp);
    }

    /*for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << TARGET_STATES[3][i][j] << " ";
        }
        cout << endl;
    }*/

    cnt = 1;
    for (int i = 0; i < 4; i++) {
        vector<int> temp;
        for (int j = 0; j < 4; j++) {
            temp.push_back((cnt++) % 16);
        }
        TARGET_STATES[4].push_back(temp);
    }

    
    /*for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << TARGET_STATES[4][i][j] << " ";
        }
        cout << endl;
    }*/

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    initTargetStates();
    int test3[3][3] = {{0, 1, 3}, {4, 2, 5}, {7, 8, 6}};
    int test3_simple[3][3] = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};
    int test3_hard[3][3] = {{1, 8, 2}, {0, 4, 3}, {7, 6, 5}};
    int test3_harder[3][3] = {{0, 3, 4}, {7, 5, 8}, {6, 1, 2}}; // unsolvable
    int test3_hardestPossible[3][3] = {{8, 6, 7}, {2, 5, 4}, {3, 0, 1}};
    // int test4[4][4] =
    State a(test3_hardestPossible);
    a.solveAStar();
    //cout << a.getManhattanDistance() << endl;


}
