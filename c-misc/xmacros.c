//
// X Macros, C90.
//
//  https://en.wikipedia.org/wiki/X_Macro
//  http://www.drdobbs.com/the-new-c-x-macros/184401387
//

#include <stdio.h>

#define COLORS_AND_NUMBERS                                                     \
    X("red", 100)                                                              \
    X("green", -23)                                                            \
    X("blue", 55)

int main(void)
{
    const int n = 55;

#define X(Color, Number)                                                       \
    if (n == (Number)) {                                                       \
        printf("color: %s\n", (Color));                                        \
    } else

    COLORS_AND_NUMBERS {}

#undef X

    //
    // the above expands roughly to:
    //
    //  if (n == (100)) {
    //      printf("color: %s\n", ("red"));
    //  } else if (n == (-23)) {
    //      printf("color: %s\n", ("green"));
    //  } else if (n == (55)) {
    //      printf("color: %s\n", ("blue"));
    //  } else {}
    //
}
