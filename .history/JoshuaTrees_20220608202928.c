// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>

typedef unsigned char uint;

typedef struct JoshuaTree {
  // points to heap
  JoshuaTree *Tree[2];
  //NOTE: always must alloc as a power of 2
  int *data;
} JoshuaTree;
typedef struct StitchedArray {
  int chunksize;
  JoshuaTree root;
} StitchedArray;

// TODO: int size should vary here for different architectures and addressing
typedef struct route_t {
  int outer_index;
  int inner_index;
} route_t;
static inline route_t route(StitchedArray root, int index) {
  int outer_index = (index >> root.chunksize) + 2;
  int inner_index = index & ((1 << root.chunksize) - 1);
  return (route_t){outer_index, inner_index};
}

void insert(StitchedArray *a, int index, int data) {
  route_t path = route(*a, index);
  void *cur_node = a->root.Tree[path.outer_index];
  // TODO: get rid of this cmp
  while (path.outer_index != 0) {
    cur_node = a->root.Tree[path.outer_index];
    path.outer_index >>= 1;
    //if cur_node is NULL, create it here
    if (cur_node == NULL) {
      //cur_node is JoshuaTree
      cur_node = malloc(sizeof(JoshuaTree));
      cur_node->data = malloc(1 << a->chunksize);
      a->root.Tree[path.outer_index] = cur_node;
    }
    // will throw here if unallocated
  }
  // now insert into cur_node at inner_index
  a->root.data[path.inner_index] = data;
}

int main(int argc, char **argv) {
  int data[2] = {1, 2};

  // printf("Hello World %d : %d -> %d\n", a.Tree[0], a.Tree[1], a.data);
  uint test = 2;
  test <<= 1;
  printf("%d -> %d\n", sizeof(test), test);

  void *mem = malloc(sizeof(route_t));
  free(mem);
}

// int add_two(int a, int b) { return a + b; }
// int (*func_type)(int, int) = &add_two;