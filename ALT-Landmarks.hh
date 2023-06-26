//
// Created by alvar on 29/05/2023.
//

#ifndef PRAKTIKUM_ALT_LANDMARKS_HH
#define PRAKTIKUM_ALT_LANDMARKS_HH

#include "Graph.hh"
#include <climits>
#include "APQ.hh"
#include "Dijkstra.hh"
#include <cmath>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <random>

double multiSourceDijkstra(Graph& myGraph, const std::vector<double>& sourceNodes) {
    // Get a priority queue
    APQ apq = APQ();

    // Initialize the distances to infinity
    std::vector<double> dist(myGraph.nodes.size(), INT_MAX);

    // Insert all source nodes with distance 0 into the APQ
    for (double sourceNode : sourceNodes) {
        apq.insertNode(sourceNode - 1, 0);
        dist[sourceNode - 1] = 0;
    }

    // Main loop
    while (!apq.isEmpty()) {
        // Active node. Note that if it's already in the set, inserting doesn't do anything
        double currentNode = apq.popMin();

        double startEdge = (currentNode > 0) ? myGraph.edgeStarts[currentNode - 1] + 1 : 0;
        double endEdge = myGraph.edgeStarts[currentNode];

        // Look for edges to relax
        for (double edgeIndex = startEdge; edgeIndex <= endEdge; edgeIndex++) {
            double edge = myGraph.edges[edgeIndex] - 1;

            // Calculate the weight as the Euclidean distance between the nodes
            double weight = distance(myGraph.nodes[currentNode], myGraph.nodes[edge]);

            // Relax the edge if a shorter path is found
            if (dist[currentNode] + weight < dist[edge]) {
                dist[edge] = dist[currentNode] + weight;

                // Decrease key operation if the node is already in APQ
                if (apq.contains(edge)) {
                    apq.decreaseKey(edge, dist[edge]);
                }
                    // Otherwise, insert it into the APQ
                else {
                    apq.insertNode(edge, dist[edge]);
                }
            }
        }
    }

    // Find the farthest node from all sources
    double farthestNode = -1;
    double maxDistance = INT_MIN;
    for (double i = 0; i < dist.size(); i++) {
        if (dist[i] > maxDistance && dist[i] < INT_MAX) {
            maxDistance = dist[i];
            farthestNode = i;
        }
    }

    return farthestNode;
}


std::vector<std::vector<double>> precomputePotentialsEuclidian(Graph myGraph, const std::vector<double>& landmarks){
    std::vector<std::vector<double>> potentialsInverted;
    for (double i = 0; i<landmarks.size(); i++){
        potentialsInverted.push_back(DijkstraToALL(myGraph, landmarks[i]));

    }
    return potentialsInverted;
}


std::vector<double> computeFarthestLandmarks(Graph myGraph, int numLandmarks, std::string filename) {
    std::vector<double> landmarks;
    std::vector<Node> nodes = myGraph.getNodes();
    std::unordered_set<double> selectedNodes;  // To keep track of selected landmarks

    // Select the first landmark node as the most southwestern one
    int firstLandmark = 0;
    for (Node node : myGraph.nodes){
        if (node.coordinateX+node.coordinateY < myGraph.nodes[firstLandmark].coordinateX+myGraph.nodes[firstLandmark].coordinateY){
            firstLandmark = node.nodeId;
        }
    }
    landmarks.push_back(firstLandmark);
    selectedNodes.insert(firstLandmark);

    // Select the remaining landmarks
    for (int i = 1; i < numLandmarks; i++) {
        double maxDistance = 0;
        int FarthestNode = -1;

        // Find the node that is Farthest from the already selected landmarks
        for (int j = 0; j < nodes.size(); j++) {
            if (selectedNodes.find(j) == selectedNodes.end()) {
                double minDistance = INT_MAX;

                // Calculate the minimum distance to already selected landmarks
                for (int landmark : landmarks) {
                    double landmarkDistance = distance(myGraph.nodes[landmark], myGraph.nodes[j]);
                    if (landmarkDistance < minDistance) {
                        minDistance = landmarkDistance;
                    }
                }

                // Update the Farthest node if it has the maximum minimum distance
                if (minDistance > maxDistance) {
                    maxDistance = minDistance;
                    FarthestNode = j;
                }
            }
        }

        // Add the Farthest node as a landmark
        landmarks.push_back(FarthestNode);
        selectedNodes.insert(FarthestNode);
    }
    //save the landmarks to a file
    std::ofstream file(filename);

    for (double landmark : landmarks) {
        file << landmark << std::endl;
    }
    file.close();
    std::cout<<"Writing landmarks to "<<filename+"coord"<<std::endl;
    std::ofstream file2(filename+"coord");

    for (double landmark : landmarks) {
        file2 << myGraph.getNode(landmark).coordinateX << " "<<myGraph.getNode(landmark).coordinateY << std::endl;
    }
    file2.close();
    return landmarks;
}

std::vector<double> farthestLandmarkSelection(Graph& myGraph, int numLandmarks, std::string filename){
    std::vector<double> landmarks;
    std::unordered_set<double> selectedNodes;
    std::vector<Node> nodes = myGraph.getNodes();


    //Randomly select the first landmarks node
    // Randomly select the first landmark node
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, nodes.size() - 1);
    int firstLandmark = dist(rng);
    landmarks.push_back(firstLandmark);
    selectedNodes.insert(firstLandmark);

    //Select the remaining landmarks
    for (int i = 1; i<numLandmarks; i++){
        double nextLandmark = multiSourceDijkstra(myGraph, landmarks);
        landmarks.push_back(nextLandmark);
        selectedNodes.insert(nextLandmark);
    }

    //save the landmarks to a file
    std::ofstream file(filename);

    for (double landmark : landmarks) {
        file << landmark << std::endl;
    }
    file.close();
    std::cout<<"Writing landmarks to "<<filename+"coord"<<std::endl;
    std::ofstream file2(filename+"coord");

    for (double landmark : landmarks) {
        file2 << myGraph.getNode(landmark).coordinateX << " "<<myGraph.getNode(landmark).coordinateY << std::endl;
    }
    file2.close();

    return landmarks;

}
std::vector<double> avoidLandmarkSelection(Graph myGraph, int numLandmarks, std::string filename) {
    std::vector<double> landmarks;
    std::vector<Node> nodes = myGraph.getNodes();
    std::unordered_set<double> selectedNodes;  // To keep track of selected landmarks

    // Randomly select the first landmark node
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, nodes.size() - 1);
    int firstLandmark = dist(rng);
    landmarks.push_back(firstLandmark);
    selectedNodes.insert(firstLandmark);

    // Select the remaining landmarks
    for (int i = 1; i < numLandmarks; i++) {
        // Calculate the weights and sizes of nodes
        std::vector<double> weights(nodes.size(), 0.0);
        std::vector<double> sizes(nodes.size(), 0.0);

        // Grow a shortest-path tree Tr from a random node r
        int r = dist(rng);
        std::queue<int> nodeQueue;
        nodeQueue.push(r);
        std::vector<bool> visited(nodes.size(), false);
        visited[r] = true;

        while (!nodeQueue.empty()) {
            int currentNode = nodeQueue.front();
            nodeQueue.pop();

            // Calculate the lower bound d(v, r) for each node v
            double lowerBound = std::numeric_limits<double>::infinity();
            for (double landmark : landmarks) {
                double landmarkDistance = distance(nodes[landmark], nodes[currentNode]);
                if (landmarkDistance < lowerBound) {
                    lowerBound = landmarkDistance;
                }
            }

            // Calculate the weight of each node v
            double nodeWeight = distance(nodes[currentNode], nodes[r]) - lowerBound;
            weights[currentNode] = nodeWeight;

            // Calculate the size of each node v
            sizes[currentNode] = nodeWeight;

            // Traverse the neighbors of the current node
            for (int neighbor : myGraph.getNeighbors(currentNode)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    nodeQueue.push(neighbor);
                }
            }
        }

        // Set the size of nodes containing a landmark to zero
        for (double landmark : landmarks) {
            sizes[landmark] = 0.0;
        }

        // Pick the node with maximum size as the next landmark
        double maxNodeSize = -1.0;
        int nextLandmark = -1;
        for (int node = 0; node < nodes.size(); node++) {
            double nodeSize = sizes[node];
            if (nodeSize > maxNodeSize) {
                maxNodeSize = nodeSize;
                nextLandmark = node;
            }
        }

        // Add the next landmark to the set
        landmarks.push_back(nextLandmark);
        selectedNodes.insert(nextLandmark);
    }
    //save the landmarks to a file
    std::ofstream file(filename);

    for (double landmark : landmarks) {
        file << landmark << std::endl;
    }
    file.close();
    std::cout<<"Writing landmarks to "<<filename+"coord"<<std::endl;
    std::ofstream file2(filename+"coord");

    for (double landmark : landmarks) {
        file2 << myGraph.getNode(landmark).coordinateX << " "<<myGraph.getNode(landmark).coordinateY << std::endl;
    }
    file2.close();

    return landmarks;
}
#endif //PRAKTIKUM_ALT_LANDMARKS_HH
