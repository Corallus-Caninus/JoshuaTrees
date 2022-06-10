//JoshuaTrees are BST-arrays 
//you get the best of both worlds
#include <stdio.h>
typedef unsigned int uint;

typedef struct JoshTree{
  uint Tree[2];
} JoshTree;

int main(int argc, char **argv){
  JoshTree a = {.Tree = {1,2}};
  printf("Hello World %d : %d\n", a.Tree[0], a.Tree[1]);
}