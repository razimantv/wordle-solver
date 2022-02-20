#include "crosswordle.h"
#include <cassert>
#include <iostream>
#include <string>

// Constructor: Call parent constructor and read board state
Crosswordle::Crosswordle(const std::string& dictionaryfilename,
                         const std::string& candidatefilename,
                         const std::string& responsecolors)
    : GameBase(dictionaryfilename, candidatefilename, responsecolors) {
  read_board();
}

// Read board state
void Crosswordle::read_board() {
  // Read list of responses until getting all correct
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

// Main recursive function to find the history
// Starts from the last level and goes up
// At each step, only try words that keep all later guesses valid
bool Crosswordle::find_history(int r, std::vector<std::string>& history) {
  if (r < 0) return true;
  // Loop over all candidates at this level
  for (int i : attempt_candidates[r]) {
    // Check that playing this word here keeps all later words valid
    for (int s = r + 1; s < num_attempts; ++s) {
      if (match(history[s], dictionary[i]) != responses[r]) goto BPP;
    }
    history[r] = dictionary[i];
    // And recurse
    if (find_history(r - 1, history)) return true;
  BPP:;
  }
  return false;
}

// Play the game
void Crosswordle::play() {
  std::vector<std::string> history(num_attempts);

  // First, prune the candidate words for each attempt
  for (int i = 0; i < num_attempts; ++i) {
    for (int j = 0; j < index.size(); ++j)
      if (match(finalword, index[j]) == responses[i])
        attempt_candidates[i].push_back(j);
  }

  // And call the recursive function
  if (find_history(num_attempts - 1, history))
    for (auto word : history) std::cout << word << "\n";
  else
    std::cout << "No solution found \n";
}

