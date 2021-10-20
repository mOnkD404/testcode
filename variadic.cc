
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

template <int... Is>
struct IntVector
{
    using Type = IntVector<Is...>;
};

template <int I_Size, typename T_Vector = IntVector<>>
struct Iota;
template <int I_Size, int... Is>
struct Iota<I_Size, IntVector<Is...>> : Iota<I_Size - 1, IntVector<I_Size - 1, Is...>>
{
};
template <int... Is>
struct Iota<0, IntVector<Is...>> : IntVector<Is...>
{
};

template <typename T_Indexes>
struct PrintTable;
template <int... Is>
struct PrintTable<IntVector<Is...>>
{
    static void at(const char in)
    {
        static const std::function<void()> table[] = {typename PrintTraits<Is>::Type{}...};
        table[in]();
    }
};

void PrintInt(int c)
{
    using Table = PrintTable<typename Iota<999>::Type>;
    Table::at(c);
}

int main(int argc, char *argv[])
{
    PrintInt(123);
    return 0;
}
