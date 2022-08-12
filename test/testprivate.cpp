#include <type_traits>
#include <iostream>

// Your class
class sample_class{
    void private_method(){
        std::cout << "Private method called" << std::endl;
    }
};

// declare method's type
template<typename TClass>
using method_t = void (TClass::*)();

// helper structure to inject call() code
template<typename TClass, method_t<TClass> func>
struct caller{
    friend void call(){
        TClass obj;
        (obj.*func)();
    }
};

// even instantiation of the helper
template struct caller<sample_class,&sample_class::private_method>;

// declare caller
void call();

int main(){
    //call(); // and call!
    caller<sample_class,&sample_class::private_method>()
    return 0;
}
