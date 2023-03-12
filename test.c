#include "JoshuaTrees.c"
int main(int argc, char *argv[]) {
  time_t t;
  srand((unsigned)time(&t));
  // get a random number between 0 and 600000
  int randomNumber = rand() % 600000000;
  double time = clock();

  JoshuaTree *j = new_JoshuaTree();
  StitchedArray *a = j->b(4);

  for (int i = 0; i < 600000000; i++) {
    if (i % 10000000 == 0){
    printf("alloced..");
    //flush
    fflush(stdout);
    }
    j->i(a, i, i);
  }
  int i = j->g(a, randomNumber);
  //get 100 random entries
  for (int i = 0; i < 60000000; i++) {
    // get a random number between 0 and 600000
    int randomNumber = rand() % 60000000;
    int i = j->g(a, randomNumber);
    //printf("%d\n", i);
  }
  printf("%d\n", i);
  // also print random number
  printf("%d\n", randomNumber);
  // print how long this took
  //printf("%f\n", (clock() - time) / CLOCKS_PER_SEC);
}
