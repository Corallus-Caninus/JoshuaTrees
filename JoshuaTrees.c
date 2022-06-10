// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char uint;

typedef struct JoshuaTree {
  // points to heap
  void *Tree[2];
  // NOTE: always must alloc as a power of 2 so route addressing routine can
  // work these shoud be typecast in and out of JoshTree
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
  JoshuaTree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  while (true) {
    void *child = cur_node->Tree[path.outer_index & 1];
    if (!child) {
      printf("allocating new node");
      int size_node = sizeof(int) << a->chunksize;
      JoshuaTree *tmp_node = (JoshuaTree *)malloc(sizeof(JoshuaTree));
      tmp_node->data = (int *)malloc(size_node);
      tmp_node->Tree[0] = (void *)malloc(sizeof(JoshuaTree));
      tmp_node->Tree[1] = (void *)malloc(sizeof(JoshuaTree));
      //  assign tmp_node to child
      cur_node->Tree[path.outer_index & 1] = tmp_node;
      child = cur_node->Tree[path.outer_index & 1];
      path.outer_index >>= 1;
      cur_node = tmp_node;
    } else {
      path.outer_index >>= 1;
      cur_node = (JoshuaTree *)child;
    }
    if (path.outer_index != 1 && path.outer_index != 0) {
      break;
    }
  }
  //  now insert into cur_node at inner_index
  //TODO: verify data integrity
  int *tmp_array = cur_node->data;
  tmp_array[path.inner_index] = data;
}

// TODO: get, delete, get_chunk, delete_chunk

int main(int argc, char **argv) {
  time_t t = time(NULL);
  int size_node = sizeof(int) << 2;
  StitchedArray test_array = {
      .chunksize = 2,
      .root = {.Tree = {NULL, NULL}, .data = (int *)malloc(size_node)}};
  printf("%d\n", 1 << test_array.chunksize);
  // TODO: dropping references here
  insert(&test_array, 10, 1);
  printf("done");
  insert(&test_array, 11, 1);
  printf("done");
  insert(&test_array, 11, 1);
  // print the time
  printf("%d\n", (int)(time(NULL)) - (int)(t));
}
