#include "rbtree.h"

/*----------------Constructor------------------*/
RBTree::RBTree()
{
   NIL =  new treeNode(-1,0, BLACK, NULL, NULL, NULL);
   root = NIL;
   N = 0;
   NIL->lChild = NIL;
   NIL->rChild = NIL;
   NIL->parent = NIL;
}

// Building a subtree using elements v[n]~v[m],using level to record current height
treeNode* RBTree::BuildSubTree(vector<treeNode *> & v, int n, int m, treeNode * p, int level)
{
    if(n > m) std::cerr<< "Boundary Violated!! " <<std::endl;
    int median = 0;
    treeNode *  q = NULL;
    switch(m-n)
    {
        case 0:         //unit node
            q = v[n];
            q->color  = ( pow(2,level+1)-1 > N)? RED : BLACK;
            q->lChild = q->rChild = this->NIL;
            q->parent = p;
            return q;

        case 1:
            q = v[m];
            q->color  = BLACK;
            q->lChild = BuildSubTree(v,n,n,q,level+1);;
            q->rChild = this->NIL;
            q->parent = p;
            return q;

        default:
            median = (n+m)/2;
            q = v[median];
            q->parent = p;
            q->color  = BLACK;
            q->lChild = BuildSubTree(v,n,median-1,q,level+1);
            q->rChild = BuildSubTree(v,median+1,m,q,level+1);
            return q;
    }
}

// Initialization
void RBTree::initialize(vector<treeNode *> & v)
{
    N = v.size();               // Firstly, record the number of nodes

    int n = v.size();
    if(0 == n) return;          // Invalid Initialization

    int median     = n/2;
    this->root   = v[median];   //set current root
    root->color  = BLACK;
    root->parent = NIL;

    if(1 == n)                  //Initialized with only 1 node
    {
        root->lChild = root->rChild = this->NIL;
        return;
    }

    root->lChild = BuildSubTree(v,0 ,median-1,root,1);

    if(2 == n)
    {
        root->rChild = this->NIL;
    }
    else
    {
        root->rChild = BuildSubTree(v,median+1,n-1,root,1);
    }
    return;
}


/*------------------Rotation-------------------*/
void RBTree::Left_Rotate( treeNode *x)
{
    treeNode* y   = x->rChild;
    x->rChild     = y->lChild;
    if(y->lChild != this->NIL)
        y->lChild->parent = x;

    y->parent     = x->parent;

    if(x->parent == this->NIL)
        this->root= y;

    else if(x == x->parent->lChild)
        x->parent->lChild = y;
    else
        x->parent->rChild = y;

    y->lChild = x;
    x->parent = y;
    return;
}

void RBTree::Right_Rotate( treeNode *x)
{
    treeNode* y   = x->lChild;
    x->lChild     = y->rChild;
    if(y->rChild != this->NIL)
        y->rChild->parent = x;

    y->parent     = x->parent;

    if(x->parent == this->NIL)
        this->root = y;

    else if(x == x->parent->rChild)
        x->parent->rChild = y;
    else
        x->parent->lChild = y;

    y->rChild = x;
    x->parent = y;
    return;
}


/*------------------Helpers--------------------*/
treeNode* RBTree::Min(treeNode* x)
{
    while(x->lChild != NIL)
        x = x->lChild;
    return x;
}

treeNode* RBTree::Max(treeNode* x)
{
    while(x->rChild != NIL)
        x = x->rChild;
    return x;
}

treeNode* RBTree::Pred( treeNode * x)
{
    if(x->lChild != this->NIL)
        return(Max(x->lChild));
    treeNode* y = x->parent;
    while(y!= NIL && x == (y->lChild))
    {
        x = y;
        y = x->parent;
    }
    return y;
}

treeNode* RBTree::Succ( treeNode * x)
{
    if(x->rChild != this->NIL)
        return(Min(x->rChild));
    treeNode*y = x->parent;
    while(y!= NIL && x == (y->rChild))
    {
        x = y;
        y = x->parent;
    }
    return y;
}

// replacing a subtree rooted by u with another subtree rooted by v
void RBTree::Transplant( treeNode* u, treeNode* v)
{
    if(u->parent == this->NIL)
        this->root = v;
    else if(u == u->parent->lChild)
        u->parent->lChild = v;
    else
        u->parent->rChild = v;
    v->parent = u->parent;
    return;
}


/*---------------Basic function----------------*/

/* Given an ID(not count), this will return the location of a node with the exact value in
 * the tree..ec, the pointer. If No concruent node found,  NIL node would be returned*/
treeNode* RBTree::Search(int key)
{
    treeNode* location  = this->root;
    while( (location != this->NIL) && (key != location->ID))
    {
        location = (key < location->ID)? location->lChild : location->rChild;
    }
    return location;
}


/*---------------Insert & Delete----------------*/
/* Codes for this section are similar to that in CLRS, since I've taken COT5405
 * class last semester and do remember most procedure of these 2 fundamental op.
 * Coding works were done by myself, but also modified with the standard pseudocode
 * shown on textbook line by line, in order to reduce the possibility of mistakes
 * taken place afterwards                       */

void RBTree::Insert( treeNode * z)
{
    treeNode* y = this->NIL;    // to record the parent
    treeNode* x = this->root;   // as index

    while( x != this->NIL)
    {
        y = x;
        x = (z->ID < x->ID)? x->lChild:x->rChild;
    }

    z->parent = y;
                                // set parent's pointer
    if(y == this->NIL)  this->root = z;
    else if(z->ID<y->ID) y->lChild = z;
    else y->rChild = z;

                                // color setted as red, no children
    z->lChild = z->rChild = this->NIL;
    z->color = RED;
    Insert_Fixup(z);            // Then fix properties
    N++;
}

void RBTree::Insert_Fixup( treeNode * z)
{
    treeNode* y = NULL;
    while(RED == z->parent->color)                  // rule 4 violated
    {
        if(z->parent == z->parent->parent->lChild)  // father is grandfa's leftchild
        {
            y = z->parent->parent->rChild;          // y is uncle
            if(RED == y->color)                     // red uncle
            {
                z->parent->color         = BLACK;
                y->color                 = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else if(z == z->parent->rChild)         // black uncle
            {
                z = z->parent;
                Left_Rotate(z);                     // case2->case3
            }
            else
            {                                       // case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Right_Rotate(z->parent->parent);
            }
        }
        else                                        // father is grandfa's rightchild
        {
            y = z->parent->parent->lChild;          // y is uncle
            if(RED == y->color)                     // red uncle
            {
                z->parent->color         = BLACK;
                y->color                 = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else if(z == z->parent->lChild)         // black uncle
            {
                z = z->parent;
                Right_Rotate(z);                    // case2->case3
            }
            else
            {                                       // case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                Left_Rotate(z->parent->parent);
            }
        }
    }
    this->root->color = BLACK;                      // set root
    return;
}


/* Must receive a treeNode which pointing to an existing node in the RBTree.
 * If not, the situation of z's parent and siblings are unpredictable, huge
 * mistakes would be caused. Thus, better call Search(key) to return a result
 * before call this function. Also, deleting from an empty tree is not allowed*/
void RBTree::Delete( treeNode * z)
{
   if(0 == N)
   {
       cerr<< "WARNING: Try to delete from an empty tree!!" <<endl;
       exit(1);
   }
   treeNode* y = z;
   treeNode* x = NULL;
   t_color original_y = y->color;

   if(z->lChild == NIL)             //No children
   {
       x = z->rChild;
       Transplant(z,z->rChild);
   }
   else if(z->rChild == NIL)        //Has Left child
   {
       x = z->lChild;
       Transplant(z,z->lChild);
   }
   else                             //Has right child
   {
       y = Min(z->rChild);          //Swap with its successor
       original_y = y->color;
       x = y->rChild;
       if(y->parent == z)
        x->parent = y;
       else
       {
           Transplant(y,y->rChild);
           y->rChild = z->rChild;
           y->rChild->parent = y;
       }
       Transplant(z,y);
       y->lChild = z->lChild;
       y->lChild->parent = y;
       y->color = z->color;
   }
                                    // If it is red, nothing to be done
   if(original_y == BLACK)
       Delete_Fixup(x);
   N--;
}

void RBTree::Delete_Fixup( treeNode * x)
{
    treeNode* w;                    // Uncle
    while( (x != this->root) && (x->color == BLACK) )
    {
        if(x == x->parent->lChild)  // x is leftchild
        {
            w = x->parent->rChild;
            if(w->color == RED)     // case 1
            {
                w->color = BLACK;
                x->parent->color = RED;
                Left_Rotate(x->parent);
                w = x->parent->rChild;
            }
                                    // case 2
            if( (w->lChild->color == BLACK) && (w->rChild->color == BLACK) )
            {
                w->color = RED;
                x = x->parent;
            }
                                    // case 3
            else if(w->rChild->color == BLACK)
            {
                w->lChild->color = BLACK;
                w->color = RED;
                Right_Rotate(w);
                w = x->parent->rChild;
            }
                                    // all could be reduced to case 4
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->rChild->color = BLACK;
            Left_Rotate(x->parent);
            x = this->root;         // Jump out
        }
        else                        // same but l&r exchanged
        {
            w = x->parent->lChild;
            if(w->color == RED)     // case 1
            {
                w->color = BLACK;
                x->parent->color = RED;
                Right_Rotate(x->parent);
                w = x->parent->lChild;
            }
                                    // case 2
            if( (w->rChild->color == BLACK) && (w->lChild->color == BLACK) )
            {
                w->color = RED;
                x = x->parent;
            }
                                    // case 3
            else if(w->lChild->color == BLACK)
            {
                w->rChild->color = BLACK;
                w->color = RED;
                Left_Rotate(w);
                w = x->parent->lChild;
            }
                                    // all could be reduced to case 4
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->lChild->color = BLACK;
            Right_Rotate(x->parent);
            x = this->root;
        }
    }
    x->color = BLACK;
}


/*-------------Advanced Operations--------------*/
void RBTree::Increase( int theID, int m)
{
    treeNode* p = this->Search(theID);
    if( p != NIL)   p->count += m;
    else
    {
         p = new treeNode(theID,m,UNDEFINED,NULL,NULL,NULL);
         this->Insert(p);
    }
    cout<< p->count <<endl;
    return;
}

void RBTree::Reduce(int theID, int m)
{
     treeNode* p = this->Search(theID);
     if( p == NIL )
     {
         cout<< 0 <<endl;
         return;
     }
    if(p->count > m)            // Still positive
    {
        p->count -= m;
        cout<< p->count <<endl;
    }
    else                        // Need to delete this node
    {
        this->Delete(p);
        cout<< 0 <<endl;
    }
    return;
}

void RBTree::Count(int theID)
{
    treeNode* p = Search(theID);
    if( p != NIL ) cout<< p->count <<endl;
    else           cout<<    0     <<endl;
    return;
}

// Note: ID dosen't need to be existed
void RBTree::Previous(int theID)
{
    treeNode* q = NULL;
    treeNode* p = Search(theID);
    if( p!= NIL)
    {
        p = Pred(p);
        if( p!= NIL) cout<<p->ID<<" "<<p->count<<endl;
        else         cout<< 0 << " " << 0 <<endl;
        return;
    }
    p = new treeNode(theID, 0 ,UNDEFINED, NULL,NULL,NULL);
    this->Insert(p);
    q = Pred(p);
    if( q!= NIL) cout<<q->ID<<" "<<q->count<<endl;
    else         cout<< 0 << " " << 0 <<endl;
    this->Delete(p);
    return;
}

// Note: ID dosen't need to be existed
void RBTree::Next(int theID)
{
    treeNode* q = NULL;
    treeNode* p = Search(theID);
    if( p!= NIL)
    {
        p = Succ(p);
        if( p!= NIL) cout<<p->ID<<" "<<p->count<<endl;
        else         cout<< 0 << " " << 0 <<endl;
        return;
    }
    p = new treeNode(theID, 0 ,UNDEFINED, NULL,NULL,NULL);
    this->Insert(p);
    q = Succ(p);
    if( q!= NIL) cout<<q->ID<<" "<<q->count<<endl;
    else         cout<< 0 << " " << 0 <<endl;
    this->Delete(p);
    return;
}

// Compute the total counts of a subtree rooted by p
int RBTree::Report(treeNode* p)
{
    if( p == NIL) return 0;
    else
    return (p->count + Report(p->lChild) + Report(p->rChild) );
}

int RBTree::Inrange(int ID1, int ID2)  // 1-D range tree
{
    int total = 0;
    if(ID1 > ID2)
    {
        cout<< "Left Boundary should be NO BIGGER than Right Boundary! " <<endl;
        return 0;
    }
    treeNode* p = this->root;

    while( p != NIL)                    // Find the Split Node
    {
        if( p->ID < ID1) p = p->rChild;
        else if (p->ID > ID2) p = p->lChild;
        else break;
    }
    //cout << p->ID <<endl;
    if (( p->lChild == NIL )&&( p->rChild == NIL ))                      // It is a leaf
    {
        if( (p->ID >= ID1) && (p->ID <= ID2) )                           // Check whether reporting this leaf
        {
            return( p->count );
        }
    }
    else                                // Not a leaf, possess subtrees
    {
        if( (p->ID >= ID1) && (p->ID <= ID2) )
        {
            total += p->count;              // whether Add the split Node
        }

        treeNode* pl = p->lChild;       // Left Path
        while( pl != NIL )
        {
            if(ID1 <= pl->ID)
            {
                total += pl->count;
                total += Report(pl->rChild);
                pl = pl->lChild;
            }
            else
                pl = pl->rChild;
        }

        treeNode* pr = p->rChild;       // Right Path
        while( pr!= NIL )
        {
            if(ID2 >= pr->ID)
            {
                total += pr->count;
                total += Report(pr->lChild);
                pr = pr->rChild;
            }
            else
                pr = pr->lChild;
        }
    }
    return total;
}

/*--------------Printing the tree---------------*/
/* 2 parameters, one is the whole tree's pointer, one is the current root
 * node. Usually called as preorder( T, T->root) to print the whole tree
 * Note: This is NOT a member function*/

void preorder(RBTree *T, treeNode * r)
{
    if (NULL == T || NULL == r)
    {
        cerr<< " NULL PTR RECEIVED!!" <<endl;
        exit(1);
    }

    if (r == T->NIL)        //External node
    {
        cout<<endl;
        return;
    }

    cout <<"ID: "        << r->ID        <<endl
         <<"count: "     << r->count     <<endl
         <<"leftchild: " << r->lChild->ID<<endl
         <<"rightchild: "<< r->rChild->ID<<endl
         <<"parent: " ;
    (NULL == r->parent)?  cout<<"None"<<endl
                       :  cout<<r->parent->ID
                              <<endl;
    cout << "color : ";
    (r->color == RED)?    cout<< "red" <<endl
                     :    cout<<"black"<<endl
                              <<endl;

    preorder(T, r->lChild);      // letf  subtree
    preorder(T, r->rChild);      // right subtree

    return;
}

