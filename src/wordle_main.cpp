#include "wordle.h"
#include <cstring>
#include <iostream>
#include <map>

void usage(std::ostream &out) {
  out << "Usage: wordle [--help] [-b <number of boards>] [-a <number of "
         "attempts>]"
      << std::endl;

  out << std::endl;
  out << "Optional arguments" << std::endl;
  out << "  --help  "
      << "Show this message and exit" << std::endl;
  out << "  -b      "
      << "Number of boards" << std::endl;
  out << "  -a      "
      << "Number of attempts" << std::endl;
}

// Wordle main function
// Multiple boards and game length are passed as command line arguments
int main(int argc, char *argv[]) {
  int num_boards = 1, num_attempts = 6;

  // Default arguments
  std::map<std::string, std::pair<int *, std::string>> optionmap{
      {"-b", {&num_boards, "boards"}}, {"-a", {&num_attempts, "attempts"}}};

  // Process options. Simple sanitisation as everything is of the form -x y
  for (int i = 1; i < argc; i += 2) {
    if (!optionmap.count(argv[i])) {
      std::cerr << "Unknown argument " << argv[i] << "\n";
      usage(std::cerr);
      return 1;
    }
    auto &[ptr, name] = optionmap[argv[i]];

    if (i + 1 == argc) {
      std::cerr << "Missing number of " << name << std::endl;
      usage(std::cerr);
      return 1;
    }
    int x;
    try {
      x = std::stoi(argv[i + 1]);
    } catch (...) {
      std::cerr << "Invalid argument " << argv[i + 1] << " for option "
                << argv[i] << "\n";
      usage(std::cerr);
      return 1;
    }
    if (x < 1) {
      std::cerr << "Number of " << name << " has to be positive\n";
      usage(std::cerr);
      return 1;
    }
    *ptr = x;
  }

  // Now just create the boards and play
  if (num_boards > 1)
    std::cout << "Wordle game with " << num_boards << " boards\n";
  Wordle G(num_boards, num_attempts, "data/dictionary.txt", "data/candidates.txt",
         "GYB", "arise");
  G.play();

  return 0;
}
