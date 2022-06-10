// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

typedef unsigned char uint;

typedef struct JoshuaTree {
  // points to heap
  void *Tree[2];
  // NOTE: always must alloc as a power of 2
  //these shoud be typecast in and out of JoshTree
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
  while (path.outer_index != 1 ||path.inner_index != 0) {

    if (path.outer_index &1== 1) {
      printf('left\n');
    } else {
      printf("right\n");
    }

    cur_node = a->root.Tree[path.outer_index];
    path.outer_index >>= 1;
    // if cur_node is NULL, we are allocating a new path in the JoshuaTree
    if (cur_node == NULL) {
      printf("allocating new path\n");
      // cur_node is JoshuaTree
      cur_node = malloc(sizeof(JoshuaTree));
      ((JoshuaTree *)cur_node)->data = malloc(sizeof(int) << a->chunksize);
    }
    // NOTE: will throw here if unallocated
  }
  // now insert into cur_node at inner_index
  ((JoshuaTree *)cur_node)->data[path.inner_index] = data;
  printf("inserted %d at %d\n", data, index);
}

//TODO: get, delete, get_chunk, delete_chunk


int main(int argc, char **argv) {
  time_t t = time(NULL);
  StitchedArray test_array = {.chunksize = 2, .root = {.Tree = {NULL, NULL}, .data = NULL}};
  printf("%d\n", 1 << test_array.chunksize);
  insert(&test_array, 10, 1);
  printf('done');
  //print the time
  printf("%d\n", time(NULL) - t);
}

// int add_two(int a, int b) { return a + b; }
// int (*func_type)(int, int) = &add_two;