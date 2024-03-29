// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//TODO: capsulize and greate a .data global psuedoheap and try this as the allocator

//typedef unsigned char uint;

typedef struct Tree {
  // points to heap
  void *Node[2];
  // NOTE: always must alloc as a power of 2 for the addressing routine
  //TODO: this should be generic voidptr
  void **data;
} Tree;
typedef struct StitchedArray {
  // NOTE: this must be a power of two
  int chunksize;
  Tree root;
} StitchedArray;

// TODO: int size should vary here for different architectures and addressing
typedef struct route_t {
  int outer_index;
  int inner_index;
} route_t;
static inline route_t route(StitchedArray root, int index) {
  int outer_index = (index >> root.chunksize) + 2;
  int inner_index = index & ((1 << root.chunksize) - 1);
  route_t res = {outer_index, inner_index};
  return res;
}

static void insert(StitchedArray *a, int index, void* data, uint size) {
  route_t path = route(*a, index);
  register Tree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    if (!child) {
      // allocate a new node
      Tree *tmp_node = (Tree *)malloc(sizeof(Tree));
      tmp_node->data = NULL;
      tmp_node->Node[0] = NULL;
      tmp_node->Node[1] = NULL;
      //  assign tmp_node to child
      cur_node->Node[path.outer_index & 1] = tmp_node;
      cur_node = tmp_node;
      path.outer_index >>= 1;
    } else {
      cur_node = (Tree *)child;
      path.outer_index >>= 1;
    }
  } while ((path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  void** tmp_array = cur_node->data;
  if (tmp_array == NULL) {
    tmp_array = (void**)malloc(sizeof(void*)*(1<< a->chunksize));
    cur_node->data = tmp_array;
    //initialize the array to NULL
  }
  //save the value pointed to by data given the size offset that was passed in
  void* tmp = malloc(size);
  memcpy(tmp, data, size);
  tmp_array[path.inner_index] = tmp;
}

static void* get(StitchedArray *a, int index, uint size) {
  route_t path = route(*a, index);
  register Tree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    cur_node = (Tree *)child;
    path.outer_index >>= 1;
  } while ((path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
//  void** tmp_array = cur_node->data;
//  void *tmp = malloc(size);
//  memcpy(tmp, tmp_array[path.inner_index], size);
//  return tmp;
//just point to the data
  void** tmp_array = (void**)cur_node->data;
  return tmp_array[path.inner_index];
  //return tmp_array[path.inner_index];
}

// returns the given outer_index array
static void **get_chunk(StitchedArray *a, int index) {
  route_t path = route(*a, index);
  register Tree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    path.outer_index >>= 1;
    cur_node = (Tree *)child;
  } while (!(path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  void **tmp_array = (void**)cur_node->data;
  return tmp_array;
}

// returns the node at the given outer_index
static Tree *get_node(StitchedArray *a, int index) {
  route_t path = route(*a, index);
  register Tree *cur_node = &a->root;
  // NOTE: cur node MUST not be NULL here
  do {
    void *child = cur_node->Node[path.outer_index & 1];
    path.outer_index >>= 1;
    cur_node = (Tree *)child;
  } while ((path.outer_index != 1 && path.outer_index != 0));
  //  now insert into cur_node at inner_index
  return cur_node;
}

// NOTE: if using higher order addressing types, this can stack overflow unless
// TCO'd or iterated
// deletes the given node and recursively deletes all children
// nodes
static void delete_node(Tree *target, int chunksize, uint size) {
  if (target->Node[0] != NULL) {
    delete_node((Tree *)target->Node[0], chunksize, size);
  }
  if (target->Node[1] != NULL) {
    delete_node((Tree *)target->Node[1], chunksize, size);
  }
  if(target->data != NULL)
    for (int i = 0; i < (1 << chunksize); i++) {
      if (target->data[i] != NULL) {
        free(target->data[i]);
        target->data[i] = NULL;
      }
    }
    free(target->data);
    target->data = NULL;
  free(target);
  target=NULL;
}
//delete_node for a StitchedArray
static void delete_stitched_array(StitchedArray *target, uint size) {
  printf("deleting stitched array");
  delete_node((Tree *)target->root.Node[0], target->chunksize, size);
  delete_node((Tree *)target->root.Node[1], target->chunksize, size);
  free(target);
  target=NULL;
}

// free the given nodes array
//static void dealloc_node(Tree *target) { free(target->data); }
// conditionally dealloc if the array at node is all 0,
// this can be called as a pass on the entire tree for defragmentation
static void dealloc_cond(Tree *target, int chunksize) {
  for (int i = 0; i < (1<<chunksize); i++) {
    if (target->data[i] != NULL) {
      return;
    }
  }
  // dealloc
  free(target->data);
  target->data = NULL;
}

static void dealloc_node(Tree *target, int chunksize) {
  fflush(stdout);
  if (target->Node[0]) {
    dealloc_node((Tree*)target->Node[0], chunksize);
  }
  if (target->Node[1]) {
    dealloc_node((Tree*)target->Node[1],chunksize);
  }
  dealloc_cond(target, chunksize);
}
//delete_node for a StitchedArray
static void cond_dealloc_stitched_array(StitchedArray *target) {
  dealloc_node((Tree *)target->root.Node[0],target->chunksize);
  dealloc_node((Tree *)target->root.Node[1],target->chunksize);
}

// JOSHUA TREE CLASS //
// constructs a StitchedArray given chunksize
static StitchedArray *build(int chunksize) {
  StitchedArray *a = (StitchedArray *)malloc(sizeof(StitchedArray));
  a->chunksize = chunksize;
  a->root.Node[0] = NULL;
  a->root.Node[1] = NULL;
  a->root.data = NULL;
  return a;
}

//TODO:@DEPRECATED this is what V is for
// class for JoshuaTrees
//typedef struct JoshuaTree {
//  StitchedArray *(*b)(void*);
//  // function pointers:
//  void (*i)(struct StitchedArray *, int, int);
//  int (*g)(struct StitchedArray *, int);
//  int **(*gc)(struct StitchedArray *, int);
//  Tree *(*gn)(struct StitchedArray *, int);
//  void (*dn)(struct Tree *);
//} JoshuaTree;
//// builder for JoshuaTree class
//static JoshuaTree *new_JoshuaTree() {
//  JoshuaTree *jt = (JoshuaTree *)malloc(sizeof(JoshuaTree));
//  // builds a new StitchedArray
//  jt->b = build;
//  // inserts a new element into the StitchedArray
//  jt->i = insert;
//  // gets the element at the given index
//  jt->g = get;
//  // gets the chunk at the given index
//  jt->gc = get_chunk;
//  // gets the node at the given index
//  jt->gn = get_node;
//  // deletes the given node and all children
//  jt->dn = delete_node;
//  return jt;
//}
//