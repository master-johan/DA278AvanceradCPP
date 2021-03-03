#include <cassert>
#define LEVEL 7
#define VG true

#if LEVEL>=1
#include "Add.hpp"
void TestAdd() {
    assert(AddFun(3, 5) == 8);
    int x = Add<3, 5>::value;
    assert((Add<3, 5>::value) == 8);
}
#endif

#if LEVEL>=2
#include "Ackermann.hpp"
void TestAckermann() {
    int x = Ackermann<0, 1>::value; //should be 2
    x = Ackermann<1, 0>::value; //should be 2
    assert((Ackermann<0, 1>::value == 2));
    assert((Ackermann<1, 0>::value == 2));
    assert((Ackermann<3, 4>::value == 125));
    assert((Ackermann<4, 0>::value == 13));
    //assert((Ackermann<4, 1>::value == 65533));
}
#endif

#include <type_traits>
#define AssertSame(X,Y) static_assert((std::is_same<X, Y>::value));

#if LEVEL>=3
#include "Simplify.hpp"

void TestSimplify() {
    AssertSame(int, SimplifyType<int*>::type);
    AssertSame(int, SimplifyType_t<int*>);
    AssertSame(int, SimplifyType_t<int&>); 
    //AssertSame(int, SimplifyType_t<int[]>);
    AssertSame(int** const*, SimplifyType_t<int** const**>);
    AssertSame(const int, SimplifyType_t<const int&>);
    SimplifyType_t<const int&> x = 0;
    SimplifyType_t<int*> y = 0;
    SimplifyType_t<const int> z = 0;

}
#endif

#if LEVEL>=4
#include "Simplify.hpp"

void TestBase() {
    AssertSame(int, int);
    AssertSame(int, BaseType_t<int*>);
    AssertSame(int, BaseType_t<int&>);
    AssertSame(int, BaseType_t<int[]>);
    AssertSame(int, BaseType_t<int[6]>);
    AssertSame(int, BaseType_t<int***&>);
    AssertSame(int, BaseType_t<const int** const**&>);
    AssertSame(BaseType_t<const int>, BaseType_t< BaseType_t<const int>**>);
   
}

#endif

#if LEVEL>=5 && VG
#include "RemAllConst.hpp"

void TestRemoveAllConst() {
    AssertSame(int, RAC<int>::type);
    AssertSame(int, RAC<const int>::type);
    AssertSame(int*, RAC<int*>::type);
#pragma warning( push )
#pragma warning( disable : 4114)
    AssertSame(int***, RAC<const int const* const* const* const>::type);
#pragma warning( pop ) 
}
#endif

#if LEVEL>=6
#include "Sum.hpp"
#include <vector>
void TestSum() {
    std::vector<int> v = { 1,2,3,-9 };
    int sum = Sum(v);
    assert(sum == -3);
}
#endif

#if LEVEL>=7
#include "SFINAE.hpp"
#include <string>
using std::string;
void TestSFINAE() {
    int i(3);
    assert(NoThrowCopyConstructible(i));
    auto str = string("hej");
    assert(!NoThrowCopyConstructible(str));
}
#endif



int main() {
#if LEVEL>=1
    TestAdd();
#endif
#if LEVEL>=2
    TestAckermann();
#endif
#if LEVEL>3
    TestSimplify();
#endif
#if LEVEL>=4
    TestBase();
#endif
#if LEVEL>5 && VG
    TestRemoveAllConst();
#endif
#if LEVEL>5
    TestSum();
#endif
#if LEVEL>6
    TestSFINAE();
#endif
}