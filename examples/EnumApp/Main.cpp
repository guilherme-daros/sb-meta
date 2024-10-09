#include <iostream>

#include "types/Integer.hpp"

int main(int argc, char *argv[]) {
  auto i = types::Integer<uint64_t>();
  std::cout << i.range() << std::endl;
  std::cout << i.bitwidth() << std::endl;
  return 0;
}
