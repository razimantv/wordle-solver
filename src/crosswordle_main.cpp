#include "crosswordle.h"

// Crosswordle main function
int main() {
  Crosswordle CG("data/dictionary.txt", "", "GYB");
  CG.play();
  return 0;
}
