#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>

using namespace std;

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

vector<vector<int>> reduceAllValuesToOne(vector<vector<int>> multigraph, int k = 1) {
    vector<vector<int>> graph;
    for (int i = 0; i < multigraph.size(); i++) {
        vector<int> row;
        for (int j = 0; j < multigraph.size(); j++) {
            if (multigraph[i][j] >= k) {
                row.push_back(1);
            } else {
                row.push_back(0);
            }
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


bool isClique(std::vector<std::vector<int>> &graph, std::vector<int> &nodes) {
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = i + 1; j < nodes.size(); ++j) {
            if (graph[nodes[i]][nodes[j]] == 0) {
                return false;
            }
        }
    }
    return true;
}

int approximateIterations(const std::vector<std::vector<int>> &graph) {
    int n = graph.size(); // Number of vertices

    // Calculate edge count
    int edgeCount = 0;
    for (int i = 0; i < n; i++) {
        edgeCount += graph[i].size();
    }
    const int scaleFactor = 15;

    return (edgeCount / (n + 1)) * scaleFactor;
}

std::vector<int> monteCarloClique(std::vector<std::vector<int>> &graph, int iterations) {
    std::vector<int> max_clique;
    std::random_device rd;
    std::mt19937 gen(rd());
    int n = graph.size();

    for (int i = 0; i < iterations; ++i) {
        int sample_size = gen() % (n + 1 - max_clique.size()) + max_clique.size(); // Will only look for cliques bigger than the current max clique
        std::vector<int> sampled_nodes;

        std::vector<int> node_list(n);
        std::iota(node_list.begin(), node_list.end(), 0);

        std::shuffle(node_list.begin(), node_list.end(), gen);
        std::copy_n(node_list.begin(), sample_size, std::back_inserter(sampled_nodes));

        if (isClique(graph, sampled_nodes)) {
            if (sampled_nodes.size() > max_clique.size()) {
                max_clique = sampled_nodes;
            }
        }
    }

    return std::sort(max_clique.begin(), max_clique.end()), max_clique;
}

set<int> intersection(set<int> x, set<int> y) {
    set<int> result;
    set_intersection(x.begin(), x.end(), y.begin(), y.end(), inserter(result, result.end()));
    return result;
}

set<int> neighbors(int v, const vector<vector<int>> &adj) {
    set<int> result;
    for (int i = 0; i < adj.size(); ++i) {
        if (adj[v][i]) {
            result.insert(i);
        }
    }
    return result;
}

vector<int> biggestCliqueBK;
void BronKerbosch(set<int> &R, set<int> &P, set<int> &X, const vector<vector<int>> &graph) {

    if (P.empty() && X.empty()) {
        cout << "MAX CLIQUE: " << endl;
        for (auto a : R) {
            cout << a << " ";
        }
        cout << "\n";
        return;
    }

    for (int v : P) {
        set<int> N = neighbors(v, graph);
        set<int> Rv = R;
        Rv.insert(v);
        set<int> Pv = intersection(P, N);
        set<int> Xv = intersection(X, N);
        BronKerbosch(Rv, Pv, Xv, graph);

        P.erase(v);
        X.insert(v);
    }
}
int main() {
    int n;
    cin >> n;
    vector<vector<int>> matrix = readMatrix(n);
    cout << "Matrix" << endl;
    // Finging a clique in a graph

    // (it doesn't matter wheather it is a multigraph, because we can have 2 definitions) (K - clique definition TODO)
    // 1. Find the biggest number of edges and replace it with eadge otherwise skip.
    // 2. If there is more edges between the same vertices we will replace multiple egdes with one.

    vector<vector<int>> graph = reduceAllValuesToOne(matrix, 1);


    cout << "BronKerbosch:\n";

    set<int> R, P, X;
    for (int i = 0; i < graph.size(); ++i) {
        P.insert(i);
    }
    cout << "Running\n";
    BronKerbosch(R, P, X, graph);
    cout << "Not running\n";

    std::sort(biggestCliqueBK.begin(), biggestCliqueBK.end());
    for (auto v : biggestCliqueBK) {
        cout << v << " ";
    }

    // Finding largest clique in polynomial time - Monte carlo aproximation
    vector<int> largest_clique = monteCarloClique(graph, 500000 * approximateIterations(graph));
    // 0 2 8 13 16 17 21 25 26 31 34 36 39
    std::cout << "Largest clique Monte Carlo:\n";
    cout << "Size: " << largest_clique.size() << endl;
    for (int x : largest_clique) {
        std::cout << x << " ";
    }
    // TODO: Homenda's request neighbourhood matrix with marked max Clique
    // TODO: Progress bar in monte carlo - TBD

    std::cout << std::endl;

    // TODO: Finding largest common subgraph
    // L-connectivity(?)
    return 0;
}