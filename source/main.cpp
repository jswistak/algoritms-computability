#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <unordered_set>
#include <iterator>
#include <cstdlib>
#include <chrono>

#include "colors.hpp"
#include "multigraph.hpp"

using namespace std;

int loadIntEnv(const char* env_var_name, int default_value);
void largestClique(vector<vector<int>> matrix);
void LConnectivity(vector<vector<int>> matrix1, vector<vector<int>> matrix2);
std::string periodToString(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);

int main() {
    const int K_CLIQUE = loadIntEnv("K_CLIQUE", 3);
    const int L_CONN = loadIntEnv("L_CONN", 2);
    int test_cases;
    cin >> test_cases;

    cout << BOLD << "Settings:\n" << RESET;
    cout << "K_CLIQUE = " << K_CLIQUE << "\n";
    cout << "L_CONN = " << L_CONN << "\n";
    cout << "Number of test cases: " << test_cases << "\n" << endl;

    for (int test_case = 0; test_case < test_cases; test_case++) {
        cout << BOLD << "-----------------------------Test case " << test_case + 1 << "-----------------------------" << RESET << endl;

        int matrix1_size, matrix2_size;

        cin >> matrix1_size;
        vector<vector<int>> matrix1 = readMatrix(matrix1_size);

        cin >> matrix2_size;
        vector<vector<int>> matrix2 = readMatrix(matrix2_size);

        cout << BOLD << "Graph 1 (N = " << matrix1_size << ") largest clique:" << RESET << endl;
        largestClique(matrix1);

        cout << BOLD << "Graph 2 (N = " << matrix2_size << ") largest clique:" << RESET << endl;
        largestClique(matrix2);

        cout << BOLD << "L-connectivity:" << RESET << endl;
        LConnectivity(matrix1, matrix2);
    }
    std::cout << std::endl;
    return 0;
}

int loadIntEnv(const char* env_var_name, int default_value) {
    const char* env_var = getenv(env_var_name);
    return env_var != nullptr ? std::stoi(env_var) : default_value;
}

void largestClique(vector<vector<int>> matrix) {
    // Finding a clique in a graph

    // (it doesn't matter whether it is a multigraph, because we can have 2 definitions) (K - clique definition)
    // 1. Find the biggest number of edges and replace it with edge otherwise skip.
    // 2. If there is more edges between the same vertices we will replace multiple edges with one.

    vector<vector<int>> graph = reduceAllValuesToOne(matrix, 1);

    biggestCliqueBK.clear();
    set<int> R, P, X;
    for (int i = 0; i < graph.size(); ++i) {
        P.insert(i);
    }

    auto start = chrono::high_resolution_clock::now();
    bronKerbosch(R, P, X, graph);
    auto end = chrono::high_resolution_clock::now();

    cout << periodToString(start, end) << "Using BronKerbosch method (size " << BOLD << biggestCliqueBK.size() << RESET << "):\n";
    cout << CYAN;
    for (auto v : biggestCliqueBK) {
        cout << v << " ";
    }
    cout << RESET << "\n\n";

    // Finding largest clique in polynomial time - Monte carlo aproximation
    start = chrono::high_resolution_clock::now();
    vector<int> largest_clique = monteCarloClique(graph, approximateIterations(graph));
    end = chrono::high_resolution_clock::now();
    std::cout << periodToString(start, end) << "Using Monte Carlo method (size " << BOLD << largest_clique.size() << RESET << "):\n";
    cout << BLUE;
    for (int x : largest_clique) {
        std::cout << x << " ";
    }
    cout << RESET << "\n\n";

    start = chrono::high_resolution_clock::now();
    std::set<int> setLargestClique(largest_clique.begin(), largest_clique.end());
    end = chrono::high_resolution_clock::now();
    cout << periodToString(start, end) << "Adjacency matrix using Monte Carlo method:" << endl;
    printColoredAdjacencyMatrix(matrix, setLargestClique);
    cout << endl;
}

void LConnectivity(vector<vector<int>> matrix1, vector<vector<int>> matrix2) {
    auto graph1 = reduceAllValuesToOne(matrix1, 1);
    auto graph2 = reduceAllValuesToOne(matrix2, 1);

    // L-connectivity - it needs to be able to have at least L edges between each pair of vertices
    //Finding maximal common subgraph
    vertexMap.clear();
    mappedVertices1.clear();
    mappedVertices2.clear();
    largestMappings.clear();

    //Assuming each graph is a connected graph!
    auto start = chrono::high_resolution_clock::now();
    maximalCommonSubgraph(graph1, graph2);
    auto end = chrono::high_resolution_clock::now();

    cout << periodToString(start, end) << "Largest common subgraph (size " << BOLD << largestMappings[0].size() << RESET << "):\n";
    cout << CYAN;
    set<int> largestMappingG1;
    vector<pair<int, int>> tmp = getLargestMapping(graph1, graph2);
    for (auto pair : tmp) {
        cout << pair.first << " -> " << pair.second << "\n";
        largestMappingG1.insert(pair.first);
    }
    cout << RESET << endl;

    cout << "Adjacency matrix of largest common subgraph:\n";
    printColoredAdjacencyMatrix(matrix1, largestMappingG1, true);
    cout << endl;

    vertexMap.clear();
    mappedVertices1.clear();
    mappedVertices2.clear();
    largestMappings.clear();

    start = chrono::high_resolution_clock::now();
    approxCommonSubgraph(graph1, graph2); //Using DFS on 2 graphs simultaneously
    end = chrono::high_resolution_clock::now();
    cout << periodToString(start, end) << "Largest common subgraph using DFS approximation (size " << BOLD << largestMappings[0].size() << RESET << "):\n";
    cout << BLUE;
    for (auto pair : largestMappings[0]) {
        cout << pair.first << " -> " << pair.second << "\n";
    }
    cout << RESET << endl;

    start = chrono::high_resolution_clock::now();
    pair<int, int> distance = distanceBetweenGraphs(matrix1, matrix2, tmp);
    end = chrono::high_resolution_clock::now();
    cout << periodToString(start, end) << "Distance between 2 graphs: " << BOLD << "(" << distance.first << " , " << distance.second << ")\n" << RESET << endl;
}

std::string periodToString(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return "[" + std::to_string(duration.count()) + " us] ";
}