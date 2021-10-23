
#include <array>
#include <cmath>
#include <iostream>
#include <type_traits>

template <char... Is>
struct DigitsVector {
  using Type = DigitsVector<Is...>;
  enum { DigitsCount = sizeof...(Is) };
  static void PrintDigitsString() { ((std::cout << (int)Is << " "), ...); }
  static void PrintDigitsCount() { std::cout << DigitsCount << " "; }
};

template <typename T, typename N>
struct PrintDigitsNameHelper;

template <char... Is, size_t... N>
struct PrintDigitsNameHelper<DigitsVector<Is...>, std::index_sequence<N...>> {
  constexpr const static char* NameTable[] = {
      "ones digit", "tens digit", "hundreds digit", "thousands digit",
      "ten thousands digit"};

  constexpr static size_t C = sizeof...(N);
  static_assert(C <= std::extent<decltype(NameTable)>::value, "overflow");
  static void Print() {
    ((std::cout << NameTable[C - N - 1] << " : " << (int)Is << " "), ...);
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

template <int N>
struct PrintTraits {
  using Helper = typename DigitTraitsHelper<N>::Type;
  using ReverseHelper = typename DigitVectorReverseHelper<
      typename DigitTraitsHelper<N>::Type>::Type;
  using PrintDigitsName =
      PrintDigitsNameHelper<Helper,
                            std::make_index_sequence<Helper::DigitsCount>>;

  static void Print() {
    std::cout << " total: ";
    Helper::PrintDigitsCount();
    PrintDigitsName::Print();
    std::cout << " in order: ";
    Helper::PrintDigitsString();
    std::cout << " reverse: ";
    ReverseHelper::PrintDigitsString();
    std::cout << std::endl;
  }
};

template <>
struct PrintTraits<0> {
  static void Print() {
    std::cout << " total: 1 ones digit: 0 in order: 0 reverse: 0 " << std::endl;
  }
};

template <typename T>
struct PrintTable;
template <size_t... N>
struct PrintTable<std::index_sequence<N...>> {
  static void at(const int in) {
    constexpr size_t C = sizeof...(N);
    static const std::array<void (*)(), C> table{PrintTraits<N>::Print...};
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
