/*
 * =====================================================================================
 *
 *       Filename:  graph.h
 *
 *    Description:  This holds the class representing the nodes and edges of the graph
 *
 *        Created:  04/15/2020 10:33:30 PM
 *       Compiler:  g++
 *
 *         Author:  Jon Baker 
 *
 * =====================================================================================
 */
#ifndef GRAPH_H
#define GRAPH_H

#include "includes.h"

class edge
{
    public:
        float k;                //spring constant
        float damping_rate;     //damping rate
        float base_length;      //base distance 

        int node1;              //first node involved
        int node2;              //second node involved
};

class node
{
    public:
        float mass;             //value of the mass
        glm::vec3 position;     //location of the node
        glm::vec3 velocity;     //velocity of the node

        vector<edge> edges;     //the edges that will impart forces on this node
};

class graph
{
    //has a list of nodes, and a list of edges
};

#endif
