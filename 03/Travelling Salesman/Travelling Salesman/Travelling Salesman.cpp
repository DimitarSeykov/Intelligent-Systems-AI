#include <iostream>
#include <vector>
#include <utility>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <set>
#include <map>
#include <tuple>


using namespace std;

typedef pair<int, int> pii;
typedef vector<vector<pii>> vvp;
typedef set<pair<double, vector<pii>>> spvp;

const int N = 10;
const int SIZE = 20;
const int GEN_SIZE = 50;
const int SELECTION_SIZE = GEN_SIZE / 10;

template <class T, class K>
void printPair(const pair<T, K>& p) {
    // T and K need to have overloaded operator<<
    cout << "(" << p.first << ", " << p.second << "), ";
}

void printPoints(const vector<pii>& points) {
    for (pii p : points) {
        printPair(p);
    }
    cout << endl;
}

int getRandomNumber(int ub, int lb = 0) {
    return rand() % ub + lb;
}

class Solution {
public:

    void initPoints() {
        for (int i = 0; i < N; i++) {
            int x = getRandomNumber(SIZE),
                y = getRandomNumber(SIZE);
            points.push_back({x, y});
        }

        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < points.size(); j++) {
                if (i != j) {
                    memo[{ points[i], points[j]}] = getDistance(points[i], points[j]);
                }
            }
        }

        printPoints(points);
        cout << endl;
    }

    void generateFirstGen() {
        vector<pii> temp = points;

        //vvp gen;
        spvp s;

        for (int i = 0; i < GEN_SIZE; i++) {
            random_shuffle(temp.begin(), temp.end());
            //gen.push_back(temp);
            s.insert({getTotalDistance(temp), temp});
        }

        evolve(s);
        /*
        for (auto el : gen) {
            printPoints(el);
            cout << getTotalDistance(el) << endl;
        }
        cout << endl;

        auto it = s.begin();
        while (it != s.end()) {
            printPoints((*it).second);
            cout << (*it).first << endl;
            it++;
        }
        */
    }

    
    void evolve(const spvp& generation) {
        int currentGeneration = 1;

        select(generation);

    }

    void select(const spvp& generation) {
        vvp selected;
        auto it = generation.begin();
        int cnt = 0;
        // take the best SELECTION_SIZE
        while (cnt < SELECTION_SIZE and it != generation.end()) {
            selected.push_back((*it).second);
            it++;
            cnt++;
        }

        spvp newGeneration;
        it = generation.begin();
        // take SELECTION_SIZE from the rest at random
        for (int i = 0; i < SELECTION_SIZE; i++) {
            int idx = getRandomNumber(GEN_SIZE - SELECTION_SIZE - 1, SELECTION_SIZE);
            advance(it, idx);
            selected.push_back((*it).second);
            newGeneration.insert(*it);
            advance(it, idx * (-1));
        }

        for (auto el : selected) {
            printPoints(el);
            cout << getTotalDistance(el) << endl;
        }
        cout << endl;
    }

    vector<pii> points;
    map<pair<pii, pii>, double> memo;

private:
    double getTotalDistance(const vector<pii>& v) {
        double res = 0;
        for (int i = 0; i < v.size(); i++) {
            res += memo[{v[i], v[(i + 1) % v.size()]}];
        }
        return res;
    }

    double getDistance(pii p1, pii p2) {
        return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
    }
};



int main()
{
    srand(time(nullptr));

    Solution s;
    s.initPoints();
    s.generateFirstGen();

    
}

