#include <iostream>
#include <set>
#include <vector>

using namespace std;

vector<int> biggestCliqueBK;

vector<vector<int>> readMatrix(int n) {
    vector<vector<int>> matrix;
    int value;
    for (int i = 0; i < n; i++) {
        vector<int> row;
        for (int j = 0; j < n; j++) {
            cin >> value;
            row.push_back(value);
        }
        matrix.push_back(row);
    }
    return matrix;
}

vector<vector<int>> reduceAllValuesToOne(vector<vector<int>> multigraph) {
    vector<vector<int>> graph;
    for (int i = 0; i < multigraph.size(); i++) {
        vector<int> row;
        for (int j = 0; j < multigraph.size(); j++) {
            row.push_back(max(0, multigraph[i][j]));
        }
        graph.push_back(row);
    }
    return graph;
}

bool edgeExists(int u, int v, const vector<vector<int>> &G) {
    return G[u][v] > 0;
}

int findPivot(const set<int> &P, const set<int> &X, const vector<vector<int>> &adjMatrix) {
    int maxDegree = -1;
    int pivot = -1;

    auto checkDegree = [&](int vertex) {
        int degree = 0;
        for (int i = 0; i < adjMatrix[vertex].size(); ++i) {
            degree += adjMatrix[vertex][i];
        }
        if (degree > maxDegree) {
            maxDegree = degree;
            pivot = vertex;
        }
    };

    // Check each vertex in P
    for (const auto &vertex : P) {
        checkDegree(vertex);
    }

    // Check each vertex in X
    for (const auto &vertex : X) {
        checkDegree(vertex);
    }

    return pivot;
}

void BronKerbosch2(set<int> R, set<int> P, set<int> X, const vector<vector<int>> &G) {
    if (P.empty() && X.empty()) {
        if (biggestCliqueBK.size() < R.size()) {
            biggestCliqueBK.clear();
            for (auto v : R) {
                biggestCliqueBK.push_back(v);
            }
        }
        // for (auto v : R) {
        //     cout << v << ' ';
        // }
        // cout << endl;
        return;
    }

    // Choosing pivot (vertex with the most neighbours in P and X)
    int u = findPivot(P, X, G);

    // Neighbours of u
    set<int> diff;
    for (auto v : P) {
        if (!edgeExists(u, v, G)) {
            diff.insert(v);
        }
    }

    for (auto v : diff) {
        set<int> newR = R, newP, newX;
        newR.insert(v);

        for (auto w : P) {
            if (edgeExists(v, w, G)) {
                newP.insert(w);
            }
        }

        for (auto w : X) {
            if (edgeExists(v, w, G)) {
                newX.insert(w);
            }
        }

        BronKerbosch2(newR, newP, newX, G);
        P.erase(v);
        X.insert(v);
    }
}

void BronKerbosch3(const vector<vector<int>> &G) {
    set<int> P, R, X;

    for (int i = 0; i < G.size(); ++i) {
        P.insert(i);
    }

    for (int i = 0; i < G.size(); ++i) {
        set<int> neighbors;
        for (int j = 0; j < G[i].size(); ++j) {
            if (G[i][j] == 1) {
                neighbors.insert(j);
            }
        }

        BronKerbosch2({i}, neighbors, X, G);
        P.erase(i);
        X.insert(i);
    }
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> matrix = readMatrix(n);
    cout << "Matrix" << endl;
    // Finging a clique in a graph

    // (it doesn't matter wheather it is a multigraph, because we can have 2 definitions)
    // 1. Find the biggest number of edges and replace it with eadge otherwise skip.
    // 2. If there is more edges between the same vertices we will replace multiple egdes with one.

    vector<vector<int>> graph = reduceAllValuesToOne(matrix);
    BronKerbosch3(graph); // exponential

    cout << "Finished BronKerbosch3:\n";
    for (auto v : biggestCliqueBK) {
        cout << v << " ";
    }
    cout << endl;
    // TODO: finding largest clique in polynomial time - Monte carlo aproximation

    // TODO: Finding largest common subgraph
    //

    return 0;
}