
#include <cmath>
#include <iostream>

template <int... Is>
struct DigitsVector {
  using Type = DigitsVector<Is...>;
  static void PrintDigitsString() { ((std::cout << Is << " "), ...); }
  static void PrintDigitsCount() { std::cout << sizeof...(Is); }
};

template <int IntVal, typename = DigitsVector<>>
struct DigitTraitsHelper;

template <int IntVal, int... Is>
struct DigitTraitsHelper<IntVal, DigitsVector<Is...>>
    : DigitTraitsHelper<IntVal / 10, DigitsVector<IntVal % 10, Is...>> {};

template <int... Is>
struct DigitTraitsHelper<0, DigitsVector<Is...>> : DigitsVector<Is...> {};

template <typename T, int... Rs>
struct DigitVectorReverseHelper;

template <int IntVal, int... Is, int... Rs>
struct DigitVectorReverseHelper<DigitsVector<IntVal, Is...>, Rs...>
    : DigitVectorReverseHelper<DigitsVector<Is...>, IntVal, Rs...> {};

template <int... Rs>
struct DigitVectorReverseHelper<DigitsVector<>, Rs...> : DigitsVector<Rs...> {};

template <int i>
struct PrintTraits {
  using Helper = typename DigitTraitsHelper<i>::Type;
  using ReverseHelper = typename DigitVectorReverseHelper<
      typename DigitTraitsHelper<i>::Type>::Type;

  void operator()() {
    std::cout << " total: ";
    Helper::PrintDigitsCount();
    std::cout << " digits: ";
    Helper::PrintDigitsString();
    std::cout << " reverse: ";
    ReverseHelper::PrintDigitsString();
    std::cout << std::endl;
  }
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
  constexpr int size = 1000;
  if (c >= size)
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
