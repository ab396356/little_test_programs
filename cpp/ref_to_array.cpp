//
// References to arrays, C++98.
//
//  http://stackoverflow.com/a/10008405
//  http://c-faq.com/decl/spiral.anderson.html
//  http://unixwiz.net/techtips/reading-cdecl.html
//

#include <cassert>
#include <iostream>

namespace
{

void pass_array_ref(int (&ra)[100])
{
    ra[0] = -456;
}

int (&get_array_ref())[55]
{
    static int a2[55];

    return a2;
}

} // unnamed namespace

int main()
{
    int a[100];

    int (&ra)[100]          = a;
    const int (&cra)[100]   = a;
    //int (&ra2)[55]        = a; // compilation error: different types
    int (&ra2)[55]          = get_array_ref();

    ra[0] = 234;
    //cra[0] = 234; // compilation error: assign to const

    // before calling `pass_array_ref()`
    assert(a[0]     == 234);
    assert(ra[0]    == 234);
    assert(cra[0]   == 234);

    pass_array_ref(a);
    //pass_array_ref(ra);   // OK but unnecessary
    //pass_array_ref(cra);  // compilation error: assign to const
    //pass_array_ref(ra2);  // compilation error: different types

    // after calling `pass_array_ref()`
    assert(a[0]     == -456);
    assert(ra[0]    == -456);
    assert(cra[0]   == -456);

    // before changing `get_array_ref()[3]`
    assert(ra2[3]   == 0);

    get_array_ref()[3] = -1;

    // after changing `get_array_ref()[3]`
    assert(ra2[3]   == -1);
}
