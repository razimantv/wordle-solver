#ifndef CROSSWORDLE_H
#define CROSSWORDLE_H

#include "gamebase.h"

class Crosswordle : public GameBase {
 public:
  void play();
  Crosswordle(const std::string& dictionaryfilename,
              const std::string& candidatefilename = "",
              const std::string& responsecolors = "");

 protected:
  int num_attempts;
  std::vector<std::string> responses;
  std::vector<std::vector<int>> attempt_candidates;
  std::string finalword;
  void read_board();
  bool find_history(int r, std::vector<std::string>& history);
};

#endif
