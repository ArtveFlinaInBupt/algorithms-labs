#include "reader.h"

#include "data_entry.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

std::vector<Entry> read_data(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << argv[1] << std::endl;
    std::cerr << "Try `python3 run.py -p` to generate data file." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Entry> entries;
  for (std::string line; std::getline(file, line);) {
    std::istringstream iss(line);
    Entry e;
    if (!(iss >> e.enodebid >> e.longitude >> e.latitude)) {
      std::cerr << "Error reading line: " << line << std::endl;
      continue;
    }
    entries.emplace_back(e);
  }
  return entries;
}
