#ifndef TREENODE_H
#define TREENODE_H
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef enum {  RED = 0,
                BLACK,
                DOUBLE,
                UNDEFINED } t_color;

class treeNode
{

public:

    int ID;
    int count;
    t_color      color;
    treeNode *   parent;
    treeNode *   lChild;
    treeNode *   rChild;

    treeNode( int , int , t_color, treeNode* , treeNode* , treeNode* );
};

#endif // TREENODE_H
