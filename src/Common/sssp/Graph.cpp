//
// Created by eric on 19-7-24.
//

#include "Graph.h"
#include <queue>
using namespace std;

void Graph::addEdge(int x, int u, int v)
{
    m_edges.emplace_back(u, v, m_head[x]);
    m_head[x] = m_edges.size() - 1;
    m_edges.emplace_back(x, v, m_head[u]);
    m_head[u] = m_edges.size() - 1;
}

std::vector<int> Graph::sovleSSSP(int src, int dst)
{
//    const double INF = 1e100;
//    std::vector<int> parents(m_head.size(), -1);
//    std::vector<double> distance(m_head.size(), INF);
//    std::vector<bool> done(m_head.size(), false);
//    auto path = std::vector<int>();
//
//    priority_queue<pair<double, int>> Q;
//    Q.emplace(0, src);
//    distance[src] = 0;
//
//    while(!Q.empty()) {
//        auto x = Q.top(); Q.pop();
//        int c = x.second;
//        if (done[c])
//        {
//            continue;//如果这个点已经被提出过了，直接抛弃（适用于那种松弛之后重复放入队列的点）
//        }
//        done[c] = true;
//        if (c == dst)
//        {
//            break;
//        }
//        for (int i = m_head[x]; i; i = m_edges[m_head[i]].next)
//        {
//            Edge &e = m_edges[i];
//            if (e.v + distance[c] < distance[e.u] && distance[c] < INF)
//            {
//                distance[e.u] = e.v + distance[c];
//                parents[e.u] = c;
//                Q.emplace(distance[e.u], e.v);//把松弛过后点的d值重新放入队列
//            }
//        }
//    }
//
//    if (distance[dst] >= INF)
//        return path;
//
//    for (int c = dst; c != src; c = parents[c])
//    {
//        path.push_back(c);
//    }
//    path.push_back(src);
//    reverse(path.begin(), path.end());
//    return path;
}

