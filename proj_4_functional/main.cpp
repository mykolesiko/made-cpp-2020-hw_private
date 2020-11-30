#include "function.h"
#include "compose.h"


class mathem
{
  public:
    mathem(int koeff_, int compare_): koeff(koeff_), compare(compare_){};
    int koeff;
    int compare;
    bool more(int a, int b) {return a * b > compare;};
    int product(int a) {return koeff * a;};
};


class callable_class {
  private:
    int a, b;
  public:
    callable_class(int a_, int b_): a(a_), b(b_) {};
    int operator() () {
        return (a + b);
    };
};


void FailWithMsg(const std::string& msg, int line) {
    std::cerr << "Test failed!\n";
    std::cerr << "[Line " << line << "] "  << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

#define ASSERT_TRUE(cond) \
    if (!(cond)) {FailWithMsg("Assertion failed: " #cond, __LINE__);};

#define ASSERT_TRUE_MSG(cond, msg) \
    if (!(cond)) {FailWithMsg(msg, __LINE__);} else std::cout << "Test passed " << msg << std::endl;

#define ASSERT_EQUAL_MSG(cont1, cont2, msg) \
    ASSERT_TRUE_MSG(std::equal(cont1.begin(), cont1.end(), cont2.begin(), cont2.end()), msg)

int func (int a) {
    return a;
}

void tests() {

    function<int (int, int)> func1 = [] (int x, int y) -> int {
        return x + y;
    };

    std::function<int (int, int)> func1_std = [] (int x, int y) -> int {
        return x + y;
    };

    ASSERT_TRUE_MSG((func1(2, 4) == func1_std(2, 4)) && (func1(2, 4) == 6) , "Test1 function from lambda");

    //int a = test_func("string");

    function<int(int, int)> func2(func1);
    std::function<int(int, int)> func2_std(func1_std);

    ASSERT_TRUE_MSG((func2(2, 4) == func2_std(2, 4)) && (func2(2, 4) == 6) , "Test2 copy constructor ");

    function<bool (int, int)> func3 = ([](int a, int b) { return a < b; });
    std::function<bool (int, int)> func3_std = ([](int a, int b) { return a < b; });
    function<bool (int, int)> func4;
    std::function<bool (int, int)> func4_std;
    func4 = func3;
    func4_std = func3_std;

    ASSERT_TRUE_MSG((func4(2, 40) == func4_std(2, 40)) && (func4(2, 40) == true), "Test3 operator=(copy)");

    func4 = std::move(func3);
    func4_std = std::move(func3_std);

    ASSERT_TRUE_MSG((func4(200, 40) == func4_std(200, 40)) && (func4(200, 40) == false), "Test4 operator=(move)");

    function<bool(mathem, int, int)> member_func_more1 = &mathem::more;
    function<int(mathem, int)> member_func_product1 = &mathem::product;


    std::function<bool(mathem, int, int)> member_func_more1_std = &mathem::more;
    std::function<int(mathem, int)> member_func_product1_std = &mathem::product;

    mathem m1(3, 4);

    std::function<bool(mathem, int, int)> member_func_more2_std(member_func_more1_std);
    std::function<int(mathem, int)> member_func_product2_std(member_func_product1_std);


    function<bool(mathem, int, int)> member_func_more2(member_func_more1);
    function<int(mathem, int)> member_func_product2(member_func_product1);


    ASSERT_TRUE_MSG((member_func_more1(m1, 2, 40) == member_func_more1_std(m1, 2, 40)) && (member_func_more1(m1, 2, 40) == true), "Test5 class member function");
    ASSERT_TRUE_MSG((member_func_more2(m1, 2, 40) == member_func_more2_std(m1, 2, 40)) && (member_func_more2(m1, 2, 40) == true), "Test5 copy constructer from member function ");

    ASSERT_TRUE_MSG((member_func_product1(m1, 2) == member_func_product1_std(m1, 2)) && (member_func_product1(m1, 2) == 6), "Test6 class member function");
    ASSERT_TRUE_MSG((member_func_product2(m1, 2) == member_func_product2_std(m1, 2)) && (member_func_product2(m1, 2) == 6), "Test6 copy constructer from member function");


    callable_class cl(5, 6);
    function<int()> func_from_class = cl;
    std::function<int()> func_from_class_std = cl;


    ASSERT_TRUE_MSG(func_from_class() == func_from_class_std(), "Test7 function from callable class");

    function<int()> func_from_class1(func_from_class);
    std::function<int()> func_from_class1_std(func_from_class_std);

    ASSERT_TRUE_MSG(func_from_class1() == func_from_class1_std(), "Test8 copy consructor from callable class function");


    function<int()> func_from_class3;
    std::function<int()> func_from_class3_std;

    func_from_class3 = func_from_class;
    func_from_class3_std = func_from_class_std;

    ASSERT_TRUE_MSG(func_from_class3() == func_from_class3_std(), "Test9 operator==(&) for callable class function");

    func_from_class3 = std::move(func_from_class);
    func_from_class3_std = std::move(func_from_class_std);
    ASSERT_TRUE_MSG(func_from_class3() == func_from_class3_std(), "Test10 operator=(&&) for callable class function");

    ASSERT_TRUE_MSG((!func_from_class), "Test12 check bool operator  ");
    ASSERT_TRUE_MSG((!func_from_class_std), "Test13 check bool operator std" );

    callable_class cl1(4, 5);
    func_from_class = cl1;
    func_from_class_std = cl1;

    func_from_class.swap(func_from_class3);
    func_from_class_std.swap(func_from_class3_std);

    ASSERT_TRUE_MSG((func_from_class3() == func_from_class3_std()) && (func_from_class3() == 9), "Test14 check swap");
}


//double f(double x){ return x * 1,1.;};

double func_int_to_double (int a) {
    return 2 * a;
}

float func_double_to_float (double a) {
   return((float)a * 3.5);
}

int func_float_to_int (float a) {
    return ((int) a + 10);
}

bool func_with_many_arguments(int a, int b) {
    return (a < b);
}


void test_compose() {
    const std::function<bool(int)> a = [] (int x) -> bool {
        bool result = (x > 10);
        //std::cout << "a" << " in = " << x << "result = " << result << std::endl;
        //std::cout << "a" << std::endl;
        return result;
    };
    const std::function<int(float)> b = [] (float y) -> int {
        int result = int(y * y);
        //std::cout << "b" << std::endl;
        //std::cout << "b" << " in = " << y << "result = " << result << std::endl;
        return result;
    };
    const std::function<float(bool)> c = [] (bool z) -> float {
        float result = z ? 3.1f : 3.34f;
        //std::cout << "c" << " in = " << z << "result = " << result << std::endl;
        return result;
    };
    auto d = compose(a, b, c, a, b, c);
    //std::cout << d(true) << std::endl;
    ASSERT_TRUE_MSG(d(true) == true, "compose for std::function 1");

    std::function<decltype(func_int_to_double)> func1 = func_int_to_double;
    std::function<decltype(func_double_to_float)> func2 = func_double_to_float;
    std::function<decltype(func_float_to_int)> func3 = func_float_to_int;

    auto d2 = compose(func3, func2, func1);
    ASSERT_TRUE_MSG(d2(1) == 17, "compose for std::function 2");

    auto d3 = compose(func_float_to_int, func_double_to_float, func_int_to_double);
    ASSERT_TRUE_MSG(d3(1) == 17, "compose for ordinary function 1");


    function<bool(int)> a1 = [] (int x) -> bool {
      //  std::cout << "a ";
        return x > 10;
    };

    function<int(float)> b1 = [] (float y) -> int {
        //std::cout << "b ";
        return int(y * y);
    };

    function<float(bool)> c1 = [] (bool z) -> float {
    //    std::cout << "c ";
        return z ? 3.1f : 3.34f;
    };

    function<decltype(func_with_many_arguments)> func4 = func_with_many_arguments;
    auto d4 = compose(a1, b1, c1, a1, b1, c1, func4);

    ASSERT_TRUE_MSG(d4(2, 4) == true, "compose for function with many arguments");
    compose();
}

int main () {

    test_compose();
    tests();

    return 0;
}

