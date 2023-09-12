#include "squarewordgenerator.h"
#include <algorithm>
#include <numeric>

SquareWordGenerator::SquareWordGenerator(int word_length,
                                         const std::vector<std::string>& index)
    : word_length(word_length), index(index) {}

std::vector<SquareWord::board> SquareWordGenerator::generate() {
  int N_words = index.size();
  std::vector<int> remaining(N_words);
  std::iota(remaining.begin(), remaining.end(), 0);
  std::vector<rowcolumn> horz(
      word_length, {std::string(word_length, ' '), remaining, false}),
      vert{horz};

  validboards.clear();
  fill_recursive(horz, vert);
  return validboards;
}

void SquareWordGenerator::fill_recursive(std::vector<rowcolumn>& horz,
                                std::vector<rowcolumn>& vert) {
  bool gameover = true;
  int bestval = index.size() + 1;
  std::pair<int, int> best{-1, -1};

  for (int i = 0; i < word_length; ++i) {
    auto& [word, remaining, done] = horz[i];
    if (!done) {
      gameover = false;
      if (remaining.empty()) return;
      if (remaining.size() < bestval) best = {0, i}, bestval = remaining.size();
    }
  }

  if (gameover) {
    validboards.push_back({});
    for (auto& rc : horz) validboards.back().push_back(rc.word);
    return;
  }

  for (int i = 0; i < word_length; ++i) {
    auto& [word, remaining, done] = vert[i];
    if (!done) {
      if (remaining.empty()) return;
      if (remaining.size() < bestval) best = {1, i}, bestval = remaining.size();
    }
  }

  auto &mainboard = best.first ? vert : horz,
       &otherboard = best.first ? horz : vert;
  for (int nextword : mainboard[best.second].remaining) {
    auto [nextmain, nextother] =
        playword(mainboard, otherboard, best.second, index[nextword]);
    if (best.first) std::swap(nextmain, nextother);
    fill_recursive(nextmain, nextother);
  }
}

std::pair<std::vector<SquareWordGenerator::rowcolumn>,
          std::vector<SquareWordGenerator::rowcolumn>>
SquareWordGenerator::playword(const std::vector<rowcolumn>& mainboard,
                     const std::vector<rowcolumn>& otherboard, int play_index,
                     std::string word) {
  auto nextmain = mainboard, nextother = otherboard;
  nextmain[play_index] = {word, {}, true};
  for (int i = 0; i < word_length; ++i) {
    auto& [w, remaining, done] = nextother[i];
    if (done) continue;
    remaining.erase(
        std::remove_if(remaining.begin(), remaining.end(),
                       [&](int x) { return index[x][play_index] != word[i]; }),
        remaining.end());
  }
  return {nextmain, nextother};
}

