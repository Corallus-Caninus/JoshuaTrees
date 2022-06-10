//JoshuaTrees are BST-arrays 
//you get the best of both worlds
#include <stdio.h>
#include <stdbool.h>
typedef unsigned char uint;

typedef struct JoshTree{
  // points to heap
  void* Tree[2];
  void* data;
} JoshTree;
int add_two(int a, int b){
  return a+b;
}

int main(int argc, char **argv){
  JoshTree a = {.Tree = {1,2}};
  void (*func_type)(int, int) = &add_two;
  printf("Hello World %d : %d\n", a.Tree[0], a.Tree[1]);
  uint test = 2;
  test <<= 1;
  printf("%d -> %d\n", sizeof(test), test);
  void* mem = malloc(sizeof(JoshTree));
  free(mem);
  printf("%d\n", func_type);
  printf("%d\n", func_type(1,2));
}