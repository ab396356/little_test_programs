//
// Reference-qualified non-static member functions, C++11.
//
//  http://en.cppreference.com/w/cpp/language/member_functions
//  http://en.cppreference.com/w/cpp/utility/move
//

#include <iostream>
#include <utility>

namespace
{

class ghost_busters
{
public:

    void who_you_gonna_call() const &
    {
        std::clog << "ghost busters!" << std::endl;
    }

    void who_you_gonna_call() const &&
    {
        std::clog << "GHOST BUSTERS!" << std::endl;
    }
};

} // unnamed namespace

int main()
{
    ghost_busters gb;

    gb.who_you_gonna_call();
    std::move(gb).who_you_gonna_call();
    ghost_busters().who_you_gonna_call();
}
