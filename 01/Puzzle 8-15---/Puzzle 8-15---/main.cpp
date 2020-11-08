#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <set>
#include <queue>
#include <chrono>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <cstdlib>


using namespace std;

typedef pair<int, int> pii;
typedef vector<vector<int>> vvi;


vector<vvi> TARGET_STATES;
const int BOARD_SIDE = 3;

void printMatrix(int mat[][BOARD_SIDE], const int SIZE){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

void printMatrix(const vvi& mat, const int SIZE){
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

    State(vvi other){
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

    bool solveAStar(int& threshold);
    void solveIterativeDeepening();

    vvi mat;
    pii zeroAt = {0, 0};
    vector<string> sequence;
    int heuristicDistance = 0;
    int ID = -1;
};


bool isValidPos(int x, int y){
    if(x < 0 or y < 0){
        return false;
    }
    if(x >= BOARD_SIDE or y >= BOARD_SIDE){
        return false;
    }

    return true;
}

State generateNewState(State oldState, pii swapPos, const string direction){
    State newState(oldState.mat);
    newState.sequence = oldState.sequence;
    newState.sequence.push_back(direction);
    pii zeroAt = oldState.zeroAt;
    swap(newState.mat[zeroAt.first][zeroAt.second], newState.mat[swapPos.first][swapPos.second]);
    newState.zeroAt = swapPos;
    newState.heuristicDistance = newState.getManhattanDistance() + newState.sequence.size();

    return newState;
}

void pushNewState(priority_queue<State>& pq, set<vvi>& used, int& threshold, State newState){
    //cout << "new id: " << newState.ID << endl;
    //printMatrix(newState.mat, BOARD_SIDE);
    //cout << "in set: " << used.count(newState.mat) << "\n\n";
    //cout << "dist: " << newState.getManhattanDistance() + newState.sequence.size() << endl;
    if(!used.count(newState.mat) and newState.heuristicDistance <= threshold){
        pq.push(newState);
        used.insert(newState.mat);
    }
   // cout << "pq size: " <<  pq.size() <<  endl;
}

bool State::solveAStar(int& threshold){
    priority_queue<State> pq;
    pq.push(*this);
    set<vvi> used;
    used.insert((*this).mat);
    //printMatrix((*this).mat, BOARD_SIDE);
    //cout << endl;
    bool solutionFound = false;

    while(not pq.empty()){
        State t = pq.top();

        if(t.getManhattanDistance() == 0){
            solutionFound = true;
            printMatrix(t.mat, BOARD_SIDE);
            cout << t.sequence.size() << ": ";
            printVector(t.sequence);
            cout << "ID: " << t.ID << endl;
            cout << "current PQ size: " << pq.size() << endl;
            break;
        }

        //cout << "ID: " << t.ID << endl;
        //printMatrix(t.mat, BOARD_SIDE);
        int i = t.zeroAt.first,
            j = t.zeroAt.second;
        // cout << i << " " << j << endl;
        pq.pop();
        //cout << "---------considered-----------\n\n";

        if(isValidPos(i - 1, j)){
            pushNewState(pq, used, threshold, generateNewState(t, {i - 1, j}, "down"));
        }

        if(isValidPos(i + 1, j)){
            pushNewState(pq, used, threshold, generateNewState(t, {i + 1, j}, "up"));
        }

        if(isValidPos(i, j - 1)){
            pushNewState(pq, used, threshold, generateNewState(t, {i, j - 1}, "right"));
        }

        if(isValidPos(i, j + 1)){
            pushNewState(pq, used, threshold, generateNewState(t, {i, j + 1}, "left"));
        }
        //cout << "-----------------------------\n\n" << endl;
        
        //cout << "size ater pop: " << pq.size() << endl;
        //cout << "-----------------------------\n\n" << endl;

    }

    return solutionFound;
}


void State::solveIterativeDeepening() {
    const int STEP = 5;
    bool solutionFound = false;
    int threshold = 30;
    while (not solutionFound) {
        solutionFound = solveAStar(threshold);
        threshold += STEP;
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
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int test3[3][3] = {{0, 1, 3}, {4, 2, 5}, {7, 8, 6}};
    int test3_simple[3][3] = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};
    int test3_hard[3][3] = {{1, 8, 2}, {0, 4, 3}, {7, 6, 5}};
    int test3_unsolvable[3][3] = {{0, 3, 4}, {7, 5, 8}, {6, 1, 2}}; // unsolvable
    int test3_21[3][3] = { {6, 5, 3}, {2, 4, 8}, {7, 0, 1} };
    int test3_31[3][3] = {{8, 6, 7}, {2, 5, 4}, {3, 0, 1}};
    int test4_11[4][4] = { {2, 3, 4, 8}, {1, 6, 7, 12}, {5, 10, 11, 0}, {9, 13, 14, 15} };
    int test4_40[4][4] = { {5, 6, 3, 4}, {8, 0, 1, 15}, {10, 7, 2, 11}, {12, 9, 14, 13} };
    int test4_66[4][4] = { {14, 15, 8, 12}, {10, 11, 9, 13}, {2, 6, 5, 1}, {3, 7, 4, 0} };
    State a(test3_simple);
    printMatrix(a.mat, BOARD_SIDE);
    cout << endl;

    a.solveIterativeDeepening();

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;


}
