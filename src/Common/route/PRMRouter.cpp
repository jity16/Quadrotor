#include <queue>
#include <iostream>
#include "PRMRouter.h"
#include "../utils/utils.h"

const int SCATTER_POINT_NUM = Config::Instance()->getIntParam("PRMRouter", "SCATTER_POINT_NUM");
float d[1005];
int pNode[1005];

std::vector<Point> PRMRouter::route(const Point &start_pos, const Point &end_pos)
{
    std::vector<Point> nodes{};
    nodes.emplace_back(start_pos);
    for (int i = 0; i < SCATTER_POINT_NUM; i++)
    {
        nodes.emplace_back(utils::generateRandomPointBetween(start_pos, end_pos));
    }
    nodes.emplace_back(end_pos);
    auto n = nodes.size();
    auto s = 0, t = (int) n - 1;
    for (int i = 0; i < n; i++)
    {
        d[i] = 1000000000;
        //std::cout << i << ' ' << nodes[i] << std::endl;
    }
    std::priority_queue<std::pair<float, int> > q;
    d[s] = 0;
    q.push(std::make_pair(0, s));
    auto cnt = 0;
    while (!q.empty())
    {
        int x = q.top().second;
        q.pop();
        cnt++;
        if (cnt == n || x == t)break;
        //std::cout << x << ' ' << d[x] << ' ' << pNode[x] << std::endl;
        for (int y = 0; y < n; y++)
        {
            if (y != x &&
                !utils::existCollisionBetweenBaseAndObstacles(nodes[y], m_obstacles) &&
                !utils::existCollisionBetweenBaselineAndObstacles(nodes[x], nodes[y], m_obstacles) &&
                d[x] + distance(nodes[x], nodes[y]) < d[y])
            {
                d[y] = d[x] + distance(nodes[x], nodes[y]);
                pNode[y] = x;
                //std::cout << x << ' ' << y << ' ' << d[x] << ' ' << d[y] << std::endl;
                q.push(std::make_pair(d[y], y));
            }
        }
    }
    auto ret = std::vector<Point>{};
    while (t)
    {
        ret.emplace_back(nodes[t]);
        std::cout << t << std::endl;
        t = pNode[t];
    }
    ret.emplace_back(nodes[s]);
    std::cout << s << std::endl;
    std::reverse(ret.begin(), ret.end());
    return ret;
}
