//
// Function-try-block, C++98.
//
//  http://en.cppreference.com/w/cpp/language/function-try-block
//

#include <iostream>
#include <stdexcept>

namespace
{

class throws_exception
{
public:

    throws_exception()
    {
        std::clog << __func__ << ": I am throwing!" << std::endl;
        throw std::logic_error("evil code!");
    }
};

class doesnt_catch_exception: public throws_exception
{
public:

    doesnt_catch_exception():
        throws_exception()
    {
        try
        {
            std::clog << __func__ << ": I'm trying!" << std::endl;
        }
        catch (const std::logic_error &e)
        {
            std::clog << __func__ << ": I caught: " << e.what() << std::endl;
        }
    }
};

class catches_exception: public throws_exception
{
public:

    catches_exception() try:
        throws_exception()
    {
        std::clog << __func__ << ": I'm trying!" << std::endl;
    }
    catch (const std::logic_error &e)
    {
        std::clog << __func__ << ": I caught: " << e.what() << std::endl;
    }
};

} // unnamed namespace

int main()
try
{
    catches_exception       ce;
    doesnt_catch_exception  dce;
}
catch (const std::logic_error &e)
{
    std::clog << __func__ << ": I caught: " << e.what() << std::endl;
}
