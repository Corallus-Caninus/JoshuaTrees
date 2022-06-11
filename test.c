#include "JoshuaTrees.c"
int main(int argc, char *argv[]) {
  double time = clock();

  JoshuaTree *j = new_JoshuaTree();
  StitchedArray *a = j->b(2);

  for (int i = 0; i < 60000; i++) {
    j->i(a, i, i);
  }
  int i = j->g(a, 300);
  printf("%d\n", i);
  // print how long this took
  printf("%f\n", (clock() - time) / CLOCKS_PER_SEC);
  printf("in test");
}
