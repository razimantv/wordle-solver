#include "squareword.h"

int main() {
  SquareWord G(15, "data/squareword_boards.txt",
               "data/squareword_dictionary.txt", {}, "GB", "loner");
  G.play();
  return 0;
}
