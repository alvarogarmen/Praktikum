//
// Created by alvar on 26/04/2023.
//

#include <string>
#include "argtable3.h"

#include "readgraph.hh"
#include "Dijkstra.hh"
#include <iostream>


void processInput(double sourceNode, double targetNode, std::string graph) {        //This will call Dijkstra and read the files
    // Give out the input
    std::cout << "Source Node: " << sourceNode << std::endl;
    std::cout << "Target Node: " << targetNode << std::endl;
    std::cout<<graph<<std::endl;
    std::cout << "Graph1: " << graph << std::endl;
    // Call the read-functions
    Graph myGraph = readCoordFile(graph);
    std::cout<<"Reading successful"<<std::endl;
    readOtherFile(graph, myGraph);
    std::cout<<"Reading successful"<<std::endl;
    // Call Dijkstra
    double Dis = Dijkstra(myGraph, sourceNode, targetNode);
    // Give out the distance
    std::cout<<"Distance from Node "<<sourceNode<<" to Node "<< targetNode<<" is: "<<Dis;
}

int main(int argc, char* argv[]) {
    //Argtable 3
    double sourceNode;
    double targetNode;
    std::string graph;

    struct arg_dbl* sourceArg = arg_dbl0(NULL, "source", "<double>", "source node");
    struct arg_dbl* targetArg = arg_dbl0(NULL, "target", "<double>", "target node");
    struct arg_str* graphArg = arg_str0(NULL, "graph", "<string>", "file with the graph, keep the .graph!");

    struct arg_end* end = arg_end(20); // Define the end marker for the argtable array

    void* argtable[] = { sourceArg, targetArg, graphArg, end };

    const char* progname = "myprogram";
    int nerrors = arg_parse(argc, argv, argtable);

    if (nerrors > 0) {
        arg_print_errors(stdout, end, progname);
        arg_print_syntax(stdout, argtable, "\n");
        return 1; // Handle parsing errors
    }

    if (sourceArg->count > 0) {
        sourceNode = sourceArg->dval[0];
    }

    if (targetArg->count > 0) {
        targetNode = targetArg->dval[0];
    }

    if (graphArg->count > 0) {
        graph = graphArg->sval[0];
    }
    // Call out functions
    processInput(sourceNode, targetNode, graph);

    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));

    return 0;
}