//
// `static` meaning "minimum size" in array declarations, C90.
//
//  http://en.cppreference.com/w/c/language/array
//  http://stackoverflow.com/q/3430315
//  http://c-faq.com/aryptr/aryparmsize.html
//

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

//
// the keyword `static`, as used inside the array declaration below, has
// the meaning of:
//  "`arr` is expected to be able to hold at least `10` elements"
// thus hinting optimizations to the compiler
//
static void func_ok(int arr[static 10])
{
    arr[9] = 42; // this is expected to be safe to do

    //
    // note that if the programmer supplies an array that cannot hold at least
    // 10 elements, the behavior is still undefined, because the compiler is not
    // currently (2016) required by the Standard to actually check and warn if
    // the array isn't large enough!
    //
}

static void func_fail(int arr[10])
{
    arr[9] = 42; // oops? maybe?
}

//
// note that the compiler interprets the above function definitions as:
//
//  static void func_ok(int *arr)       // with optimization hints for `arr`
//  static void func_fail(int *arr)     // without optimization hints for `arr`
//

int main(void)
{
    int *arr = malloc(3 * sizeof (int));

    assert(arr != NULL);
    //func_ok(arr);     // Undefined Behavior
    //func_fail(arr);   // Undefined Behavior

    arr = realloc(arr, 10 * sizeof (int));
    assert(arr != NULL);
    func_ok(arr);       // OK
    func_fail(arr);     // OK
    free(arr);
}
