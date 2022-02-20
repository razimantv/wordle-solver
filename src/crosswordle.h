#ifndef CROSSWORDLE_H
#define CROSSWORDLE_H

#include "gamebase.h"

// Crosswordle solver (https://crosswordle.vercel.app/?daily=1)
class Crosswordle : public GameBase {
 public:
  void play();
  Crosswordle(const std::string& dictionaryfilename,
              const std::string& candidatefilename = "",
              const std::string& responsecolors = "");

 protected:
  int num_attempts;                    // Length of the game
  std::vector<std::string> responses;  // Responses that led to solution
  std::string finalword;
  // Subset of dictionary words that could have been played in each step
  std::vector<std::vector<int>> attempt_candidates;
  void read_board();  // Input the game board
  // Main recursive function that does the work
  bool find_history(int r, std::vector<std::string>& history);
};

#endif
