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
  // NOTE: always must alloc as a power of 2 so route addressing routine can work
  // these shoud be typecast in and out of JoshTree
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
  void *cur_node = &a->root;
  do {
    // if cur_node is NULL, we are allocating a new path in the JoshuaTree
    if (cur_node == NULL) {
      int size_node = sizeof(int) << a->chunksize;
      struct JoshuaTree *tmp_node = malloc(sizeof(JoshuaTree));
      tmp_node->data = (int *)malloc(size_node);
      tmp_node->Tree[0] = (JoshuaTree *)malloc(sizeof(JoshuaTree));
      tmp_node->Tree[1] = (JoshuaTree *)malloc(sizeof(JoshuaTree));
      cur_node = (void *)tmp_node->Tree[path.outer_index & 1];
      path.outer_index >>= 1;
    } else {
      // now get the next node with a typecast
      // TODO: this cast is redundant with the above, does this incur runtime
      // penalty?
      struct JoshuaTree *tmp_node = (struct JoshuaTree *)cur_node;
      struct JoshuaTree *child_node = tmp_node->Tree[path.outer_index & 1];
      cur_node = child_node;
      path.outer_index >>= 1;
      // cur_node = (void *)(tmp_node->Tree[path.outer_index & 1]);
      //->Tree[path.outer_index & 1];
    }
  } while (path.outer_index != 1 && path.inner_index != 0);
  //  now insert into cur_node at inner_index
  struct JoshuaTree *tmp_node = cur_node;
  int *tmp_data = &tmp_node->data;
  tmp_data[path.inner_index] = data;
  return;
}

// TODO: get, delete, get_chunk, delete_chunk

int main(int argc, char **argv) {
  time_t t = time(NULL);
  StitchedArray test_array = {.chunksize = 2,
                              .root = {.Tree = {NULL, NULL}, .data = NULL}};
  printf("%d\n", 1 << test_array.chunksize);
  insert(&test_array, 10, 1);
  printf("done");
  // print the time
  printf("%d\n", (int)(time(NULL)) - (int)(t));
}
