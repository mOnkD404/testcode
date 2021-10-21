
#include <cmath>
#include <iostream>

template <int i>
struct PrintTraitsHelper;

template <>
struct PrintTraitsHelper<0> {
  enum { digits = 0 };
  void Print() {}
};

template <int i>
struct PrintTraitsHelper {
  enum { value = i % 10 };
  enum { digits = 1 + PrintTraitsHelper<i / 10>::digits };
  void Print() {
    std::cout << value << " ";
    PrintTraitsHelper<i / 10>{}.Print();
  }
};

template <int i>
struct PrintTraits {
  using Helper = PrintTraitsHelper<i>;

  void operator()() {
    std::cout << "total: " << Helper::digits << " digits:";
    Helper{}.Print();
    std::cout << std::endl;
  }
};

template <>
struct PrintTraits<0> {
  void operator()() { std::cout << "total: 1  digits: 0" << std::endl; }
};

template <typename T>
struct PrintTable;
template <size_t... N>
struct PrintTable<std::index_sequence<N...>> {
  static void at(const int in) {
    static const std::function<void()> table[] = {PrintTraits<N>{}...};
    table[in]();
  }
};

void PrintInt(int c) {
  constexpr int size = 999;
  if (c > size)
    return;

  using Table = PrintTable<std::make_index_sequence<size>>;
  Table::at(c);
}

int main(int argc, char* argv[]) {
  for (std::string line; std::getline(std::cin, line);) {
    PrintInt(atoi(line.c_str()));
  }
  return 0;
}
