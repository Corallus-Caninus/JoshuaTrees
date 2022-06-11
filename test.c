#include "JoshuaTrees.c"
int main(int argc, char *argv[]) {
  time_t t;
  srand((unsigned)time(&t));
  // get a random number between 0 and 6000000
  int randomNumber = rand() % 6000000;
  double time = clock();

  JoshuaTree *j = new_JoshuaTree();
  StitchedArray *a = j->b(6);

  for (int i = 0; i < 60000; i++) {
    j->i(a, i, i);
  }
  int i = j->g(a, randomNumber);
  printf("%d\n", i);
  // also print random number
  printf("%d\n", randomNumber);
  // print how long this took
  printf("%f\n", (clock() - time) / CLOCKS_PER_SEC);
}
