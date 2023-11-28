#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Function to generate a random number of edges between 0 and maxEdges
int getRandomEdges(int maxEdges) {
    return rand() % (maxEdges + 1);
}

// Function to generate an undirected multigraph adjacency matrix
std::vector<std::vector<int>> generateMultigraph(int size, int maxEdges) {
    // Seed for random number generation
    srand(time(NULL));

    // Initialize the adjacency matrix with zeros
    std::vector<std::vector<int>> adjacencyMatrix(size, std::vector<int>(size, 0));

    // Generate the adjacency matrix for the undirected multigraph
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            int edges = getRandomEdges(maxEdges);
            adjacencyMatrix[i][j] = edges;
            adjacencyMatrix[j][i] = edges; // Due to undirected nature
        }
    }

    // Ensure multigraph is connected
    for (int i = 0; i < size; ++i) {
        for (int j = i; j < size; ++j) {
            if (adjacencyMatrix[i][j] == 0) {
                adjacencyMatrix[i][j] = 1;
                adjacencyMatrix[j][i] = 1;
                break;
            }
        }
    }

    return adjacencyMatrix;
}

// Function to display the adjacency matrix
void displayMatrix(const std::vector<std::vector<int>>& matrix) {
    std::cout << matrix.size() << std::endl;
    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <size> <maxEdges>" << std::endl;
        return 1;
    }

    // Extract parameters from command-line arguments
    int size = std::stoi(argv[1]);
    int maxEdges = std::stoi(argv[2]);

    // Generate and display the adjacency matrix
    std::vector<std::vector<int>> adjacencyMatrix = generateMultigraph(size, maxEdges);
    displayMatrix(adjacencyMatrix);

    return 0;
}
