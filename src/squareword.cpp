#include "gamebase.h"
#include "squarewordgenerator.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <unordered_map>
#include <unordered_set>

SquareWord::SquareWord(int num_attempts, const std::string& squarewordfilename,
                       const std::string& dictionaryfilename,
                       const std::string& candidatefilename,
                       const std::string& responsecolors,
                       const std::string& firstguess)
    : GameBase(dictionaryfilename, candidatefilename, responsecolors),
      num_attempts(num_attempts),
      solved(std::vector<bool>(word_length, false)),
      greens(word_length, std::string(word_length, ' ')),
      nextguess(firstguess) {
  read_validboards(squarewordfilename);
}

void SquareWord::read_validboards(const std::string& squarewordfilename) {
  std::ifstream fin(squarewordfilename);
  // If the file with valid boards does not exist, create it
  if (!fin) {
    std::cout << "Generating valid boards\n";
    SquareWordGenerator G(word_length, index);
    validboards = G.generate();
    std::ofstream fout(squarewordfilename);
    assert(fout);
    for (auto& B : validboards) {
      for (auto& word : B) fout << word << " ";
      fout << "\n";
    }
    std::cout << "Wrote valid boards to " << squarewordfilename << "\n";
  } else {
    std::string word;
    while (fin >> word) {
      validboards.push_back({word});
      for (int i = 1; i < word_length; ++i) {
        fin >> word;
        validboards.back().push_back(word);
      }
    }
  }

  int V = validboards.size();
  remaining = std::vector<int>(V);
  std::iota(remaining.begin(), remaining.end(), 0);
}

// Same idea as the wordle cost
// Partition boards into sets based on response to word and pick the largest
int SquareWord::cost(const std::string& word, int limit) {
  std::map<boardresponse, int> sets;
  int cost = 0;
  for (int b : remaining) {
    cost = std::max(cost, ++sets[boardmatch(validboards[b], word)]);
    if (cost >= limit) return limit + 1;
  }
  return cost;
}

// Same idea as Wordle bestguess:
// Play the word that gives the lowest cost,
//    prioritising most frequent words in remaining boards
std::string SquareWord::bestguess() {
  std::string best;
  int bestval = remaining.size() + 1;

  std::unordered_map<std::string, int> cnt;
  std::map<int, std::unordered_set<std::string>, std::greater<int>> cntmap;

  // Find frequencies of remaining words
  for (int i : remaining) {
    for (int j = 0; j < word_length; ++j) {
      if (solved[j]) continue;
      auto word2 = validboards[i][j];
      if (cnt.count(word2)) cntmap[cnt[word2]].erase(word2);
      cntmap[++cnt[word2]].insert(word2);
    }
  }

  // Try out most frequent words as possible attempts first
  for (auto& [c, vec] : cntmap)
    for (const auto& word : vec)
      if (double c = cost(word, bestval); c < bestval) bestval = c, best = word;

  for (const auto& word : dictionary)
    if (int worst = cost(word, bestval); worst < bestval)
      bestval = worst, best = word;

  return best;
}

void SquareWord::play() {
  for (int n = 1, done = 0; n <= num_attempts and !remaining.empty(); ++n) {
    if (nextguess.empty()) nextguess = bestguess();
    // Make the guess and read the response from the user
    std::cout << "Guess " << n << ". " << nextguess << " (" << remaining.size()
              << " candidates remaining)\n";

    boardresponse boardresponse;
    std::vector<int> newsolved;
    for (int i = 0; i < word_length; ++i) {
      if (solved[i]) continue;
      std::cout << "Enter response for row " << i + 1 << ": ";
      response response;
      auto& [exactmatch, partialmatch] = response;
      std::cin >> exactmatch >> partialmatch;
      assert(exactmatch.size() == word_length);
      if (!isalpha(partialmatch[0])) partialmatch = "";
      sort(partialmatch.begin(), partialmatch.end());
      if (exactmatch == std::string(word_length, responsecolors[0])) {
        newsolved.push_back(i);
        std::cout << "Row  " << i + 1 << " solved in " << n << " attempts \n";
        if (++done == word_length) {
          std::cout << "Game solved in " << n << " attempts\n";
          return;
        }
      }
      boardresponse.push_back(response);
    }
    update(nextguess, boardresponse);
    nextguess.clear();
  }
  std::cout << "Game could not be solved\n";
}

// Game has the weird property that the response depends on past guesses as well
// Modify match function to use memory of past attempts and already found cells
SquareWord::response SquareWord::match(const std::string& real,
                                       const std::string& attempt,
                                       const std::string& green) {
  auto used = this->used_characters;
  for (char c : attempt) used.insert(c);

  std::string exactmatch(word_length, responsecolors[1]);
  std::set<char> existingletters;
  for (int i = 0; i < word_length; ++i) {
    if (green[i] == responsecolors[0] or real[i] == attempt[i])
      exactmatch[i] = responsecolors[0];
    else if (used.count(real[i]))
      existingletters.insert(real[i]);
  }
  std::string partialmatch(existingletters.begin(), existingletters.end());
  return {exactmatch, partialmatch};
}

// Response for the board is just a vector of responses for words still in play
SquareWord::boardresponse SquareWord::boardmatch(const board& b,
                                                 const std::string& attempt) {
  boardresponse ret;
  for (int i = 0; i < word_length; ++i)
    if (!solved[i]) ret.push_back(match(b[i], attempt, greens[i]));
  return ret;
}

// Keep only the boards giving the correct response
// And update list of used characters and green character positions
void SquareWord::update(const std::string& word,
                        const boardresponse& boardresponse) {
  remaining.erase(std::remove_if(remaining.begin(), remaining.end(),
                                 [&](int x) {
                                   return boardmatch(validboards[x], word) !=
                                          boardresponse;
                                 }),
                  remaining.end());
  for (char c : word) used_characters.insert(c);
  for (int i = 0, j = 0; i < word_length; ++i) {
    if (solved[i]) continue;
    auto& exactmatch = boardresponse[j++].first;
    if (exactmatch == std::string(word_length, responsecolors[0]))
      solved[i] = 1;
    for (int k = 0; k < word_length; ++k)
      if (exactmatch[k] == responsecolors[0]) greens[i][k] = responsecolors[0];
  }
}
