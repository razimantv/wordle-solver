#ifndef NERDLE_H
#define NERDLE_H

#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

// Class to generate all valid equations for nerdle (https://nerdlegame.com/)
class ExpressionGenerator {
 public:
  ExpressionGenerator(int len);
  void generate();                          // Generate all valid equations
  void write(const std::string& filename);  // Output valid equations to file

 protected:
  struct term {         // Term structure
    std::string expr;   // expression string
    double val;         // numerical value of the expression
    bool haspm, hasmd;  // whether the expression has +-, and */ operators
  };
  int len;
  std::vector<term> terms;
  std::unordered_set<std::string> seen;  // Already processed expressions
  std::vector<std::string> equations;    // Complete valid equations

  void process(const term& t1, const term& t2, char opchar,
               const std::function<double(double, double)>& opfun, bool haspm,
               bool hasmd);  // Process a new term found by combining two
};

#endif
