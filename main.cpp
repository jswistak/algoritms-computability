#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <unordered_set>
#include <iterator>


using namespace std;

const std::string RED = "\033[31m";      // Red
const std::string GREEN = "\033[32m";    // Green
const std::string YELLOW = "\033[33m";   // Yellow
const std::string BLUE = "\033[34m";     // Blue
const std::string MAGENTA = "\033[35m";  // Magenta
const std::string CYAN = "\033[36m";     // Cyan
const std::string RESET = "\033[0m";     // Reset to default color


// Function to print the adjacency matrix with the clique elements colored
void printColoredAdjacencyMatrix(const std::vector<std::vector<int>>& matrix, const std::set<int>& largest_clique) {

    std::cout << "\n" << RESET; // Reset text color to default
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            // Check if both nodes are in the largest clique
            if (i != j && largest_clique.find(i) != largest_clique.end() && largest_clique.find(j) != largest_clique.end()) {
                std::cout << RED; // Set text color to red for clique elements
            }
            else {
                std::cout << RESET; // Reset text color to default
            }

            // Print the matrix element
            std::cout << matrix[i][j] << " ";


        }
        std::cout << std::endl; // Newline after each row
    }
    std::cout << RESET; // Reset text color to default
}
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
            if(i == j){
                row.push_back(0);
            }
            else if (multigraph[i][j] >= k) {
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
    const int scaleFactor = 200;

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


set<int> neighbors(int v, const vector<vector<int>> &adj) {
    set<int> result;
    for (int i = 0; i < adj.size(); ++i) {
        if (adj[v][i]) {
            result.insert(i);
        }
    }
    return result;
}

set<int> biggestCliqueBK;
void BronKerbosch(set<int> &R, set<int> &P, set<int> &X, const vector<vector<int>> graph) {

    if (P.empty() && X.empty()) {
        if(R.size() > biggestCliqueBK.size()){
            biggestCliqueBK = R;
        }
//        for (auto a : R) {
//            cout << a << " ";
//        }
//        cout << "\n";
        return;
    }

    vector<int> verticesToProcess(P.begin(), P.end());
    for (int v : verticesToProcess) {
        if (P.find(v) == P.end()) continue; // Skip if v was already removed from P

        set<int> N = neighbors(v, graph);
        set<int> Rv = R;
        Rv.insert(v);
        set<int> Pv; set_intersection(P.begin(), P.end(), N.begin(), N.end(), inserter(Pv, Pv.end()));
        set<int> Xv; set_intersection(X.begin(), X.end(), N.begin(), N.end(), inserter(Xv, Xv.end()));
        BronKerbosch(Rv, Pv, Xv, graph);
        P.erase(v);
        X.insert(v);
    }
}



////////////////////////////
// MAXIMAL COMMON SUBGRAPH //
////////////////////////////
unordered_map<int, int> vertexMap;
unordered_set<int> mappedVertices1;
unordered_set<int> mappedVertices2;
vector<pair<int,int>> largestMapping;
void maximalCommonSubgraphProcess(const vector<vector<int>> &graph1, const vector<vector<int>> &graph2, vector<pair<int,int>> &edges1, vector<pair<int,int>> &edges2){
    for(auto element: vertexMap){
        for(int v1 = 0; v1 < graph1[element.first].size(); ++v1){ //iterate over all neighbors of v1 not yet mapped
            if(mappedVertices1.find(v1) != mappedVertices1.end()) continue;
            if(graph1[element.first][v1] == 0) continue;

            mappedVertices1.insert(v1);
            for(int v2 = 0; v2 < graph2[element.second].size(); ++v2){ //iterate over all neighbors of v2 not yet mapped
                if(mappedVertices2.find(v2) != mappedVertices2.end()) continue;
                if(graph2[element.second][v2] == 0) continue;

                mappedVertices2.insert(v2);
                //We can try to map v1 to v2
                vertexMap[v1] = v2;
                maximalCommonSubgraphProcess(graph1, graph2, edges1, edges2);

                vertexMap.erase(v1);
                mappedVertices2.erase(v2);

            }
            mappedVertices1.erase(v1);
            //Debug stack overflow error here

        }
    }
    //cout << "Possible mapping: \n";
    vector<pair<int,int>> mapping;
    for(auto pair: vertexMap){
        //cout << pair.first + 1 << " - >" << pair.second + 1 << "\n";
        mapping.push_back({pair.first, pair.second});
    }
    if(mapping.size() > largestMapping.size()){
        largestMapping = mapping;
    }
    //try to save as the biggest common subgraph
}
void maximalCommonSubgraph(const vector<vector<int>> &graph1, const vector<vector<int>> &graph2) {
    for(int i = 0; i < graph1.size(); ++i){
        for(int j = 0; j < graph2.size(); ++j){
            vertexMap[i] = j;
            mappedVertices1.insert(i);
            mappedVertices2.insert(j);
            vector<pair<int,int>> edges1;
            vector<pair<int,int>> edges2;
            maximalCommonSubgraphProcess(graph1, graph2, edges1, edges2);
            mappedVertices1.erase(i);
            mappedVertices2.erase(j);
            vertexMap.erase(i);
        }
    }
}








int main() {
    const int K_CLIQUE = 1;
    const int L_MAXIMAL_COMMON_SUBGRAPH = 2;
    const int TRIES = 3;
    int n;
    cin >> n;
    vector<vector<int>> matrix = readMatrix(n);
    // Finding a clique in a graph

    // (it doesn't matter whether it is a multigraph, because we can have 2 definitions) (K - clique definition TODO)
    // 1. Find the biggest number of edges and replace it with edge otherwise skip.
    // 2. If there is more edges between the same vertices we will replace multiple edges with one.

    vector<vector<int>> graph = reduceAllValuesToOne(matrix, K_CLIQUE);

    // Finding largest clique
    {
        set<int> R, P, X;
        for (int i = 0; i < graph.size(); ++i) {
            P.insert(i);
        }
        BronKerbosch(R, P, X, graph);


        cout << "BronKerbosch size - " << biggestCliqueBK.size() << " :\n";
        cout << CYAN;
        for (auto v: biggestCliqueBK) {
            cout << v << " ";
        }
        cout << RESET;
        cout << "\n";
        // Finding largest clique in polynomial time - Monte carlo aproximation
        vector<int> largest_clique = monteCarloClique(graph, approximateIterations(graph));
        // 0 2 8 13 16 17 21 25 26 31 34 36 39
        std::cout << "Largest clique Monte Carlo - " << largest_clique.size() << " :\n";
        cout << BLUE;
        for (int x: largest_clique) {
            std::cout << x << " ";
        }
        cout << RESET << "\n";
        std::set<int> setLargestClique(largest_clique.begin(), largest_clique.end());
        printColoredAdjacencyMatrix(graph, setLargestClique);

    }



    cin >> n;
    vector<vector<int>> matrix2 = readMatrix(n);

    vector<vector<int>> graph2 = reduceAllValuesToOne(matrix2, K_CLIQUE);

    //Finding maximal common subgraph
    {
        //Assuming each graph is a connected graph!
        maximalCommonSubgraph(graph, graph2);

        cout << "Largest common subgraph of size - " << largestMapping.size() << " : \n";
        cout << CYAN;
        for(auto pair: largestMapping){
            cout << pair.first << " -> " << pair.second  << "\n";
        }
        cout << RESET;


        //Longest path in a graph or some common BFS or DFS //TBD
        //approxCommonSubgraph(graph, graph2, TRIES);


        //Distance between 2 graphs TBD
    }


    std::cout << std::endl;

    // TODO: Finding largest common subgraph
    // L-connectivity(?)
    return 0;
}