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

#define TIMESTEP 0.01
#define GRAVITY  10.0

//maybe make these structs?
class edge
{
    public:
        float k;                    //spring constant
        float damping_rate;         //damping rate
        float base_length;          //base distance 

        int node1;                  //first node involved
        int node2;                  //second node involved
};

class node
{
    public:
        float mass;                 //value of the mass
        glm::vec3 position;         //location of the node
        glm::vec3 velocity;         //velocity of the node

        glm::vec3 old_position;     //previous value of the node's position
        glm::vec3 old_velocity;     //previous value of the node's velocity

        bool anchor;                //is this node an anchored node

        std::vector<edge> edges;    //the edges that will impart forces on this node
};

class graph
{
    public:
        void add_node(float mass, glm::vec3 position, bool anchor);    //add a node to the list
        void add_edge(float k, float damping, int node1, int node2);   //add an edge between two nodes
        
        void update();              //updates all the position/velocity values
        
    private:
    //has a list of nodes - list of edges is held by each node, to have a vector of edges is redundant
        std::vector<node> nodes;
};

#endif
