#include "crosswordle.h"

int main() {
  Crosswordle CG("data/dictionary.txt", "", "GYB");
  CG.play();
  return 0;
}
