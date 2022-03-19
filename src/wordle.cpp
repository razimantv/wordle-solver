#include "wordle.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

// Constructor: Call base constructor and initialise boards
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

// If no words remain in a board, game cannot be solved
// If only one word remains in some board, optimal to play it now
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

// Cost of attempting a word W
//   For every other word W' remaining in every board,
//     find out what response we get if W' was the word
//   Group W' into sets based on this response
//   The largest set is the "worst case scenario"
// Cost is the product of worst case scenario over all boards
double Wordle::cost(std::string word) {
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
  return cost;
}

// Find the best guess for the remaining set of words
// Play the word with the lowest cost
// Prioritise words with highest frequency among sets
std::string Wordle::bestguess() {
  std::string best;
  double bestval = std::pow(index.size() + 1., boards.size());

  std::unordered_map<int, int> cnt;
  std::map<int, std::unordered_set<int>, std::greater<int>> cntmap;

  // Find frequencies of remaining words
  for (auto& [state, remaining] : boards) {
    if (!state) continue;
    for(auto word2:remaining) {
      if(cnt.count(word2)) cntmap[cnt[word2]].erase(word2);
      cntmap[++cnt[word2]].insert(word2);
    }
  }

  // Try out most frequent words as possible attempts first
  for(auto& [c, vec]: cntmap) 
    for(int word: vec)
      if (double c; (c = cost(index[word])) < bestval)
        bestval = c, best = index[word];

  // Now try all words in the dictionary
  for (auto word : dictionary) {
    if (double c; (c = cost(word)) < bestval)
      bestval = c, best = word;
  }

  return best;
}

// Prune the remaining words so that only the words which give
//   the received response to out last attempt are retained
void Wordle::prune(board& B, std::string attempt, std::string response) {
  auto& [state, remaining] = B;
  if (response == std::string(word_length, responsecolors[0])) {
    state = false;
    return;
  }

  std::vector<int> pruned;
  for (int i : remaining)
    if (match(index[i], attempt) == response) pruned.push_back(i);
  remaining = pruned;
}

void Wordle::play() {
  // We have only given number of attempts
  for (int i = 1, done = 0; i <= num_attempts; ++i) {
    std::string response;

    if (!sanitize()) break;
    if (nextguess.empty()) nextguess = bestguess();

    // Make the guess and read the response from the user
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
  std::cout << "Game could not be solved\n";
}
