#ifndef SQUAREWORDGENERATOR_H
#define SQUAREWORDGENERATOR_H

#include <string>
#include <vector>
#include "squareword.h"

class SquareWordGenerator {
 public:
  SquareWordGenerator(int word_length, const std::vector<std::string>& index);
  struct rowcolumn {
    std::string word;
    std::vector<int> remaining;
    bool done;
  };
  std::vector<SquareWord::board> generate();
  void fill_recursive(std::vector<rowcolumn>& horz,
                      std::vector<rowcolumn>& vert);
  std::pair<std::vector<rowcolumn>, std::vector<rowcolumn>> playword(
      const std::vector<rowcolumn>& mainboard,
      const std::vector<rowcolumn>& otherboard, int play_index,
      std::string word);

 protected:
  int word_length;
  const std::vector<std::string>& index;
  std::vector<SquareWord::board> validboards;
};

#endif /* ifndef SQUAREWORDGENERATOR_H */
