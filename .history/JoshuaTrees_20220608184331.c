// JoshuaTrees are BST-arrays
// you get the best of both worlds
#include <stdbool.h>
#include <stdio.h>
typedef unsigned char uint;

void* iter(int length, *void start, *void type) {
    void* current = start;
    for (int i = 0; i < length; i++) {
        current = *(void**)current;
    }
    return current;
}

typedef struct JoshuaTree
{
  // points to heap
  void *Tree[2];
  void *data;
} JoshTree;


int add_two(int a, int b) { return a + b; }
int (*func_type)(int, int) = &add_two;

int main(int argc, char **argv)
{
  int data[2] = {1, 2};
  JoshTree a = {.Tree = {1, 2}, .data = data};

  printf("Hello World %d : %d -> %d\n", a.Tree[0], a.Tree[1], a.data);
  uint test = 2;
  test <<= 1;
  printf("%d -> %d\n", sizeof(test), test);

  void *mem = malloc(sizeof(JoshTree));
  free(mem);
  printf("%d\n", func_type);
  // print size of func_type
  printf("%d\n", sizeof(func_type));

  // int res = (*func_type)(1,2);
  int res = func_type(1, 2);
  printf("%d\n", res);
}