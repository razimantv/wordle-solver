#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <string>
#include <vector>

class GameBase {
 public:
  GameBase(const std::string& dictionaryfilename,
           const std::string& candidatefilename = {},
           const std::string& responsecolors = "GYB");
  virtual void play() = 0;

 protected:
  int word_length;
  double has_separate_candidatefile;
  std::vector<std::string> dictionary, candidates, &index;
  std::string responsecolors;

  void read(const std::string& dictionaryfilename,
            const std::string& candidatefilename = {});
  std::string match(const std::string& real, const std::string& attempt);
};

#endif
