#include <iostream>
#include <fstream>
#include "rbtree.h"
#include "stdlib.h"
using namespace std;


//temp, every element is a POINTER to a treenode
vector<treeNode *> input;

int main(int argc, char * const argv[])
{
    string command = "";
    int temp1 = -1;             // To record ID
    int temp2 = -1;             // To record count
    treeNode* t = NULL;
    RBTree*   T = new RBTree();

#if 1

//    ifstream inputFile("/home/ashmaycry/Downloads/test_1000000.txt");
    ifstream inputFile(argv[1]);
    if(!inputFile.is_open())
    {
        cerr<<"ERROR OPENING" <<endl;
        return -1;
    }

    inputFile >> T->N;          // set the total # of nodes
    while(inputFile >> temp1 >> temp2)
    {

        treeNode *t = new treeNode(temp1, temp2,UNDEFINED,NULL,NULL,NULL);
        input.push_back(t);
    }
    inputFile.close();
    T->initialize(input);
    //preorder(T,T->root);
    while(1)                    // repeating receiving commands from user
    {
        cin >> command;
        if("increase" == command)
        {
            cin >> temp1 >> temp2;
            T->Increase(temp1,temp2);
            temp1 = temp2 = 0;
            continue;
        }
        if("reduce"   == command)
        {
            cin >> temp1 >> temp2;
            T->Reduce(temp1,temp2);
            temp1 = temp2 = 0;
            continue;
        }
        if("count"    == command)
        {
            cin >> temp1;
            T->Count(temp1);
            temp1 = 0;
            continue;
        }
        if("inrange"  == command)
        {
            cin >> temp1 >> temp2;
            cout<< T->Inrange(temp1,temp2)<<endl;
            temp1 = temp2 = 0;
            continue;
        }
        if("previous" == command)
        {
            cin >> temp1;
            T->Previous(temp1);
            temp1 = 0;
            continue;
        }
        if("next"     == command)
        {
            cin >> temp1;
            T->Next(temp1);
            temp1 = 0;
            continue;
        }
        if("quit"     == command)
        {
   //        break;
           exit(0);
        }
    }
#endif

#if 0
    for(int i = 0; i != 10 ; ++i)
    {
        t = new treeNode(i*i, i , UNDEFINED, NULL, NULL, NULL)    ;
        input.push_back(t);
    }
    T->initialize(input);
    T->Previous(49);
    preorder(T,T->root);
#endif


    free(t);
    for(unsigned int i = 0; i< input.size() ;++i)
    {
        free(input[i]);
    }
    return 0;
}

