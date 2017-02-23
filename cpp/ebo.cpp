//
// Empty Base Optimization, C++98.
//
//  http://en.cppreference.com/w/cpp/language/ebo
//  https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Empty_Base_Optimization
//

#include <cassert>

namespace
{

class base
{
};

class derived: public base
{
private:

    char arr[10];
};

} // unnamed namespace

int main()
{
    assert(sizeof (base) == 1);
    assert(sizeof (derived) == 10); // not "11" due to EBO
}
