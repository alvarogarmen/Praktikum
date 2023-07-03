#ifndef PRAKTIKUM_ASTARBIDIRECTIONAL_HH
#define PRAKTIKUM_ASTARBIDIRECTIONAL_HH
#include "Graph.hh"
#include <climits>
#include "APQ.hh"
#include "Dijkstra.hh"
#include <cmath>
#include <set>
double AStarBidirectional(Graph& myGraph, double& sourceNode, double& targetNode, int secureBidirectional) {
    //Get two priority queues
    APQ apqForward = APQ();
    APQ apqBackward = APQ();
    std::vector<bool> visitedForward(myGraph.nodes.size(), false);
    std::vector<bool> visitedBackward(myGraph.nodes.size(), false);
    std::vector<double> distForward(myGraph.nodes.size(), INT_MAX);
    std::vector<double> distBackward(myGraph.nodes.size(), INT_MAX);
    double minPath = distance(myGraph.nodes[sourceNode-1], myGraph.nodes[targetNode-1])*3;
    apqForward.insertNode(sourceNode - 1, 0);
    apqBackward.insertNode(targetNode - 1, 0);
    distForward[sourceNode - 1] = 0;
    distBackward[targetNode - 1] = 0;

    double bestPath = INT_MAX;
    double meetingNode = -1;
    while (!apqForward.isEmpty() && !apqBackward.isEmpty()) {

        double forwardNode = apqForward.popMin();
        double backwardNode = apqBackward.popMin();


        visitedForward[forwardNode]=true;
        visitedBackward[backwardNode]=true;


        if (bestPath <= minPath){
            return bestPath;
        }
        if (distForward[forwardNode] + distBackward[forwardNode] < bestPath) {
            bestPath = distForward[forwardNode] + distBackward[forwardNode];
        }

        double startForwardEdge = (forwardNode > 0) ? myGraph.edgeStarts[forwardNode - 1] + 1 : 0;
        double endForwardEdge = myGraph.edgeStarts[forwardNode];

        double startBackwardEdge = (backwardNode > 0) ? myGraph.edgeStarts[backwardNode - 1] + 1 : 0;
        double endBackwardEdge = myGraph.edgeStarts[backwardNode];

        for (double forwardEdgeIndex = startForwardEdge; forwardEdgeIndex <= endForwardEdge; forwardEdgeIndex++) {
            double forwardEdge = myGraph.edges[forwardEdgeIndex] - 1;
            double forwardWeight = distance(myGraph.nodes[forwardNode], myGraph.nodes[forwardEdge]);

            if (distForward[forwardNode] + forwardWeight < distForward[forwardEdge]) {
                distForward[forwardEdge] = distForward[forwardNode] + forwardWeight;

                double forwardH = distance(myGraph.nodes[forwardEdge], myGraph.nodes[targetNode-1]);

                double forwardF = distForward[forwardEdge] + forwardH;
                if(visitedBackward[forwardEdge] && distForward[forwardEdge]+forwardWeight+distBackward[forwardEdge] < minPath){minPath=distForward[forwardEdge]+forwardWeight+distBackward[forwardEdge];}
                if (apqForward.contains(forwardEdge)) {
                    apqForward.decreaseKey(forwardEdge, forwardF);
                } else {
                    apqForward.insertNode(forwardEdge, forwardF);
                }
            }
        }

        for (double backwardEdgeIndex = startBackwardEdge; backwardEdgeIndex <= endBackwardEdge; backwardEdgeIndex++) {
            double backwardEdge = myGraph.edges[backwardEdgeIndex] - 1;
            double backwardWeight = distance(myGraph.nodes[backwardNode], myGraph.nodes[backwardEdge]);

            if (distBackward[backwardNode] + backwardWeight  < distBackward[backwardEdge] ) {
                distBackward[backwardEdge] = distBackward[backwardNode] + backwardWeight;

                double backwardH = distance(myGraph.nodes[backwardEdge], myGraph.nodes[sourceNode-1]);
                double backwardF = distBackward[backwardEdge] + backwardH;
                if(visitedForward[backwardEdge] && distForward[backwardEdge]+backwardWeight+distBackward[backwardEdge] < minPath){minPath=distForward[backwardEdge]+backwardWeight+distBackward[backwardEdge];}

                if (apqBackward.contains(backwardEdge)) {
                    apqBackward.decreaseKey(backwardEdge, backwardF);
                } else {
                    apqBackward.insertNode(backwardEdge, backwardF);
                }
            }
        }
    }

    return bestPath;
}

double AStarBidirectionalSaving(Graph& myGraph, double& sourceNode, double& targetNode, std::string exploredFileName, int secureBidirectional) {
    APQ apqForward = APQ();
    APQ apqBackward = APQ();
    std::vector<bool> visitedForward(myGraph.nodes.size(), false);
    std::vector<bool> visitedBackward(myGraph.nodes.size(), false);
    std::vector<double> distForward(myGraph.nodes.size(), INT_MAX);
    std::vector<double> distBackward(myGraph.nodes.size(), INT_MAX);
    std::vector<double> forwardPath(myGraph.nodes.size(), -1);
    std::vector<double> backwardPath(myGraph.nodes.size(), -1);
    double minPath = distance(myGraph.nodes[sourceNode-1], myGraph.nodes[targetNode-1])*3;
    apqForward.insertNode(sourceNode - 1, 0);
    apqBackward.insertNode(targetNode - 1, 0);
    distForward[sourceNode - 1] = 0;
    distBackward[targetNode - 1] = 0;

    double bestPath = INT_MAX;
    std::ofstream exploredNodeFile(exploredFileName);
    while (!apqForward.isEmpty() && !apqBackward.isEmpty()) {

        double forwardNode = apqForward.popMin();
        double backwardNode = apqBackward.popMin();

        visitedForward[forwardNode]=true;
        exploredNodeFile << myGraph.getNode(forwardNode).coordinateX << " " << myGraph.getNode(forwardNode).coordinateY << std::endl;  // Write explored node to the file

        visitedBackward[backwardNode]=true;
        exploredNodeFile << myGraph.getNode(backwardNode).coordinateX << " " << myGraph.getNode(backwardNode).coordinateY << std::endl;  // Write explored node to the file




        if (bestPath <= minPath){
            exploredNodeFile.close();
            return bestPath;
        }

        if (distForward[forwardNode] + distBackward[forwardNode] < bestPath) {
            bestPath = distForward[forwardNode] + distBackward[forwardNode];
        }

        double startForwardEdge = (forwardNode > 0) ? myGraph.edgeStarts[forwardNode - 1] + 1 : 0;
        double endForwardEdge = myGraph.edgeStarts[forwardNode];

        double startBackwardEdge = (backwardNode > 0) ? myGraph.edgeStarts[backwardNode - 1] + 1 : 0;
        double endBackwardEdge = myGraph.edgeStarts[backwardNode];

        for (double forwardEdgeIndex = startForwardEdge; forwardEdgeIndex <= endForwardEdge; forwardEdgeIndex++) {
            double forwardEdge = myGraph.edges[forwardEdgeIndex] - 1;
            double forwardWeight = distance(myGraph.nodes[forwardNode], myGraph.nodes[forwardEdge]);

            if (distForward[forwardNode] + forwardWeight < distForward[forwardEdge]) {
                distForward[forwardEdge] = distForward[forwardNode] + forwardWeight;
                forwardPath[forwardEdge] = forwardNode;

                double forwardH = distance(myGraph.nodes[forwardEdge], myGraph.nodes[targetNode-1]);
                double forwardF = distForward[forwardEdge] + forwardH;
                if(visitedBackward[forwardEdge] && forwardF+distBackward[forwardEdge] < minPath){minPath=distBackward[forwardEdge]+forwardF;}


                if (apqForward.contains(forwardEdge)) {
                    apqForward.decreaseKey(forwardEdge, forwardF);
                } else {
                    apqForward.insertNode(forwardEdge, forwardF);
                }
            }
        }

        for (double backwardEdgeIndex = startBackwardEdge; backwardEdgeIndex <= endBackwardEdge; backwardEdgeIndex++) {
            double backwardEdge = myGraph.edges[backwardEdgeIndex] - 1;
            double backwardWeight = distance(myGraph.nodes[backwardNode], myGraph.nodes[backwardEdge]);

            if (distBackward[backwardNode] + backwardWeight  < distBackward[backwardEdge] ) {
                distBackward[backwardEdge] = distBackward[backwardNode] + backwardWeight;

                double backwardH = distance(myGraph.nodes[backwardEdge], myGraph.nodes[sourceNode-1]);
                double backwardF = distBackward[backwardEdge] + backwardH;
                if(visitedForward[backwardEdge] && backwardF+distForward[backwardEdge] < minPath){minPath=distForward[backwardEdge]+backwardF;}


                if (apqBackward.contains(backwardEdge)) {
                    apqBackward.decreaseKey(backwardEdge, backwardF);
                } else {
                    apqBackward.insertNode(backwardEdge, backwardF);
                }
            }
        }
    }



    if (bestPath == INT_MAX) {
        return -1;
    }

    exploredNodeFile.close();

    return bestPath;
}



#endif