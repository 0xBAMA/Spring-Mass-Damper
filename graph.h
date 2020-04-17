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

#define TIMESTEP 0.01f
#define GRAVITY  10.0f

struct edge
{
    float k;                    //spring constant
    float damping_rate;         //damping rate
    //float base_length;          //base distance - may want to implement this compressible springs 

    int node1;                  //first node involved
    int node2;                  //second node involved
};

struct node
{
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
        
        void update(int mousex, int mousey, bool clicked);             //updates all the position/velocity values
        
        void draw_links(SDL_Renderer* r);
        void draw_nodes(SDL_Renderer* r);
        void draw_mouse_link(SDL_Renderer* r, int mousex, int mousey, bool mouseclicked);

    private:
    //has a list of nodes - list of edges is held by each node, and redundantly here, for drawing purposes
        std::vector<node> nodes;
        std::vector<edge> edges;
};


void graph::add_node(float mass, glm::vec3 position, bool anchor)
{
    node temp;
    temp.mass = mass;
    temp.position = position;
    temp.velocity = glm::vec3(0,0,0);
    temp.anchor = anchor;

    nodes.push_back(temp);
}

void graph::add_edge(float k, float damping, int node1, int node2)
{
    edge temp1, temp2;

    //construct the edge
    temp1.k = k;
    temp1.damping_rate = damping;
    temp1.node1 = node1;
    temp1.node2 = node2;

    //same as above, but the nodes are switched
    temp2.k = k;
    temp2.damping_rate = damping;
    temp2.node1 = node2;
    temp2.node2 = node1;

    //base lengths are the same, just the distance between the 
    //temp1.base_length = temp2.base_length = glm::distance(nodes[node1].position, nodes[node2].position); 

    //put the edge in the graph's list of edges
    edges.push_back(temp1);

    //put the edge in each node's list of edges
    nodes[node1].edges.push_back(temp1);
    nodes[node2].edges.push_back(temp2);

}

void graph::draw_links(SDL_Renderer* r)
{
    glm::vec3 point1, point2;
    SDL_SetRenderDrawColor(r, 0xff, 0, 0, 0xff);   
    for(uint i = 0; i < edges.size(); i++)
    {
        point1 = nodes[edges[i].node1].position;
        point2 = nodes[edges[i].node2].position;

        SDL_RenderDrawLine(r, point1.x, point1.y, point2.x, point2.y);
    }
}

void graph::draw_nodes(SDL_Renderer* r)
{
    glm::vec3 point;
    SDL_SetRenderDrawColor(r, 0x0,0xf0,0xf0,0xff);
    SDL_Rect rect;

    for(uint i = 0; i < nodes.size(); i++)
    {
        point = nodes[i].position;
        
        rect.x = int(point.x-5);
        rect.y = int(point.y-5);
        rect.w = 10;
        rect.h = 10; // the rectangle
        
       SDL_RenderFillRect(r, &rect);
    }
}

void graph::draw_mouse_link(SDL_Renderer* r, int mousex, int mousey, bool mouseclicked)
{
    glm::vec3 closest_point;
    float closest_point_dist = 1000;

    for(uint i = 0; i < nodes.size(); i++)
    {
        float dist = glm::distance(nodes[i].position, glm::vec3(mousex,mousey,0));

        if(!nodes[i].anchor && dist < closest_point_dist)
        {
            closest_point_dist = dist;
            closest_point = nodes[i].position;
        }
    }

    //we've got the closest point
    if(mouseclicked)
        SDL_SetRenderDrawColor(r, 0xff, 0x89, 0x23, 0xff);
    else
        SDL_SetRenderDrawColor(r, 0xd3, 0xd9, 0x23, 0xff);

    SDL_RenderDrawLine(r, closest_point.x, closest_point.y, mousex, mousey);
}

void graph::update(int mousex, int mousey, bool clicked)
{
    //back up previous values of position and velocity
    for(uint i = 0; i < nodes.size(); i++)
    {
        nodes[i].old_position = nodes[i].position;
        nodes[i].old_velocity = nodes[i].velocity;
    }

    for(uint i = 0; i < nodes.size(); i++)
    {
        if(!nodes[i].anchor)
        {

            glm::vec3 force; //holds total force that will accelerate the current node
            for(uint j = 0; j < nodes[i].edges.size(); j++) //loop through connections
            {
                float k = nodes[i].edges[j].k;
                float d = nodes[i].edges[j].damping_rate;

                float my_mass = nodes[i].mass;

                glm::vec3 my_position = nodes[i].old_position;
                glm::vec3 ur_position = nodes[nodes[i].edges[j].node2].old_position;

                glm::vec3 my_velocity = nodes[i].old_velocity; 

                glm::vec3 springforce = -k * (my_position - ur_position);
                glm::vec3 dampforce = d * my_velocity;
                glm::vec3 gravforce = my_mass * glm::vec3(0,GRAVITY,0);

                force += (springforce + gravforce - dampforce);
            }

            //compute acceleration that will come from this force, f=ma
            glm::vec3 acceleration = force / nodes[i].mass;
            //update velocity based on this computed acceleration
            nodes[i].velocity = nodes[i].old_velocity + acceleration * TIMESTEP;
            //update position based on this new value of velocity
            nodes[i].position = nodes[i].old_position + nodes[i].velocity * TIMESTEP;
        }
    }
}

#endif
