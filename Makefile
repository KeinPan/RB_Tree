all:
	g++ -Wall -g -c ./project/treenode.cpp
	g++ -Wall -g -c ./project/rbtree.cpp
	g++ -Wall -g -c ./project/main.cpp
	g++ -g treenode.o rbtree.o main.o -lm -o bbst

cl:
	rm -f *.o bbst
