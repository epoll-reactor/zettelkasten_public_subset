#include <string>
#include <cstring>
#include <cassert>
#include <iostream>

using namespace std;

int main()
{
    // ci = case insensitive.
    // Стоит обратить внимание, что объекты, наследуемые
    // от std::char_traits не используются полиморфно, а
    // наследование используется лишь с целью показания
    // того, какой функционал мы хотим изменить, определяя
    // определённые функции.
    struct ci_char_traits : std::char_traits<char>
    {
        // std::string реализует сравнение используя функции
        // eq() и lt() из std::char_traits (или его модификаций).
        static bool eq(char lhs, char rhs)
            { return toupper(lhs) == toupper(rhs); }
        static bool lt(char lhs, char rhs)
            { return toupper(lhs)  < toupper(rhs); }

        static int compare(const char *lhs_str,
                           const char *rhs_str,
                           size_t      len)
        {
            while (len-- != 0)
            {
                if (toupper(*lhs_str) < toupper(*rhs_str)) return -1;
                if (toupper(*lhs_str) > toupper(*rhs_str)) return  1;
                ++lhs_str;
                ++rhs_str;
            }
            return 0;
        }

        static const char *find(const char *str, int len, char ch)
        {
            while (len-- > 0 && toupper(*str) != toupper(ch))
                ++str;
       	    return len >= 0 ? str : nullptr;
        }
    };

    using ci_string = basic_string<char, ci_char_traits>;
    ci_string s("AbCdE");
    assert(s == "abcde");
    assert(s == "ABCDE");
    assert(strcmp(s.c_str(), "AbCdE") == 0);
    assert(strcmp(s.c_str(), "abcde") != 0);

    // Оператор вывода выглядит так:
    // template <typename CharT,
    //           typename Traits,
    //           typename Allocator>
    // basic_ostream<CharT, Traits> operator<<(
    //     basic_ostream<CharT, Traits>                 &os,
    //     const basic_string<CharT, Traits, Allocator> &str
    // )
    //
    // cout является шаблоном basic_ostream<char, char_traits<char>>.
    // Следовательно, причина ошибки компиляции ясна.
    //
    // Имеется 2 решения:
    // 1. Определить операторы << и >> для ci_string.
    // 2. Использовать в этих операторах .c_str().
    // @{
    cout << s.c_str() << std::endl;
    // @}
}
