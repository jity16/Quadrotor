//
// Created by eric on 19-7-22.
//

#ifndef DRONE_TREENODE_H
#define DRONE_TREENODE_H

#include "../utils/utils.h"

struct TreeNode{
    TreeNode(Point _pos, int _parent);
    Point pos;
    int parent;
    std::vector<int> child;
};


#endif //DRONE_TREENODE_H
