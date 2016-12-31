//
// Duff's Device, pre-C90.
//
//  https://en.wikipedia.org/wiki/Duff%27s_device
//  http://c-faq.com/misc/duff.html
//  http://c-faq.com/misc/duffexpln.html
//  https://en.wikipedia.org/wiki/Loop_unrolling
//
// "Duff's device" is a construct that combines `switch` and a loop to perform
// manual loop unrolling. It was discovered by Tom Duff in 1983 and was used to
// improve the speed of his real-time animation program.
//
// Nowadays (2016), modern C compilers can perform loop unrolling automatically
// as an optimization option, rendering Duff's device an obsolete curiosity.
//

#include <stdio.h>

///
/// @brief Prints numbers from `0` to `n` (including).
/// @details This function uses a `do-while` loop Duff device that unrolls by
///  a factor of 4 (prints four numbers at a time if possible).
/// @param [in] n                   Maximum number to print.
///
static void print_numbers(unsigned long int n)
{
    //
    // Duff's device is made possible by two language features:
    //
    //  (1) the actual syntax of the `switch()` statement is:
    //
    //      switch (expression)
    //          statement
    //
    //      //
    //      // meaning that code such as below is legal, even though it will
    //      // do nothing due to missing an appropriate `case 0:` label:
    //      //
    //
    //      switch (0)
    //          printf("nothing");
    //
    //  (2) `case` labels will be followed by `switch()` even if they are
    //      planted deeper than the first block of code, e.g.:
    //
    //      switch (Letter)
    //      {
    //          case 'A': ...; break; // normal stuff
    //          case 'B': ...; break;
    //
    //          for (...)
    //          {
    //              if (...)
    //              {
    //                  //
    //                  // the `switch()` can and will jump in here!
    //                  //
    //                  case 'C': ...;
    //              }
    //          }
    //      }
    //

    unsigned long int i=0;

    switch ((n + 1) % 4)
    {
        case 0:     do {    printf("%4lu ",     i++);
        case 3:             printf("%4lu ",     i++);
        case 2:             printf("%4lu ",     i++);
        case 1:             printf("%4lu\n",    i++);
                    } while (i < n);
    }
}

int main(void)
{
    print_numbers(22);
}
