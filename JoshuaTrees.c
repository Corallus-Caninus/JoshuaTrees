// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned char uint;

// TODO: encapsulate in an object that makes it easy to store type pointers

typedef struct JoshuaTree {
  // points to heap
  void *Node[2];
  // NOTE: always must alloc as a power of 2 so route addressing routine can
  // work these shoud be typecast in and out of JoshNode
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
  //TODO: this should be more static?
  int size_node = sizeof(int) << a->chunksize;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    if (!child) {
      // allocate a new node
      JoshuaTree *tmp_node = (JoshuaTree *)malloc(sizeof(JoshuaTree));
      // TODO: we dont necessarily need to eagerly alloc each time, which
      //       can make sparse indexing performant, profile this
      //  assign tmp_node to child
      tmp_node->data = (int *)malloc(size_node);
      tmp_node->Node[0] = NULL;
      tmp_node->Node[1] = NULL;
      cur_node->Node[path.outer_index & 1] = tmp_node;
    } else {
      cur_node = (JoshuaTree *)child;
      path.outer_index >>= 1;
    }
  } while ((path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  int *tmp_array = cur_node->data;
  tmp_array[path.inner_index] = data;
}

int get(StitchedArray *a, int index) {
  route_t path = route(*a, index);
  JoshuaTree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    if (!child) {
      return -1;
    } else {
      path.outer_index >>= 1;
      cur_node = (JoshuaTree *)child;
    }
  } while ((path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  int *tmp_array = cur_node->data;
  return tmp_array[path.inner_index];
}

// returns the given outer_index array
int **get_chunk(StitchedArray *a, int index) {
  route_t path = route(*a, index);
  JoshuaTree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    if (!child) {
      return NULL;
    } else {
      path.outer_index >>= 1;
      cur_node = (JoshuaTree *)child;
    }
  } while (!(path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  int **tmp_array = cur_node->data;
  return tmp_array;
}

// returns the node at the given outer_index
JoshuaTree *get_node(StitchedArray *a, int index) {
  route_t path = route(*a, index);
  JoshuaTree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    if (!child) {
      return NULL;
    } else {
      path.outer_index >>= 1;
      cur_node = (JoshuaTree *)child;
    }
  } while ((path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  return cur_node;
}

// NOTE: if using higher order addressing types, this can stack overflow unless
// TCO'd or iterated
// deletes the given node and recursively deletes all children
// nodes
void delete_node(JoshuaTree *target) {
  if (target->Node[0]) {
    delete_node((JoshuaTree *)target->Node[0]);
  }
  if (target->Node[1]) {
    delete_node((JoshuaTree *)target->Node[1]);
  }
  free(target->data);
  free(target);
}

int main(int argc, char **argv) {
  time_t t = time(NULL);
  int size_node = sizeof(int) << 2;
  StitchedArray test_array = {
      .chunksize = 2,
      .root = {.Node = {NULL, NULL}, .data = (int *)malloc(size_node)}};
  printf("%d\n", 1 << test_array.chunksize);
  insert(&test_array, 10, 1);
  printf("done");
  insert(&test_array, 11, 2);
  printf("done");
  int get_one = get(&test_array, 10);
  // print one
  printf("%d\n", get_one);
  insert(&test_array, 11, 3);
  int get_three = get(&test_array, 11);
  printf("%d\n", get_three);
  insert(&test_array, 10000000, 1);
  int get_last = get(&test_array, 10000000);
  printf("last: %d\n", get_last);
  // print the time
  printf("%d\n", (int)(time(NULL)) - (int)(t));
}
