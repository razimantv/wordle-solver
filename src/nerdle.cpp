#include "nerdle.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

ExpressionGenerator::ExpressionGenerator(int len) : len(len) {}

void ExpressionGenerator::process(
    const term& t1, const term& t2, char opchar,
    const std::function<double(double, double)>& opfun, bool haspm,
    bool hasmd) {
  auto newexpr = t1.expr + opchar + t2.expr;
  if (seen.count(newexpr)) return;
  terms.push_back({newexpr, opfun(t1.val, t2.val), haspm, hasmd});
  seen.insert(newexpr);
  int result = int(terms.back().val + 0.1);
  if (std::abs(result - terms.back().val) < 1e-6) {
    auto resultstr = std::to_string(result);
    if (newexpr.size() + resultstr.size() == 7 and resultstr[0] != '-')
      equations.push_back(newexpr + "=" + resultstr);
  }
}

void ExpressionGenerator::generate() {
  for (int i = 0; ; ++i) {
    auto str = std::to_string(i);
    if (std::min(str.size() * 2 + 1, str.size() + 4) > len) break;
    terms.push_back({str, (double)i, false, false});
    seen.insert(terms.back().expr);
  }

  while (1) {
    int N = terms.size(), startsize = seen.size();
    for (int i = 0; i < N; ++i) {
      term t1{terms[i]};
      for (int j = 0; j < N; ++j) {
        term t2{terms[j]};
        if (t1.expr.size() + t2.expr.size() > 5) break;
        process(t1, t2, '+', std::plus<double>(), true, t1.hasmd or t2.hasmd);
        if (t2.haspm) continue;
        process(t1, t2, '-', std::minus<double>(), true, t1.hasmd or t2.hasmd);
        if (t1.haspm or t2.haspm) continue;
        process(t1, t2, '*', std::multiplies<double>(), false, true);
        if (t2.hasmd or t2.val < 1e-6) continue;
        process(t1, t2, '/', std::divides<double>(), false, true);
      }
    }
    if (startsize == seen.size()) break;
    sort(terms.begin(), terms.end(), [](const term& t1, const term& t2) {
      return t1.expr.size() < t2.expr.size();
    });
  }
}

void ExpressionGenerator::write(const std::string& filename) {
  std::ofstream out(filename);
  for(const auto& eq: equations) out << eq << "\n";
}

