#include "wordle.h"
#include "nerdle.h"
#include <fstream>

// Nerdle main function
int main() {
  // If the expression table does not exist, create it
  std::string expressionfilename("data/valid_expressions.txt");
  {
    std::ifstream fin(expressionfilename);
    if (!fin) {
      ExpressionGenerator E(8);
      E.generate();
      E.write(expressionfilename);
    }
  }

  // Now just call Wordle with the expression set as dictionary
  Wordle G(1, 6, "data/valid_expressions.txt", "", "GRB", "7*5+4=39");
  G.play();
  return 0;
}
