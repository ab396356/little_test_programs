//
// References to arrays, C++98.
//
//  http://stackoverflow.com/a/10008405
//  http://c-faq.com/decl/spiral.anderson.html
//  http://unixwiz.net/techtips/reading-cdecl.html
//

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
    //int (&ra2)[55]        = a; // compilation error
    int (&ra2)[55]          = get_array_ref();

    ra[0] = 234;
    //cra[0] = 234; // compilation error

    std::cout << "before calling `pass_array_ref()`:\n";
    std::cout << "a[0]      == " << a[0] << '\n';
    std::cout << "ra[0]     == " << ra[0] << '\n';
    std::cout << "cra[0]    == " << cra[0] << std::endl;

    pass_array_ref(a);
    //pass_array_ref(ra);   // OK but unnecessary
    //pass_array_ref(cra);  // compilation error
    //pass_array_ref(ra2);  // compilation error

    std::cout << "after calling `pass_array_ref()`:\n";
    std::cout << "a[0]      == " << a[0] << '\n';
    std::cout << "ra[0]     == " << ra[0] << '\n';
    std::cout << "cra[0]    == " << cra[0] << std::endl;

    std::cout << "before changing `get_array_ref()[3]`:\n";
    std::cout << "ra2[3]    == " << ra2[3] << '\n';
    get_array_ref()[3] = -1;
    std::cout << "after changing `get_array_ref()[3]`:\n";
    std::cout << "ra2[3]    == " << ra2[3] << std::endl;
}
