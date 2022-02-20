#include "nerdle.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

ExpressionGenerator::ExpressionGenerator(int len) : len(len) {}

const std::unordered_map<char, std::function<double(double, double)>>
    ExpressionGenerator::opfun{{'+', std::plus<double>()},
                               {'-', std::minus<double>()},
                               {'*', std::multiplies<double>()},
                               {'/', std::divides<double>()}};

// Process a new term found by combining two existing terms
void ExpressionGenerator::process(const term& t1, const term& t2, char opchar,
                                  bool haspm, bool hasmd) {
  auto newexpr = t1.expr + opchar + t2.expr;
  // First check if the term has been processed already
  if (seen.count(newexpr)) return;

  // Store it
  terms.push_back({newexpr, opfun.at(opchar)(t1.val, t2.val), haspm, hasmd});
  seen.insert(newexpr);

  // If the value of the expression is an integer that completes the equation,
  // save it
  int result = int(terms.back().val + 0.1);
  if (std::abs(result - terms.back().val) < 1e-6) {
    auto resultstr = std::to_string(result);
    // Value needs to be positive and total length needs to be correct
    if (newexpr.size() + resultstr.size() == len - 1 and resultstr[0] != '-')
      equations.push_back(newexpr + "=" + resultstr);
  }
}

// Generate all valid equations of given length
void ExpressionGenerator::generate() {
  // First, generate all numbers that could appear in terms
  for (int i = 0; ; ++i) {
    auto str = std::to_string(i);
    if (std::min(str.size() * 2 + 1, str.size() + 4) > len) break;
    terms.push_back({str, (double)i, false, false});
    seen.insert(terms.back().expr);
  }

  // Combine terms pairwise with operators
  // Ensure that new operators play well with existing ones
  //    Cannot have a-(b-c) for instance
  while (1) {
    int N = terms.size(), startsize = seen.size();
    for (int i = 0; i < N; ++i) {
      term t1{terms[i]};
      for (int j = 0; j < N; ++j) {
        term t2{terms[j]};
        if (t1.expr.size() + t2.expr.size() > 5) break;
        process(t1, t2, '+', true, t1.hasmd or t2.hasmd);
        if (t2.haspm) continue;
        process(t1, t2, '-', true, t1.hasmd or t2.hasmd);
        if (t1.haspm or t2.haspm) continue;
        process(t1, t2, '*', false, true);
        if (t2.hasmd or t2.val < 1e-6) continue;
        process(t1, t2, '/', false, true);
      }
    }
    // Stop when no new term has been added
    if (startsize == seen.size()) break;
    sort(terms.begin(), terms.end(), [](const term& t1, const term& t2) {
      return t1.expr.size() < t2.expr.size();
    });
  }
}

// Write out the valid equations to file
void ExpressionGenerator::write(const std::string& filename) {
  std::ofstream out(filename);
  for(const auto& eq: equations) out << eq << "\n";
}

