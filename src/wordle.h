#ifndef WORDLE_H
#define WORDLE_H

#include "gamebase.h"
#include <string>
#include <vector>

// Class for multiple wordle boards
// (https://www.nytimes.com/games/wordle/index.html)
class Wordle : public GameBase {
 public:
  // Board state = whether it is still in play, list of possible words
  typedef std::pair<bool, std::vector<int>> board;

  Wordle(int num_boards, int num_attempts,
         const std::string& dictionaryfilename,
         const std::string& candidatefilename = {},
         const std::string& responsecolors = "GYB",
         const std::string& firstguess = "");
  void play();

 protected:
  int num_boards, num_attempts;
  std::string nextguess;
  std::vector<board> boards;

  std::string bestguess();  // Find the best next guess
  bool sanitize();          // Process singleton and null sets
  // Prune word list to only those giving the current response
  void prune(board& B, std::string attempt, std::string response);
};

#endif
