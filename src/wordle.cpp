#include "wordle.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_map>

Wordle::Wordle(int num_boards, int num_attempts,
           const std::string& dictionaryfilename,
           const std::string& candidatefilename,
           const std::string& responsecolors, const std::string& firstguess)
    : GameBase(dictionaryfilename, candidatefilename, responsecolors),
      num_boards(num_boards),
      num_attempts(num_attempts),
      nextguess(firstguess) {
  board B{true, std::vector<int>(index.size())};
  std::iota(B.second.begin(), B.second.end(), 0);
  boards = std::vector<board>(num_boards, B);
}

bool Wordle::sanitize() {
  for (int i = 0; i < num_boards; ++i) {
    if (!boards[i].first) continue;
    if (boards[i].second.size() == 1) {
      std::cout << "Only one candidate remaining"
                << ((num_boards > 1) ? (" in board " + std::to_string(i + 1))
                                     : "")
                << "\n";
      nextguess = index[boards[i].second[0]];
      return true;
    } else if (boards[i].second.size() == 0) {
      std::cout << "No candidates remaining"
                << ((num_boards > 1) ? (" in board " + std::to_string(i + 1))
                                     : "")
                << "\n";
      return false;
    }
  }
  return true;
}

// Find the best guess for the remaining set of words
// For every word W in dictionary, suppose we attempt it
//   For every other word W' remaining,
//     find out what response we get if W' was the word
//   Group W' into sets based on this response
//   The largest set is the "worst case scenario"
// Pick W that gives the smallest product of worst case scenario over all boards
std::string Wordle::bestguess() {
  std::string best;
  double bestval = std::pow(index.size() + 1., boards.size());

  // Loop over words W in dictionary as possible attempts
  for (auto word : dictionary) {
    double cost = 1;
    for (auto& [state, remaining] : boards) {
      if (!state) continue;

      // Group every word W' in remaining based on response to playing W
      std::unordered_map<std::string, int> sets;
      for (auto word2 : remaining) {
        ++sets[match(index[word2], word)];
      }

      // Find the worst case scenario and update bestvec accordingly
      int worst = 0;
      for (const auto& [response, count] : sets) worst = std::max(worst, count);
      cost *= worst;
    }
    if (cost < bestval) bestval = cost, best = word;
  }

  return best;
}

// Prune the remaining words so that only the words which give
//   the received response to out last attempt are retained
void Wordle::prune(board& B, std::string attempt, std::string response) {
  if (response == std::string(word_length, responsecolors[0])) {
    B.first = false;
    return;
  }

  std::vector<int> pruned;
  for (int i : B.second)
    if (match(index[i], attempt) == response) pruned.push_back(i);
  B.second = pruned;
}

void Wordle::play() {
  // We have only six attempts
  for (int i = 1, done = 0;; ++i) {
    std::string response;

    if (!sanitize()) goto BPP;
    if (nextguess.empty()) nextguess = bestguess();

    // Make the guess and wait for the response from the user
    std::cout << "Guess " << i << ". " << nextguess << "\n";
    for (int j = 0; board & B : boards) {
      std::string boardstring =
          (boards.size() > 1) ? (" for board " + std::to_string(j + 1)) : "";
      auto& [state, remaining] = B;
      ++j;
      if (!state) continue;
      std::cout << "Enter response" << boardstring << " (" << B.second.size()
                << " candidates): ";
      std::cin >> response;
      assert(response.size() == word_length);
      if (response == std::string(word_length, responsecolors[0])) {
        std::cout << "Solution " << nextguess << boardstring << " found in "
                  << i << " attempts \n";
        if (++done == num_boards) {
          std::cout << "Game solved in " << i << " attempts\n";
          return;
        }
      }
      prune(B, nextguess, response);
    }
    nextguess.clear();
  }
BPP:;
  std::cout << "Game could not be solved\n";
}
