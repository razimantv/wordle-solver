#ifndef SQUAREWORD_H
#define SQUAREWORD_H

#include "gamebase.h"
#include <unordered_set>

class SquareWord : public GameBase {
 public:
  typedef std::vector<std::string> board;
  void play();
  SquareWord(int num_attempts, const std::string& squarewordfilename,
             const std::string& dictionaryfilename,
             const std::string& candidatefilename = {},
             const std::string& responsecolors = "GB",
             const std::string& firstguess = "");

 protected:
  int num_attempts;
  std::vector<board> validboards;
  void read_validboards(const std::string& squarewordfilename);

  typedef std::pair<std::string, std::string> response;
  typedef std::vector<response> boardresponse;

  // Memory of words played till now: 
  //    Used characters, remaining boards, solved boards, and found characters
  std::unordered_set<char> used_characters;
  std::vector<int> remaining;
  std::vector<bool> solved;
  std::vector<std::string> greens;

  std::string nextguess;

  int cost(const std::string& word, int limit);
  std::string bestguess();
  response match(const std::string& real, const std::string& attempt,
                 const std::string& green);
  boardresponse boardmatch(const board& b, const std::string& attempt);
  void update(const std::string& word, const boardresponse& boardresponse);
};

#endif /* ifndef SQUAREWORD_H */
