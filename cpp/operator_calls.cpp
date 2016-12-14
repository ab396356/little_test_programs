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
    number n1, n2;

    n1.operator != (n2);    // same as `n1 != n2;`
    operator == (n1, n2);   // same as `n1 == n2;`

    // first calls the non-member overload:
    //
    //  std::operator << (ostream &, const char *)
    //
    operator << (std::clog, "needlessly verbose\n");
    operator << (std::clog, "an int: ").operator << (130);

    // starting `std::` unnecessary thanks to Argument-Dependent Lookup (ADL)
    std::operator << (std::clog, '\n');

    // first calls the member overload:
    //
    //  std::ostream::operator << (const void *)
    //
    // which will treat the string as a pointer and proceed to print
    // a memory address
    //
    std::clog.operator << ("hello?").operator << (std::endl);
}
