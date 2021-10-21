
#include <cmath>
#include <iostream>

template <char... Is>
struct DigitsVector {
  using Type = DigitsVector<Is...>;
  enum { DigitsCount = sizeof...(Is) };

  static void PrintDigitsString() { ((std::cout << (int)Is << " "), ...); }
  static void PrintDigitsCount() { std::cout << DigitsCount << " "; }

  constexpr const static char* NameTable[] = {
      "ones digit", "tens digit", "hundreds digit", "thousands digit",
      "ten thousands digit"};
  static_assert(sizeof...(Is) <= sizeof(NameTable), "e");

  inline constexpr static void PrintDigitsNameHelper(size_t index, char c) {
    std::cout << NameTable[index - 1] << " : " << (int)c << " ";
  }
  template <typename... Ps>
  inline constexpr static void PrintDigitsNameHelper(size_t index,
                                                     char c,
                                                     Ps&&... args) {
    std::cout << NameTable[index - 1] << " : " << (int)c << " ";
    PrintDigitsNameHelper(index - 1, std::forward<Ps>(args)...);
  }

  static void PrintDigitsStringName() {
    PrintDigitsNameHelper(sizeof...(Is), Is...);
  }
};

template <int IntVal, typename = DigitsVector<>>
struct DigitTraitsHelper;

template <int IntVal, char... Is>
struct DigitTraitsHelper<IntVal, DigitsVector<Is...>>
    : DigitTraitsHelper<IntVal / 10, DigitsVector<IntVal % 10, Is...>> {};

template <int... Is>
struct DigitTraitsHelper<0, DigitsVector<Is...>> : DigitsVector<Is...> {};

template <typename T, char... Rs>
struct DigitVectorReverseHelper;

template <int IntVal, char... Is, char... Rs>
struct DigitVectorReverseHelper<DigitsVector<IntVal, Is...>, Rs...>
    : DigitVectorReverseHelper<DigitsVector<Is...>, IntVal, Rs...> {};

template <char... Rs>
struct DigitVectorReverseHelper<DigitsVector<>, Rs...> : DigitsVector<Rs...> {};

template <int i>
struct PrintTraits {
  using Helper = typename DigitTraitsHelper<i>::Type;
  using ReverseHelper = typename DigitVectorReverseHelper<
      typename DigitTraitsHelper<i>::Type>::Type;

  void operator()() {
    std::cout << " total: ";
    Helper::PrintDigitsCount();
    // std::cout << " digits: ";
    Helper::PrintDigitsStringName();
    std::cout << " in order: ";
    Helper::PrintDigitsString();
    std::cout << " reverse: ";
    ReverseHelper::PrintDigitsString();
    std::cout << std::endl;
  }
};

template <>
struct PrintTraits<0> {
  void operator()() {
    std::cout << " total: 1 ones digit: 0 in order: 0 reverse: 0 " << std::endl;
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
  constexpr int size = 100;
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
