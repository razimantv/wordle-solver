#include "gamebase.h"
#include <cassert>
#include <fstream>
#include <unordered_map>

GameBase::GameBase(const std::string& dictionaryfilename,
                   const std::string& candidatefilename,
                   const std::string& responsecolors)
    : word_length(-1),
      has_separate_candidatefile(!candidatefilename.empty()),
      index(has_separate_candidatefile ? candidates : dictionary),
      responsecolors(responsecolors) {
  read(dictionaryfilename, candidatefilename);
}

// Read all words from dictionary: Assumes the file is sane
void GameBase::read(const std::string& dictionaryfilename,
                const std::string& candidatefilename) {
  std::string word;
  std::ifstream dictfile(dictionaryfilename);
  while (dictfile >> word) {
    if (word_length == -1)
      word_length = word.size();
    else
      assert(word_length == word.size());
    dictionary.push_back(word);
  }
  assert(!dictionary.empty());

  if (has_separate_candidatefile) {
    std::ifstream candidatefile(candidatefilename);
    while (candidatefile >> word) {
      assert(word_length == word.size());
      candidates.push_back(word);
    }
  }
}

// Find the response on playing <attempt> to the word <real>
std::string GameBase::match(const std::string& real, const std::string& attempt) {
  std::string ret(word_length, '0');

  // To count unmatched occurreces
  std::unordered_map<char, int> cnt;

  // First look for exact character matches
  // And increment counter of unmatched characters
  for (int i = 0; i < word_length; ++i) {
    if (real[i] == attempt[i])
      ret[i] = responsecolors[0];
    else
      ++cnt[real[i]];
  }

  // Now mark all the characters at wrong positions
  for (int i = 0; i < word_length; ++i) {
    if (ret[i] != '0') continue;

    if (cnt[attempt[i]])
      ret[i] = responsecolors[1], --cnt[attempt[i]];
    else
      ret[i] = responsecolors[2];
  }

  return ret;
}

