//JoshuaTrees are BST-arrays 
//you get the best of both worlds
#include <stdio.h>

typedef struct JoshTree{
  int Tree[2];
} JoshTree;

int main(int argc, char **argv){
  JoshTree a = {.Tree = {1,2}};
  printf("Hello World %d : \n", a.Tree[0]);
}