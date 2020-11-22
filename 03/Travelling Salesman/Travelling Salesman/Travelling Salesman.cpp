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
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

typedef pair<int, int> pii;
typedef vector<vector<pii>> vvp;
typedef vector<vector<int>> vvi;
typedef set<pair<double, vector<int>>> spvp;

const int N = 100;
const int SIZE = 100;
const int GEN_SIZE = 100;
const int SELECTION_SIZE = GEN_SIZE / 20;
const int MUTATE_CHANCE = 20;
const int GENERATIONS = 3000;
const int MUTATE_SWAPS = 1;

template <class T, class K>
void printPair(const pair<T, K>& p) {
    // T and K need to have overloaded operator<<
    cout << p.first << "," << p.second << ",";
    // cerr << p.first << "," << p.second << ",";
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

        // if(N < 20){
            printPoints();
        // }
        cout << endl;
    }

    void generateFirstGen() {
        vector<int> temp;
        temp.resize(N);

        for (int i = 0; i < N; i++) {
            temp[i] = i;
        }

        //vvp gen;
        spvp s;

        for (int i = 0; i < GEN_SIZE; i++) {
            random_shuffle(temp.begin(), temp.end());
            //gen.push_back(temp);
            s.insert({getTotalDistance(temp), temp});
        }

        evolve(s);
    }

    void findMST() {
        vector<Edge> v;
        vector<int> tree_id(N);
        double cost = 0;
        for (int i = 0; i < N; i++) {
            tree_id[i] = i;
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                Edge e;
                if (i != j) {
                    e.u = i;
                    e.v = j;
                    e.dist = memo[{points[i], points[j]}];
                    v.push_back(e);
                }
            }
        }

        sort(v.begin(), v.end());

        for (Edge e : v) {
            if (tree_id[e.u] != tree_id[e.v]) {
                cost += e.dist;

                int old_id = tree_id[e.u], new_id = tree_id[e.v];
                for (int i = 0; i < N; i++) {
                    if (tree_id[i] == old_id)
                        tree_id[i] = new_id;
                }
            }
        }
        cerr << "Length of MST is: " << cost << endl;
    }

private:
    vector<pii> points;
    map<pair<pii, pii>, double> memo;

    struct Edge {
        int u, v;
        double dist;
        bool operator<(const Edge& other) {
            return dist < other.dist;
        }
    };

    void evolve(spvp& generation) {
        int currentGeneration = 1;

        while (currentGeneration < GENERATIONS) {
            cerr << currentGeneration << ": ";
            select(generation);
            auto it = generation.begin();
            printPoints((*it).second);
            currentGeneration++;
        }

        auto it = generation.begin();
        if(N < 20){
            printPoints((*it).second);
        }

    }

    void select(spvp& generation) {
        vvi selected;
        spvp newGeneration;
        auto it = generation.begin();
        int cnt = 0;
        // take the best SELECTION_SIZE
        while (cnt < SELECTION_SIZE and it != generation.end()) {
            selected.push_back((*it).second);
            newGeneration.insert(*it);
            it++;
            cnt++;
        }

        it = generation.begin();
        // take SELECTION_SIZE from the rest at random
        for (int i = 0; i < SELECTION_SIZE; i++) {
            int idx = getRandomNumber(GEN_SIZE - SELECTION_SIZE - 1, SELECTION_SIZE);
            advance(it, idx);
            selected.push_back((*it).second);
            newGeneration.insert(*it);
            advance(it, idx * (-1));
        }

        crossover(selected, newGeneration);

        it = newGeneration.begin();

        while (it != newGeneration.end()) {
            generation.insert(*it);
            it++;
        }

        it = generation.begin();
        advance(it, GEN_SIZE);
        generation.erase(it, generation.end());
        it = generation.begin();
        cerr << (*it).first << endl;


    }

    void crossover(const vvi& selected, spvp& newGen) {

        while (newGen.size() < GEN_SIZE) {
            int parentIdx1 = getRandomNumber(selected.size());
            int parentIdx2 = -1;
            do {
                parentIdx2 = getRandomNumber(selected.size());
            } while (parentIdx1 == parentIdx2);

            int crosspoint = getRandomNumber(N);
            vector<int> child1, child2;
            generateChild(selected, child1, crosspoint, parentIdx1, parentIdx2);
            generateChild(selected, child2, crosspoint, parentIdx2, parentIdx1);

            newGen.insert({ getTotalDistance(child1), child1 });
            newGen.insert({ getTotalDistance(child2), child2 });
        }

    }

    void generateChild(const vvi& selected, vector<int>& child, int crosspoint, int parentIdx1, int parentIdx2){
        for (int i = 0; i < crosspoint; i++) {
                child.push_back(selected[parentIdx1][i]);
            }

            for (int i = crosspoint; i < N; i++) {
                child.push_back(selected[parentIdx2][i]);
            }

            // printPoints(child);
            if (not isValidChild(child)) {
                makeValid(child);
            }

            int mutation = getRandomNumber(100);
            if (mutation < MUTATE_CHANCE) {
                mutate(child);
            }
    }

    void mutate(vector<int>& child) {
        for(int i = 0; i < MUTATE_SWAPS; i++){
            int idx1 = getRandomNumber(child.size());
            int idx2 = -1;
            do {
                idx2 = getRandomNumber(child.size());
            } while (idx1 == idx2);

            swap(child[idx1], child[idx2]);
        }
    }

    bool isValidChild(const vector<int>& child) {
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                if (child[i] == child[j]) {
                    return false;
                }
            }
        }
        return true;
    }

    void makeValid(vector<int>& child) {
        int hist[N + 1] = {};

        for (int i = 0; i < child.size(); i++) {
            hist[child[i]]++;
        }
        vector<int> missing;
        vector<int> duplicate;

        for (int i = 0; i < N; i++) {
            if (hist[i] == 0) {
                missing.push_back(i);
            }
            if (hist[i] > 1) {
                duplicate.push_back(i);
            }
        }

        for (int i = 0; i < missing.size(); i++) {
            for (int j = 0; j < child.size(); j++) {
                if (child[j] == duplicate[i]) {
                    child[j] = missing[i];
                    break;
                }
            }
        }

        // printPoints(child);
    }

    double getTotalDistance(const vector<int>& v) {
        double res = 0;
        for (int i = 0; i < v.size(); i++) {
            res += memo[{points[v[i]], points[v[(i + 1) % v.size()]]}];
        }
        return res;
    }

    double getDistance(pii p1, pii p2) {
        return sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
    }

    void printPoints(const vector<int>& idxs) {
        for (int i : idxs) {
            printPair(points[i]);
        }
        cout << endl;
    }

    void printPoints() {
        for (pii p : points) {
            printPair(p);
        }
        cout << endl;
    }
};



int main()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    string date_time = oss.str();

    string filename = "output/output_" + date_time + ".txt";
    string vis = "output/output_vis1.txt";
    freopen(filename.c_str(), "w", stderr);
    freopen(vis.c_str(), "w", stdout);

    cerr << "Points: " << N <<  endl;
    cerr << "Generations size: " << GEN_SIZE << endl;
    cerr << "Selection size: " << SELECTION_SIZE * 2 <<  endl;
    cerr << "Mutation percent: " << MUTATE_CHANCE << endl;
    cerr << "Mutate swaps: " << MUTATE_SWAPS << endl;
    cerr << "Plane size: " << SIZE << "x" << SIZE << endl;
    srand(time(nullptr));

    Solution s;
    s.initPoints();
    s.findMST();
    s.generateFirstGen();


}

