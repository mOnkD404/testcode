
#include <iostream>

template <int i>
struct PrintTraitsHelper;

template <>
struct PrintTraitsHelper<0> {
  void operator()() {}
};

template <int i>
struct PrintTraitsHelper {
  enum { value = i % 10 };
  void operator()() {
    std::cout << value << " ";
    PrintTraitsHelper<i / 10>{}();
  }
};

template <int i>
struct PrintTraits {
  using Type = PrintTraitsHelper<i>;
};

template <typename T>
struct PrintTable;
template <size_t... N>
struct PrintTable<std::index_sequence<N...>> {
  static void at(const int in) {
    static const std::function<void()> table[] = {
        typename PrintTraits<N>::Type{}...};
    table[in]();
  }
};

void PrintInt(int c) {
  constexpr int size = 99999;
  if (c > size)
    return;

  using Table = PrintTable<std::make_index_sequence<size>>;
  Table::at(c);
}

int main(int argc, char* argv[]) {
  PrintInt(123);
  return 0;
}
