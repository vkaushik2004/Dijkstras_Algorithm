#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <climits> // Include for INT_MAX
//instead of using multiple files as were given to us I found it easier to put everything into one file

//give the edge class a starting vertex, an ending and a weight for the edge
struct Edge {
    int startVert;
    int endVert;
    int weight;
    Edge() : startVert(-1), endVert(-1), weight(-1) {}
    Edge(int start, int end) : startVert(start), endVert(end), weight(1) {}
};

//each graph has a number of vertices and edges provided by the input text file
class Graph {
private:
    int numOfVertices;
    int numOfEdges;
    int** adjacencyMatrix;
    int* degrees;


//create an adjacency matrix
public:
//this initiates the graph with the first two values of the txt file and fills it up with zeros
    Graph(int n, int m, const int* edgeData) : numOfVertices(n), numOfEdges(m) {
        adjacencyMatrix = new int*[numOfVertices];
        for (int i = 0; i < numOfVertices; ++i) {
            adjacencyMatrix[i] = new int[numOfVertices];
            for (int j = 0; j < numOfVertices; ++j) {
                adjacencyMatrix[i][j] = 0;
            }
        }
        degrees = new int[numOfVertices];
        for (int i = 0; i < numOfVertices; ++i) {
            degrees[i] = 0;
        }

// Fill adjacency matrix and update degrees
        for (int i = 0; i < m; ++i) {
            int startVertex = edgeData[i * 2];
            int endVertex = edgeData[i * 2 + 1];
// Update matrix for edge between startVertex and endVertex
            adjacencyMatrix[startVertex - 1][endVertex - 1] = 1;
            adjacencyMatrix[endVertex - 1][startVertex - 1] = 1;
           // Update degrees for startVertex and endVertex
            degrees[startVertex - 1]++;
            degrees[endVertex - 1]++;
        }
    }

    // Function to get the degree of a vertex 
    int getDegree(int vertex) const {
        return degrees[vertex - 1]; // Adjust for 1-based indexing
    }

    void printAdjacencyMatrix() const {
        for (int i = 0; i < numOfVertices; ++i) {
            for (int j = 0; j < numOfVertices; ++j) {
                std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

//find the odd degrees by summing the rows of the adjacency matrix and checking if its odd
    void findOddDegreeVertices() const {
        std::cout << "The odd degree vertices in G:" << std::endl;
        std::cout << "O = { ";
        for (int i = 0; i < numOfVertices; ++i) {
            int degree = 0;
            for (int j = 0; j < numOfVertices; ++j) {
                degree += adjacencyMatrix[i][j];
            }
            if (degree % 2 != 0) {
                std::cout << i + 1 << " ";
            }
        }
        std::cout << "}" << std::endl;
    }

    void dijkstra(int source) const {
    std::cout << "\nSingle source shortest path lengths from node " << source << std::endl;

    int* dist = new int[numOfVertices];
    bool* visited = new bool[numOfVertices];

    // Initialize distances and visited array
    for (int i = 0; i < numOfVertices; ++i) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    
    // Set the distance of the source node to 0
    dist[source - 1] = 0;

    // Mark a node as visited and mark the shortest distance
    for (int count = 0; count < numOfVertices - 1; ++count) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        // Update distances for adjacent vertices
        for (int v = 0; v < numOfVertices; ++v) {
            if (!visited[v] && adjacencyMatrix[u][v] && dist[u] != INT_MAX && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
            }
        }
    }

    // Print shortest path lengths
    for (int i = 0; i < numOfVertices; ++i) {
        std::cout << "  " << i + 1 << ": ";
        if (dist[i] != INT_MAX) {
            std::cout << dist[i]; // Print the shortest path length
        } else {
            std::cout << "No path"; // Print if no path exists
        }
        std::cout << std::endl;
    }

    delete[] dist;
    delete[] visited;
}


//function that iterates through vertices and finds smallest distance
private:
    int minDistance(const int* dist, const bool* visited) const {
        int min = INT_MAX;
        int minIndex = -1;
        for (int v = 0; v < numOfVertices; ++v) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                minIndex = v;
            }
        }
        return minIndex;
    }
};

int main() {
    //intialize variables
    int numOfVertices;
    int numOfEdges;

    std::cin >> numOfVertices;
    std::cin >> numOfEdges;

    int* edgeData = new int[numOfEdges * 2];
    for (int i = 0; i < numOfEdges * 2; ++i) {
        std::cin >> edgeData[i];
    }

    Graph graph(numOfVertices, numOfEdges, edgeData);

    // Printing the adjacency matrix
    std::cout << "The adjacency matrix of G:" << std::endl;
    graph.printAdjacencyMatrix();

    // Finding odd degree vertices
    graph.findOddDegreeVertices();

    // Executing Dijkstra's for odds
    for (int i = 1; i <= numOfVertices; ++i) {
        if (graph.getDegree(i) % 2 != 0) {
            graph.dijkstra(i);
        }
    }

    delete[] edgeData;

    return 0;
}
