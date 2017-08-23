#include "treenode.h"
#include <iostream>


treeNode::treeNode(int a, int b, t_color c,
                   treeNode * p, treeNode * c1,treeNode * c2):
                   ID(a), count(b), color(c),
                   parent(p), lChild(c1), rChild(c2)
    {}
