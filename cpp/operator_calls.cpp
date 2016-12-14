//
// Operators function call syntax, C++98.
//
//  http://en.cppreference.com/w/cpp/language/operators
//  http://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
//  http://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt2
//  http://en.cppreference.com/w/cpp/language/adl
//

#include <iostream>

namespace
{

class number
{
public:

    // defined as member function
    bool operator != (const number &) const
    {
        return true;
    }
};

// defined as non-member function
bool operator == (const number &, const number &)
{
    return true;
}

} // unnamed namespace

int main()
{
    // first calls the non-member overload:
    //
    //  std::operator << (ostream &, const char *)
    //
    operator << (std::clog, "needlessly verbose\n");
    operator << (std::clog, "an int: ").operator << (130);

    // starting `std::` unnecessary thanks to Argument-Dependent Lookup (ADL)
    std::operator << (std::clog, '\n');

    // this will compile OK, but will treat the argument as a pointer
    // and print the address of the string literal, not its contents,
    // because it first calls the member overload:
    //
    //  std::ostream::operator << (const void *)
    //
    std::clog.operator << ("hello?").operator << (std::endl);

    number n1, n2;

    n1.operator != (n2);    // same as `n1 != n2;`
    operator == (n1, n2);   // same as `n1 == n2;`
}
