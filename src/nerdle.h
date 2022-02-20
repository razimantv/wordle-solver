#ifndef NERDLE_H
#define NERDLE_H

#include <string>
#include <vector>
#include <unordered_set>
#include <functional>

class ExpressionGenerator {
 public:
  ExpressionGenerator(int len);
  void generate();
  void write(const std::string& filename);

 protected:
  struct term {
    std::string expr;
    double val;
    bool haspm, hasmd;
  };
  int len;
  std::vector<term> terms;
  std::unordered_set<std::string> seen;
  std::vector<std::string> equations;

  void process(const term& t1, const term& t2, char opchar,
               const std::function<double(double, double)>& opfun, bool haspm,
               bool hasmd);
};

#endif
