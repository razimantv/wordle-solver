#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <string>
#include <vector>

// Base class the game is built on
class GameBase {
 public:
  GameBase(const std::string& dictionaryfilename,
           const std::string& candidatefilename = {},
           const std::string& responsecolors = "GYB");
  virtual void play() = 0;

 protected:
  int word_length;
  // If the dictionary and candidate lists are different, index will point to
  // candidates and to dictionary otherwise
  double has_separate_candidatefile;
  std::vector<std::string> dictionary, candidates, &index;
  // Three characters for complete match, match at wrong position, and no match
  std::string responsecolors;

  // Read the word lists
  void read(const std::string& dictionaryfilename,
            const std::string& candidatefilename = {});
  // Compute the match between the real and attempted words
  std::string match(const std::string& real, const std::string& attempt);
};

#endif
