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
void printColoredAdjacencyMatrix(const std::vector<std::vector<int>>& matrix, const std::set<int>& largest_clique, bool colorTheSame = false) {

    std::cout << "\n" << RESET; // Reset text color to default
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            // Check if both nodes are in the largest clique
            if ((colorTheSame || i != j) && largest_clique.find(i) != largest_clique.end() && largest_clique.find(j) != largest_clique.end() and matrix[i][j] > 0) {
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

void bronKerbosch(set<int> &R, set<int> &P, set<int> &X, const vector<vector<int>> graph) {

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
        bronKerbosch(Rv, Pv, Xv, graph);
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
vector<vector<pair<int,int>>> largestMappings;

void maximalCommonSubgraphProcess(const vector<vector<int>> &graph1, const vector<vector<int>> &graph2, vector<pair<int,int>> &edges1, vector<pair<int,int>> &edges2) {
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
    if(largestMappings.size() == 0 || mapping.size() > largestMappings[0].size()){
        largestMappings.clear();
        largestMappings.push_back(mapping);
    } else if(mapping.size() == largestMappings[0].size()){
        largestMappings.push_back(mapping);
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
int get_random_element_from_set(set<int> s){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, s.size() - 1);

    int random_position = distr(gen);
    std::set<int>::iterator it = s.begin();
    std::advance(it, random_position);

    return *it;
}
set<int> subtract(set<int> s, unordered_set<int> us){
    set<int> result = s;
    for(auto element: s){
        if(us.find(element) != us.end()){
            result.erase(element);
        }
    }
    return result;
}

void maximalCommonSubgraphProcessHeuristic(const vector<vector<int>> &graph1, const vector<vector<int>> &graph2, int v1, int v2){
    set<int> n1 = neighbors(v1, graph1);
    set<int> n2 = neighbors(v2, graph2);

    //Vertices from G1 and G2 to be mapped
    n1 = subtract(n1, mappedVertices1);
    n2 = subtract(n2, mappedVertices2);
    if(n1.size() == 0 || n2.size() == 0){
        if(largestMappings.empty() || vertexMap.size() > largestMappings[0].size()){
            largestMappings.clear();
            vector<pair<int,int>> tmp;
            for(auto element: vertexMap){
                tmp.push_back({element.first, element.second});
            }
            largestMappings.push_back(tmp);
        }
        return;
    };

    //Choose random vertex from n1 and n2
    for(auto v: n1){
        if(mappedVertices1.find(v) != mappedVertices1.end()) continue;
        mappedVertices1.insert(v);
        n2 = subtract(n2, mappedVertices2);//TODO check if this is correct
        if(n2.size() == 0)
            break;

        int v2_random = get_random_element_from_set(n2);
        n2.erase(v2_random);

        mappedVertices2.insert(v2_random);
        vertexMap[v] = v2_random;
        maximalCommonSubgraphProcessHeuristic(graph1, graph2, v, v2_random);
    }
    if(largestMappings.empty() || vertexMap.size() > largestMappings[0].size()){
        largestMappings.clear();
            vector<pair<int,int>> tmp;
            for(auto element: vertexMap){
                tmp.push_back({element.first, element.second});
            }
            largestMappings.push_back(tmp);
    }

}
void approxCommonSubgraph(const vector<vector<int>> &graph1, const vector<vector<int>> &graph2){
    vertexMap.clear();
    mappedVertices1.clear();
    mappedVertices2.clear();

    for(int i = 0; i < graph1.size(); i++){
        for(int j = 0; j < graph2.size(); j++){
            vertexMap[i] = j;
            mappedVertices1.insert(i);
            mappedVertices2.insert(j);
            maximalCommonSubgraphProcessHeuristic(graph1, graph2, i, j);
            mappedVertices1.clear();
            mappedVertices2.clear();
            vertexMap.clear();

        }
    }
}

int getEdges(const vector<pair<int, int>> mappingP, const vector<vector<int>> &graph, const vector<vector<int>> &graph2){
    int edges = 0;
    for(auto mapping: mappingP){
        for(auto mapping2: mappingP){
            //if(mapping.first == mapping2.first) continue;
            if(graph[mapping.first][mapping2.first] > 0 && graph2[mapping.second][mapping2.second] > 0){
                edges += min(graph[mapping.first][mapping2.first], graph2[mapping.second][mapping2.second]);
            }
        }
    }
    return edges;
}
vector<pair<int, int>> getLargestMapping(const vector<vector<int>> &graph, const vector<vector<int>> &graph2){
    vector<pair<int, int>> largestMapping;
    int edges = 0;
    //cout << "largestMappings.size(): " << largestMappings.size() << endl;
    for(const auto& mapping: largestMappings){
        int tmp = getEdges(mapping, graph, graph2);

        if(tmp > edges){
            //cout << "tmp: " << tmp << endl;
            edges = tmp;
            largestMapping = mapping;
        }
    }
    return largestMapping;
}
int graphEdges(const vector<vector<int>> &graph){
    int edges = 0;
    for(int i = 0; i < graph.size(); ++i){
        for(int j = 0; j < graph.size(); ++j){
            edges += graph[i][j];
        }
    }
    return edges;
}
pair<int, int> distanceBetweenGraphs(const vector<vector<int>> &graph1, const vector<vector<int>> &graph2, const vector<pair<int, int>> &mapping){

    int distance_v = graph1.size() + graph2.size() - 2 * mapping.size();
    int distance_e = graphEdges(graph1) + graphEdges(graph2) - 2 * getEdges(mapping, graph1, graph2);
    return {distance_v, distance_e};
}

int main() {
    const int K_CLIQUE = 1;
    const int TRIES = 3;
    int test_cases;
    cin >> test_cases;
    for(int i = 0; i < test_cases; i++) {
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
        vector<vector<int>> graph2 = reduceAllValuesToOne(matrix2, K_CLIQUE);
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