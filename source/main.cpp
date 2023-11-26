#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <unordered_set>
#include <iterator>
#include <cstdlib>

#include "colors.hpp"
#include "multigraph.hpp"

using namespace std;

int loadIntEnv(const char* env_var_name, int default_value);
void largestClique(vector<vector<int>> matrix);
void LConnectivity(vector<vector<int>> matrix1, vector<vector<int>> matrix2);

int main() {
    const int K_CLIQUE = loadIntEnv("K_CLIQUE", 3);
    const int L_CONN = loadIntEnv("L_CONN", 2);

    cout << "K_CLIQUE = " << K_CLIQUE << "\n";
    cout << "L_CONN = " << L_CONN << "\n\n";

    int test_cases;
    cin >> test_cases;
    cout << "Number of test cases (one test case = 2 graphs) : " << test_cases << "\n\n";

    for (int test_case = 0; test_case < test_cases; test_case++) {
        int matrix1_size, matrix2_size;

        cin >> matrix1_size;
        vector<vector<int>> matrix1 = readMatrix(matrix1_size);

        cin >> matrix2_size;
        vector<vector<int>> matrix2 = readMatrix(matrix2_size);

        cout << "Test case " << test_case + 1 << ":\n\n";

        largestClique(matrix1);
        largestClique(matrix2);       

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
    bronKerbosch(R, P, X, graph);


    cout << "BronKerbosch size - " << biggestCliqueBK.size() << " :\n" << CYAN;
    for (auto v: biggestCliqueBK) {
        cout << v << " ";
    }
    cout << RESET << "\n\n";

    // Finding largest clique in polynomial time - Monte carlo aproximation
    vector<int> largest_clique = monteCarloClique(graph, approximateIterations(graph));
    std::cout << "Largest clique Monte Carlo - " << largest_clique.size() << " :\n" << BLUE;
    for (int x: largest_clique) {
        std::cout << x << " ";
    }
    cout << RESET << "\n";

    std::set<int> setLargestClique(largest_clique.begin(), largest_clique.end());
    cout << "Adjacency matrix of largest clique using Monte Carlo:\n";
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
    maximalCommonSubgraph(graph1, graph2);

    cout << "Largest common subgraph of size - " << largestMappings[0].size() << " : \n";
    cout << CYAN;
    set<int> largestMappingG1;
    vector<pair<int, int>> tmp = getLargestMapping(graph1, graph2);
    for (auto pair : tmp) {
        cout << pair.first << " -> " << pair.second << "\n";
        largestMappingG1.insert(pair.first);
    }
    cout << RESET;
    cout << "Adjacency matrix of largest common subgraph (exp):\n";
    printColoredAdjacencyMatrix(matrix1, largestMappingG1, true);


    vertexMap.clear();
    mappedVertices1.clear();
    mappedVertices2.clear();
    largestMappings.clear();

    approxCommonSubgraph(graph1, graph2); //Using DFS on 2 graphs simultaneously
    cout << "Largest common subgraph of size - " << largestMappings[0].size() << " :\n" << BLUE;
    for (auto pair : largestMappings[0]) {
        cout << pair.first << " -> " << pair.second << "\n";
    }
    cout << RESET;
    cout << "Distance between 2 graphs: ";
    pair<int, int> distance = distanceBetweenGraphs(matrix1, matrix2, tmp);
    cout << "(" << distance.first << " , " << distance.second << ")\n";
}