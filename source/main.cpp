#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <unordered_set>
#include <iterator>
#include <cstdlib>
#include <chrono>
#include <fstream>

#include "colors.hpp"
#include "multigraph.hpp"

using namespace std;

int loadIntEnv(const char* env_var_name, int default_value);
bool loadEnvFlag(const char* env_var_name, bool default_value);
void largestClique(vector<vector<int>> matrix);
void LConnectivity(vector<vector<int>> matrix1, vector<vector<int>> matrix2);
string periodToString(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);
ifstream loadFile(const char* filename);

int main(int argc, char** argv) {

#pragma region Input files loading
    if (argc < 2 || argc > 3) {
        cout << "Usage: " << argv[0] << " <input_file1> [<input_file2>]" << endl;
        exit(1);
    }

    const bool two_files = argc == 3;
    int file1_graph_count, file2_graph_count;
    ifstream file1, file2;
    file1 = loadFile(argv[1]);

    file1 >> file1_graph_count;
    if (two_files) {
        file2 = loadFile(argv[2]);
        file2 >> file2_graph_count;
    } else {
        file2_graph_count = 0;
    }

    if (file1.eof() || (two_files && file2.eof())) {
        cerr << "Error: invalid file format" << endl;
        exit(1);
    }

    if (file1_graph_count < 0 || file2_graph_count < 0 || (file1_graph_count == 0 && file2_graph_count == 0)) {
        cerr << "Error: invalid graph count" << endl;
        exit(1);
    }
#pragma endregion    

#pragma region Environment variables loading
    const bool skip_clique = loadEnvFlag("SKIP_CLIQUE", false);
    const bool skip_conn = loadEnvFlag("SKIP_CONN", false);
    if (skip_clique && skip_conn) {
        cout << "Nothing to do. Both SKIP_CLIQUE and SKIP_CONN are set to 1.\n";
        return 0;
    }
    const int K_CLIQUE = loadIntEnv("K_CLIQUE", 3);
    const int L_CONN = loadIntEnv("L_CONN", 2);
#pragma endregion

    int test_cases = max(file1_graph_count, file2_graph_count);

    cout << BOLD << "Setup:\n" << RESET;
    cout << "K_CLIQUE = " << K_CLIQUE << "\n";
    cout << "L_CONN = " << L_CONN << "\n";
    cout << "File 1 graph count = " << file1_graph_count << "\n";
    cout << "File 2 graph count = " << file2_graph_count << "\n";
    cout << "Test cases = " << test_cases << "\n\n";

    for (int test_case = 0; test_case < test_cases; test_case++) {
        cout << BOLD << "-----------------------------Test case " << test_case + 1 << "-----------------------------" << RESET << endl;
        bool first_graph = test_case < file1_graph_count;
        bool second_graph = test_case < file2_graph_count;

        int matrix1_size, matrix2_size;
        vector<vector<int>> matrix1, matrix2;

        if (first_graph) {
            file1 >> matrix1_size;
            matrix1 = readMatrix(file1, matrix1_size);
            cout << "Adjacency matrix of " << test_case + 1 << " graph from first file:\n";
            printMatrix(matrix1);
            cout << endl;
        }

        if (second_graph) {
            file2 >> matrix2_size;
            matrix2 = readMatrix(file2, matrix2_size);
            cout << "Adjacency matrix of " << test_case + 1 << " graph from second file:\n";
            printMatrix(matrix2);
            cout << endl;
        }

        if (!skip_clique) {
            if (first_graph) {
                cout << BOLD << "Graph 1 (N = " << matrix1_size << ") largest clique:" << RESET << endl;
                largestClique(matrix1);
            }
            
            if (second_graph) {
                cout << BOLD << "Graph 2 (N = " << matrix2_size << ") largest clique:" << RESET << endl;
                largestClique(matrix2);
            }
        } else {
            if (first_graph) {
                cout << BOLD << "Graph 1 (N = " << matrix1_size << ")" << RESET << endl;
            }

            if (second_graph) {
                cout << BOLD << "Graph 2 (N = " << matrix2_size << ")" << RESET << endl;
            }
        }

        if (!skip_conn && first_graph && second_graph) {
            cout << BOLD << "L-connectivity:" << RESET << endl;
            LConnectivity(matrix1, matrix2);
        }
    }
    std::cout << std::endl;
    return 0;
}

int loadIntEnv(const char* env_var_name, int default_value) {
    const char* env_var = getenv(env_var_name);
    return env_var != nullptr ? std::stoi(env_var) : default_value;
}

bool loadEnvFlag(const char* env_var_name, bool default_value) {
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
    for (size_t i = 0; i < graph.size(); ++i) {
        P.insert(i);
    }

    auto start = chrono::steady_clock::now();
    bronKerbosch(R, P, X, graph);
    auto end = chrono::steady_clock::now();

    cout << periodToString(start, end) << "Using BronKerbosch method (size " << BOLD << biggestCliqueBK.size() << RESET << "):\n";
    cout << CYAN;
    for (auto v : biggestCliqueBK) {
        cout << v << " ";
    }
    cout << RESET << "\n\n";

    // Finding largest clique in polynomial time - Monte carlo aproximation
    start = chrono::steady_clock::now();
    vector<int> largest_clique = monteCarloClique(graph, approximateIterations(graph));
    end = chrono::steady_clock::now();
    std::cout << periodToString(start, end) << "Using Monte Carlo method (size " << BOLD << largest_clique.size() << RESET << "):\n";
    cout << GREEN;
    for (int x : largest_clique) {
        std::cout << x << " ";
    }
    cout << RESET << "\n\n";

    std::set<int> setLargestClique(largest_clique.begin(), largest_clique.end());
    cout << "Adjacency matrix using Monte Carlo method:" << endl;
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
    auto start = chrono::steady_clock::now();
    maximalCommonSubgraph(graph1, graph2);
    auto end = chrono::steady_clock::now();

    cout << periodToString(start, end) << "Largest common subgraph (size " << BOLD << largestMappings[0].size() << RESET << "):\n";
    cout << MAGENTA;
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

    start = chrono::steady_clock::now();
    approxCommonSubgraph(graph1, graph2); //Using DFS on 2 graphs simultaneously
    end = chrono::steady_clock::now();
    cout << periodToString(start, end) << "Largest common subgraph using DFS approximation (size " << BOLD << largestMappings[0].size() << RESET << "):\n";
    cout << GREEN;
    for (auto pair : largestMappings[0]) {
        cout << pair.first << " -> " << pair.second << "\n";
    }
    cout << RESET << endl;

    start = chrono::steady_clock::now();
    pair<int, int> distance = distanceBetweenGraphs(matrix1, matrix2, tmp);
    end = chrono::steady_clock::now();
    cout << periodToString(start, end) << "Distance between 2 graphs: " << BOLD << "(" << distance.first << " , " << distance.second << ")\n" << RESET << endl;
}

std::string periodToString(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return "[" + std::to_string(duration.count()) + " us] ";
}

std::ifstream loadFile(const char* filename) {
    std::ifstream fileStream(filename);

    if (!fileStream.is_open()) {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }

    return fileStream;
}
