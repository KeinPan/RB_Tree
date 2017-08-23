#ifndef RBTREE_H
#define RBTREE_H

#include "treenode.h"
#include <vector>
#include <iostream>

class RBTree
{
public:
/*----------------Member Var------------------*/
    treeNode * root;
    treeNode * NIL;  // A pointer to the NIL node for an RB tree
    int N;  // Number of treeNodes

/*----------------Constructor------------------*/
    RBTree();
    treeNode* BuildSubTree( vector<treeNode*> & , int , int , treeNode* , int );
    void initialize( vector<treeNode *> & );

/*----------------Unit_Funcs------------------*/
    void Left_Rotate ( treeNode*);
    void Right_Rotate( treeNode*);

    treeNode* Min ( treeNode* );
    treeNode* Max ( treeNode* );
    treeNode* Pred( treeNode* );
    treeNode* Succ( treeNode* );
    void Transplant( treeNode*, treeNode* );

/*----------------Basic_Funcs------------------*/
    treeNode* Search( int );

    void Insert( treeNode *);
    void Insert_Fixup( treeNode *);

    void Delete( treeNode *);
    void Delete_Fixup( treeNode *);

/*---------------Advanced_Funcs-----------------*/
    void Increase( int , int );
    void Reduce( int, int );
    void Count (int );
    int  Report( treeNode* );
    int  Inrange( int , int );
    void Next( int );
    void Previous( int );
};

void preorder(RBTree *, treeNode * );
#endif // RBTREE_H
