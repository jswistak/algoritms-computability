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

int loadIntEnv(const char* env_var_name, int default_value) {
    const char* env_var = getenv(env_var_name);

    if (env_var != nullptr) {
        // Convert to integer
        int value = std::stoi(env_var);
        std::cout << "Environment variable " << env_var_name << " set to value: " << value << std::endl;
        return value;
    } else {
        std::cerr << "Environment variable not set" << std::endl;
        return default_value;
    }
}


int main() {

    const int K_CLIQUE = loadIntEnv("K_CLIQUE", 3);
    const int L_CONN = loadIntEnv("L_CONN", 2);
    int test_cases;
    cin >> test_cases;
    for(int test_case = 0; test_case < test_cases; test_case++) {
        int n;
        cin >> n;
        vector<vector<int>> matrix = readMatrix(n);
        // Finding a clique in a graph

        // (it doesn't matter whether it is a multigraph, because we can have 2 definitions) (K - clique definition)
        // 1. Find the biggest number of edges and replace it with edge otherwise skip.
        // 2. If there is more edges between the same vertices we will replace multiple edges with one.

        vector<vector<int>> graph = reduceAllValuesToOne(matrix, K_CLIQUE);

        // Finding largest clique
        {
            biggestCliqueBK.clear();
            set<int> R, P, X;
            for (int i = 0; i < graph.size(); ++i) {
                P.insert(i);
            }
            bronKerbosch(R, P, X, graph);


            cout << "\nBronKerbosch size - " << biggestCliqueBK.size() << " :\n";
            cout << CYAN;
            for (auto v: biggestCliqueBK) {
                cout << v << " ";
            }
            cout << RESET;
            cout << "\n";
            // Finding largest clique in polynomial time - Monte carlo aproximation
            vector<int> largest_clique = monteCarloClique(graph, approximateIterations(graph));
            std::cout << "Largest clique Monte Carlo - " << largest_clique.size() << " :\n";
            cout << BLUE;
            for (int x: largest_clique) {
                std::cout << x << " ";
            }
            cout << RESET << "\n";

            std::set<int> setLargestClique(largest_clique.begin(), largest_clique.end());
            cout << "Adjacency matrix of largest clique using Monte Carlo:\n";
            printColoredAdjacencyMatrix(matrix, setLargestClique);

        }


        cin >> n;
        vector<vector<int>> matrix2 = readMatrix(n);

        // L-connectivity - it needs to be able to have at least L edges between each pair of vertices
        vector<vector<int>> graph2 = reduceAllValuesToOne(matrix2, L_CONN);
        graph = reduceAllValuesToOne(matrix, L_CONN);
        cout << "L-connectivity: \n";
        //Finding maximal common subgraph
        {
            vertexMap.clear();
            mappedVertices1.clear();
            mappedVertices2.clear();
            largestMappings.clear();

            //Assuming each graph is a connected graph!
            maximalCommonSubgraph(graph, graph2);

            cout << "Largest common subgraph of size - " << largestMappings[0].size() << " : \n";
            cout << CYAN;
            set<int> largestMappingG1;
            vector<pair<int, int>> tmp = getLargestMapping(graph, graph2);
            for (auto pair: tmp) {
                cout << pair.first << " -> " << pair.second << "\n";
                largestMappingG1.insert(pair.first);
            }
            cout << RESET;
            cout << "Adjacency matrix of largest common subgraph (exp):\n";
            printColoredAdjacencyMatrix(matrix, largestMappingG1, true);


            vertexMap.clear();
            mappedVertices1.clear();
            mappedVertices2.clear();
            largestMappings.clear();

            approxCommonSubgraph(graph, graph2); //Using DFS on 2 graphs simultaneously
            cout << "Largest common subgraph of size - " << largestMappings[0].size() << " :\n" << BLUE;
            for (auto pair: largestMappings[0]) {
                cout << pair.first << " -> " << pair.second << "\n";
            }
            cout << RESET;
            cout << "Distance between 2 graphs: ";
            pair<int, int> distance = distanceBetweenGraphs(matrix, matrix2, tmp);
            cout << "(" << distance.first << " , " << distance.second << ")\n";
        }

    }
    std::cout << std::endl;
    return 0;
}