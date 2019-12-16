//
// Created by eric on 19-7-24.
//

#ifndef DRONE_GRAPH_H
#define DRONE_GRAPH_H

#include "../utils/utils.h"

struct Edge
{
    int u, next;
    double v;

    Edge(int _u, int _next, double _v) : u(_u), next(_next), v(_v)
    {

    }
};

class Graph
{
public:
    void addEdge(int x, int u, int v);

    /**
     *
     * @param src
     * @param dst
     * @return Path represented with vertex ids
     */
    std::vector<int> sovleSSSP(int src, int dst);

private:
    std::vector<Edge> m_edges;
    std::vector<int> m_head;

};


#endif //DRONE_GRAPH_H
