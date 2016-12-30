//
// `const`, `volatile` and `restrict` qualifiers used in array parameter
// declarations, C99.
//
// The aforementioned qualifiers are used to qualify the pointer type
// to which the array parameter type is decayed.
//
//  http://en.cppreference.com/w/c/language/array
//  http://c-faq.com/aryptr/aryptrparam.html
//

extern void     func0(int arr[]);                   // same as
// extern void  func0(int arr[10]);                 // same as
// extern void  func0(int *arr);

extern void     func1(int arr[const]);              // same as
// extern void  func1(int * const arr);

extern void     func2(int arr[const volatile]);     // same as
// extern void  func2(int * const volatile arr);

extern void     func3(int arr[restrict]);           // same as
// extern void  func3(int * restrict arr);

extern void     func4(int arr[restrict static 10]); // same as
// extern void  func4(int * restrict arr);          // with optimization hints

int main(void)
{
}
