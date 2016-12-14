//
// The `template` disambiguator for dependent names, C++98.
//
//  http://en.cppreference.com/w/cpp/language/dependent_name
//

namespace
{

template <typename Obj>
class type_obj_cls
{
public:

    template <typename Fail>
    void type_fail_func() const
    {
    }

    template <typename Ok>
    void type_ok_func(const Ok &) const
    {
    }
};

template <typename T>
void user_func()
{
    type_obj_cls<T> obj;

    //obj.type_fail_func<double>(); // compilation error
    obj.template type_fail_func<double>();

    obj.type_ok_func("hello?");
    //obj.template type_ok_func("hello?"); // unnecessary
}

} // unnamed namespace

int main()
{
}
