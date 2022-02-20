#ifndef WORDLE_H
#define WORDLE_H

#include "gamebase.h"
#include <string>
#include <vector>

class Wordle : public GameBase {
 public:
  typedef std::pair<bool, std::vector<int>> board;

  Wordle(int num_boards, int num_attempts, const std::string& dictionaryfilename,
       const std::string& candidatefilename = {},
       const std::string& responsecolors = "GYB",
       const std::string& firstguess = "");
  void play();

 protected:
  int num_boards, num_attempts;
  std::string nextguess;
  std::vector<board> boards;

  std::string bestguess();
  bool sanitize();
  void prune(board& B, std::string attempt, std::string response);
};

#endif
