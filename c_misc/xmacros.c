//
// X Macros, C90.
//
//  https://en.wikipedia.org/wiki/X_Macro
//  http://www.drdobbs.com/the-new-c-x-macros/184401387
//  http://www.drdobbs.com/cpp/the-x-macro/228700289
//
// X macros are a metaprogramming technique used to generate repetitive code
// that would otherwise be tedious to write and maintain by hand.
//
// In the example code below, consider how straightforward it is to add or
// remove colors and their numbers and then have the correct code expanded
// without further oversight.
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

    COLORS_AND_NUMBERS {
        printf("unknown color\n");
    }

#undef X

    //
    // the above expands roughly to:
    //
    //  if (n == (100)) {
    //      printf("color: %s\n", ("red"));
    //  } else
    //  if (n == (-23)) {
    //      printf("color: %s\n", ("green"));
    //  } else
    //  if (n == (55)) {
    //      printf("color: %s\n", ("blue"));
    //  } else {
    //      printf("unknown color\n");
    //  }
    //
}
