// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>

typedef unsigned char uint;

typedef struct JoshuaTree
{
  // points to heap
  void *Tree[2];
  void *data;
} JoshuaTree;
typedef struct StitchedArray{
  int chunksize;
  JoshuaTree root;
} StitchedArray;


//TODO: int size should vary here for different architectures and addressing
typedef struct route_t{
  int outer_index;
  int inner_index;
} route_t;
route_t route(StitchedArray root, int index){
  int outer_index = (index >> root.chunksize) +2;
  int inner_index = index & ((1<<root.chunksize)-1);
  return (route_t){outer_index, inner_index};
}


int add_two(int a, int b) { return a + b; }
int (*func_type)(int, int) = &add_two;

int main(int argc, char **argv)
{
  int data[2] = {1, 2};

  printf("Hello World %d : %d -> %d\n", a.Tree[0], a.Tree[1], a.data);
  uint test = 2;
  test <<= 1;
  printf("%d -> %d\n", sizeof(test), test);

  free(mem);
  printf("%d\n", func_type);
  // print size of func_type
  printf("%d\n", sizeof(func_type));

  // int res = (*func_type)(1,2);
  int res = func_type(1, 2);
  printf("%d\n", res);
}