
#include <iostream>

template <int i>
struct PrintTraitsHelper;

template <>
struct PrintTraitsHelper<0>
{
    void operator()()
    {
    }
};

template <int i>
struct PrintTraitsHelper
{
    enum
    {
        value = i % 10
    };
    void operator()()
    {
        std::cout << value << " ";
        PrintTraitsHelper<i / 10>{}();
    }
};

template <int i>
struct PrintTraits
{
    using Type = PrintTraitsHelper<i>;
};

// template <int... Is>
// struct IntVector
// {
//     using Type = IntVector<Is...>;
// };

// template <int I_Size, typename T_Vector = IntVector<>>
// struct Iota;
// template <int I_Size, int... Is>
// struct Iota<I_Size, IntVector<Is...>> : Iota<I_Size - 1, IntVector<I_Size - 1, Is...>>
// {
// };
// template <int... Is>
// struct Iota<0, IntVector<Is...>> : IntVector<Is...>
// {
// };

// template <typename T_Indexes>
// struct PrintTable;
// template <int... Is>
// struct PrintTable<IntVector<Is...>>
// {
//     static void at(const char in)
//     {
//         static const std::function<void()> table[] = {typename PrintTraits<Is>::Type{}...};
//         table[in]();
//     }
// };

template <typename T>
struct PrintTable;
template <size_t... N>
struct PrintTable<std::index_sequence<N...>>
{
    static void at(const int in)
    {
        static const std::function<void()> table[] = {typename PrintTraits<N>::Type{}...};
        table[in]();
    }
};

void PrintInt(int c)
{
    // using Table = PrintTable<typename Iota<99999>::Type>;
    constexpr int size = 2048;
    if (c > size)
        return;

    using Table = PrintTable<std::make_index_sequence<size>>;
    Table::at(c);
}

int main(int argc, char *argv[])
{
    PrintInt(123);
    return 0;
}
