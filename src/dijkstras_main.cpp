#include "dijkstras.h"
#include <iostream>
#include <fstream>


void run_test_from_file(const string& filename, int source, int destination) {
    Graph G;

    try {
        file_to_graph(filename, G);
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    vector<int> previous(G.numVertices);
    vector<int> distances = dijkstra_shortest_path(G, source, previous);
    vector<int> path = extract_shortest_path(distances, previous, destination);

    cout << "Graph from file: " << filename << endl;
    cout << "Source: " << source << ", Destination: " << destination << endl;
    print_path(path, distances[destination]);
}

int main() {
    // Run tests with provided test files
    run_test_from_file("small.txt", 0, 2);

    return 0;
}