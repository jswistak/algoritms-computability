#pragma once

#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>

extern std::set<int> biggestCliqueBK;
////////////////////////////
// MAXIMAL COMMON SUBGRAPH //
////////////////////////////
extern std::unordered_map<int, int> vertexMap;
extern std::unordered_set<int> mappedVertices1;
extern std::unordered_set<int> mappedVertices2;
extern std::vector<std::vector<std::pair<int,int>>> largestMappings;


void printColoredAdjacencyMatrix(const std::vector<std::vector<int>>& matrix, const std::set<int>& largest_clique, bool colorTheSame = false);
std::vector<std::vector<int>> readMatrix(int n);
std::vector<std::vector<int>> reduceAllValuesToOne(std::vector<std::vector<int>> multigraph, int k = 1);
bool isClique(std::vector<std::vector<int>>& graph, std::vector<int>& nodes);
int approximateIterations(const std::vector<std::vector<int>>& graph);
std::vector<int> monteCarloClique(std::vector<std::vector<int>>& graph, int iterations);
std::set<int> neighbors(int v, const std::vector<std::vector<int>>& adj);
void bronKerbosch(std::set<int>& R, std::set<int>& P, std::set<int>& X, const std::vector<std::vector<int>> graph);
void maximalCommonSubgraphProcess(const std::vector<std::vector<int>>& graph1, const std::vector<std::vector<int>>& graph2, std::vector<std::pair<int, int>>& edges1, std::vector<std::pair<int, int>>& edges2);
void maximalCommonSubgraph(const std::vector<std::vector<int>>& graph1, const std::vector<std::vector<int>>& graph2);
int getRandomElementFromSet(std::set<int> s);
std::set<int> subtract(std::set<int> s, std::unordered_set<int> us);
void maximalCommonSubgraphProcessHeuristic(const std::vector<std::vector<int>>& graph1, const std::vector<std::vector<int>>& graph2, int v1, int v2);
void approxCommonSubgraph(const std::vector<std::vector<int>>& graph1, const std::vector<std::vector<int>>& graph2);
int getEdges(const std::vector<std::pair<int, int>> mappingP, const std::vector<std::vector<int>>& graph, const std::vector<std::vector<int>>& graph2);
std::vector<std::pair<int, int>> getLargestMapping(const std::vector<std::vector<int>>& graph, const std::vector<std::vector<int>>& graph2);
int graphEdges(const std::vector<std::vector<int>>& graph);
std::pair<int, int> distanceBetweenGraphs(const std::vector<std::vector<int>>& graph1, const std::vector<std::vector<int>>& graph2, const std::vector<std::pair<int, int>>& mapping);