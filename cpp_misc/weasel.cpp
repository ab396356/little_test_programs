//
// Weasel Program, written in "gone-full-retard" C++14!
//
//  https://en.wikipedia.org/wiki/Weasel_program
//
// The Weasel program is a thought experiment and a very rough simulation of
// evolution, by starting with a random string of characters and gradually
// "evolving" them into the text "methinks it is like a weasel".
//
// The program can be easily modified to generate text that satisfies other
// criteria, e.g. being a palindrome, thus not choosing one particular output.
//
// The starting text is random and is mutated randomly (mutation) and it is
// then scored for suitability (natural selection), resulting in a non-random
// output (evolution).
//

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{

///
/// @brief Global target string.
/// @todo Doesn't need to be in unnamed namespace: const's have static linkage.
///
const std::string methinks("methinks it is like a weasel");

///
/// @brief Program modes.
/// @note Uncomment the one you want used, comment all the others.
/// @todo These should be strongly-typed `enum`s used together with a
///  "static if" mechanism, such as `if constexpr` in C++17.
///
#define MODE_WEASEL
//#define MODE_PALINDROME

///
/// @brief Gives the suitability score of a candidate string.
/// @details A perfect score equals `0` (zero); the higher the worse.
/// @todo Use `if constexpr` instead of `#ifdef` once C++17 becomes available.
/// @note This function was designed to be general such that it can be modified
///  easily to score things other than resemblance to the "weasel" text, while
///  retaining the interface and usage: for example, it could score how good
///  a palindrome the `candidate` string is (as already implemented).
/// @param [in] candidate           Candidate string.
/// @returns Suitability score.
/// @retval 0                       Perfect score.
///
std::size_t score(const std::string &candidate)
try
{
#if defined MODE_WEASEL
    if (candidate.length() != methinks.length())
        return std::numeric_limits<std::size_t>::max();

    std::size_t r(methinks.length());

    for (std::size_t i(0); i < methinks.length(); ++i)
        if (candidate.at(i) == methinks.at(i))
            --r;

    return r;
#elif defined MODE_PALINDROME
    auto            ib  (candidate.cbegin());
    const auto      ie  (candidate.cend());
    auto            rib (candidate.crbegin());
    std::size_t     r   (candidate.length());

    for (; ib != ie; ++ib, ++rib)
        if (*ib == *rib)
            --r;

    return r;
#endif
}
catch (const std::out_of_range &e)
{
    std::cerr << "`std::out_of_range` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    throw;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    throw;
}

} // unnamed namespace

///
/// @brief Enters the program.
/// @returns Whether or not the operation was successful.
/// @retval EXIT_SUCCESS            Operation success.
/// @retval EXIT_FAILURE            Operation failure.
///
int main()
try
{
    // Number of Candidate strings in a Generation
    constexpr std::size_t num_cagen(100);

    std::size_t calen(0); // Candidate string Length

#if defined MODE_WEASEL
    calen = methinks.length();
#elif defined MODE_PALINDROME
    calen = 50;
#endif

    //
    // create a candidate string
    //

    // Pseudo-Random Number Generator
    std::mt19937 prng(
        std::chrono::system_clock::now().time_since_epoch().count());

    // Random Character
    std::uniform_int_distribution<char> rand_char(
        std::numeric_limits<char>::min(),
        std::numeric_limits<char>::max());

    // Random chance to Mutate (0 == no, 1 == yes)
    std::discrete_distribution<unsigned long int> rand_mutate({
        95.0,       // 0
         5.0});     // 1

    // Generate random Printable Character
    std::function<char (void)> gen_print_char =
        [&prng, &rand_char]() -> char
        {
            char c;

            do
            {
                c = rand_char(prng);
            }
            while (!std::isprint(c, std::locale::classic()));

            return c;
        };

    std::string candidate(calen, char(0)); // Candidate string

    std::generate(candidate.begin(), candidate.end(), gen_print_char);

    // the current Candidates Generation
    std::vector<std::string> cagen(num_cagen, candidate);

    // Value passed to `std::setw()` to prettify printing
    constexpr std::size_t val_setw(10);

    std::cout << std::setw(val_setw) << "FIRST: " << candidate << '\n';

    std::size_t kgen(1); // Generation counter

    while (score(candidate) != 0)
    {
        for (std::string &ca: cagen)
            for (char &c: ca)
                if (rand_mutate(prng) != 0)
                    c = gen_print_char();

        for (const std::string &ca: cagen)
            if (score(ca) < score(candidate))
            {
                candidate = ca;
                std::cout << std::setw(val_setw) << std::to_string(kgen) + ": ";
                std::cout << candidate << '\n';
            }

        std::fill(cagen.begin(), cagen.end(), candidate);
        ++kgen;
    }

    std::cout << std::setw(val_setw) << "LAST: " << candidate << std::endl;
    return EXIT_SUCCESS;
}
catch (const std::out_of_range &e)
{
    std::cerr << "`std::out_of_range` exception in `" << __func__ << "`: ";
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "unknown exception in `" << __func__ << '`' << std::endl;
    return EXIT_FAILURE;
}
