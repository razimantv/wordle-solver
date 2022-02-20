#include "crosswordle.h"
#include <cassert>
#include <iostream>
#include <string>

Crosswordle::Crosswordle(const std::string& dictionaryfilename,
                         const std::string& candidatefilename,
                         const std::string& responsecolors)
    : GameBase(dictionaryfilename, candidatefilename, responsecolors) {
  read_board();
}

void Crosswordle::read_board() {
  while (1) {
    std::string response;
    std::cin >> response;
    assert(response.size() == word_length);
    if (response == std::string(word_length, responsecolors[0])) break;
    responses.push_back(response);
  }
  std::cin >> finalword;
  assert(finalword.size() == word_length);

  num_attempts = responses.size();
  attempt_candidates = std::vector<std::vector<int>>(num_attempts);
}

bool Crosswordle::find_history(int r, std::vector<std::string>& history) {
  if (r < 0) return true;
  for (int i : attempt_candidates[r]) {
    for (int s = r + 1; s < num_attempts; ++s) {
      if (match(history[s], dictionary[i]) != responses[r]) goto BPP;
    }
    history[r] = dictionary[i];
    if (find_history(r - 1, history)) return true;
  BPP:;
  }
  return false;
}

void Crosswordle::play() {
  std::vector<std::string> history(num_attempts);

  for (int i = 0; i < num_attempts; ++i) {
    for (int j = 0; j < index.size(); ++j)
      if (match(finalword, index[j]) == responses[i])
        attempt_candidates[i].push_back(j);
  }

  if (find_history(num_attempts - 1, history))
    for (auto word : history) std::cout << word << "\n";
  else
    std::cout << "No solution found \n";
}

