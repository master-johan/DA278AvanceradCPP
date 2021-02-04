#include "TestLevel.h"

#if (defined(LEVEL)  && (LEVEL>20) ||  !defined(LEVEL))   && (defined(VG) || defined(G))

#include "Vector.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <cassert>
#include<random>

#pragma region help jox

using it = Vector<char>::iterator;
using cit = Vector<char>::const_iterator;
//using rit = Vector<char>::reverse_iterator;
//using crit = Vector<char>::const_reverse_iterator;

template<class T>
struct IsSameFun {
    template<class V>
    static void e(V v) {
        static_assert(std::is_same<T, V>::value, "Not right return type");
    }
};

template<class T, class U>
struct IsSameS {
    static_assert(std::is_same_v<T, U>, "Not right type");
    static const bool value = true;
};

template< class T, class U >
constexpr bool IsSameCheck = IsSameS<T, U>::value;

struct D {
    static std::string usedConstr;

    void Test() {}
    D() {
        usedConstr += "DC";
    }
    D(const D& c) {
        usedConstr += "CC";
    }
    D(D&& c) {
        usedConstr += "MC";
    }
    D& operator=(const D& c) {
        usedConstr += "CA";
        return *this;
    }
    D& operator=(D&& c) {
        usedConstr += "MA";
        return *this;
    }
};

std::string D::usedConstr{};

#pragma endregion help jox



/*	*it, ++it, it++, (it+i), it[i], == och !=	*/
void TestIterForward() {
    Vector<char> vecFooBar("FooBar");
    for (auto i = vecFooBar.begin(); i != vecFooBar.end(); i++)
        cout << *i;
    cout << " står det FooBar?" << endl;
    //    vecBar = "raboof";
    auto it = vecFooBar.begin();
    assert(*it == 'F');
    assert(*(it++) == 'F' && *it == 'o');
    *(++it) = 'y';
    assert(*++it == 'B');
    assert(*(it + 1) == 'a');
    assert(it[2] == 'r');
}

//void TestIterPartR() {
//    Vector<char> s1("foobar");
//    for (auto i = s1.rbegin(); i != s1.rend(); i++)
//        cout << *i;
//    cout << endl;
//    s1 = "raboof";
//    auto it = s1.rbegin();
//    assert(*it == 'f');
//
//    assert(*(it++) == 'f' && *it == 'o');
//    ++it;
//    assert(*++it == 'b');
//    assert(*(it + 1) == 'a');
//    assert(it[2] == 'r');
//}

void TestIterForwardC() {
    Vector<char> s1("foobar");
    for (auto i = s1.cbegin(); i != s1.cend(); i++)
        cout << *i;
    cout << endl;
    //    s1 = "raboof";
    auto it = s1.cbegin();
    assert(*it == 'f');
    assert(*(it++) == 'f' && *it == 'o');
    ++it;
    assert(*++it == 'b');
    assert(*(it + 1) == 'a');
    assert(it[2] == 'r');
}

//void TestIterPartCR() {
//    Vector<char> s1("foobar");
//    for (auto i = s1.crbegin(); i != s1.crend(); ++i)
//        cout << *i;
//    cout << endl;
//    s1 = "raboof";
//    auto it = s1.crbegin();
//    assert(*it == 'f');
//    assert(*(it++) == 'f' && *it == 'o');
//    ++it;
//    assert(*++it == 'b');
//    assert(*(it + 1) == 'a');
//    assert(it[2] == 'r');
//}

void TestIterPart1() {

    //-	typdefs för iterator, const_iterator,  reverse_iterator och const_revers_iterator
    Vector<char>::iterator Str;
    //Vector<char>::reverse_iterator rStr;

    //-	funktionerna begin, end, cbegin, cend
    {
        Vector<char> v;
        const Vector<char> cv;
        assert(typeid(Vector<char>::iterator) == typeid(v.begin()));
        assert(typeid(Vector<char>::const_iterator) == typeid(cv.begin()));
        assert(typeid(Vector<char>::const_iterator) == typeid(v.cbegin()));

        assert(typeid(Vector<char>::iterator) == typeid(v.end()));
        assert(typeid(Vector<char>::const_iterator) == typeid(cv.end()));
        assert(typeid(Vector<char>::const_iterator) == typeid(v.cend()));
    }


    //rbegin, rend, crbegin och crend.


    TestIterForward();
    //TestIterPartR();
    Vector<char>::const_iterator cStr;
    //Vector<char>::const_reverse_iterator crStr;
    TestIterForwardC();
    //TestIterPartCR();

    //Iteratorerna ska kunna göra:
    //-	*it, ++it, it++, (it+i), it[i], == och !=



    //-	default constructor, copy constructor och tilldelning (=) 
    Vector<char> s("foobar");
    Str = s.begin();
    //rStr = s.rbegin();
    //cStr = s.cbegin();
    //crStr = s.crbegin();

    *Str = 'a';
    ////	*(cStr+1)='b';	//Sak ge kompileringsfel!
    //*(rStr + 2) = 'c';
    ////	*(crStr+3)='d';	//Sak ge kompileringsfel!
    assert(s == Vector<char>("aoobar"));

    cout << "\nTestFörGodkänt Iter klar\n";
#ifdef VG
    cout << "\nTestFörVälGodkänt Iter klar\n";
#endif VG

}

#pragma region Test Iterator in Algorithm
namespace std {
    template <class IT>
    void random_shuffle(IT begin, IT end) {
        shuffle(begin, end, std::default_random_engine());
    }
}

void TestIterInAlg() {
    static const int N = 26;
    Vector<char> v;
    v.reserve(N);
    for (int i = 0; i < N; ++i) {
        v.push_back('a' + i);
    }
    v.begin();
    auto b = std::begin(v);
    auto e = std::end(v);

    std::random_shuffle(b, e);

    cout << v << endl;
    std::stable_sort(b, e);
    std::sort(b, e);

    cout << v << endl;
}

//void TestRevIterInAlg() {
//    static const int N = 26;
//    Vector<char> v;
//    v.reserve(N);
//    for (int i = 0; i < N; ++i) {
//        v.push_back('a' + i);
//    }
//    auto b = std::rbegin(v);
//    auto e = std::rend(v);
//
//    std::random_shuffle(b, e);
//
//    cout << v << endl;
//    std::stable_sort(b, e);
//
//    cout << v << endl;
//}
#pragma endregion Test Iterator in Algorithm

#pragma region Test of typedefs!
using std::is_same;

template<class Iter>
struct CheckTypedefs {
    static_assert(is_same<int, typename Iter::value_type>::value, "value_type is missing");
    static_assert(is_same<ptrdiff_t, typename Iter::difference_type>::value, "typdef missing");
    static_assert(is_same<int&, typename Iter::reference>::value, "typdef missing, int&");
    static_assert(is_same<int*, typename Iter::pointer>::value, "typdef missing, int*");
    static_assert(is_same<std::random_access_iterator_tag, typename Iter::iterator_category>::value, "should be random_access_iterator_tag");
};

template<class Iter>
struct CheckTypedefsConst {
    static_assert(is_same<int, typename Iter::value_type>::value ||
        is_same<const int, typename Iter::value_type>::value, "value_type should be without const");
    static_assert(is_same<ptrdiff_t, typename Iter::difference_type>::value, "typdef missing");
    static_assert(is_same<const int&, typename Iter::reference>::value, "should be with const");
    static_assert(is_same<const int*, typename Iter::pointer>::value, "should be with const");
    static_assert(is_same<std::random_access_iterator_tag, typename Iter::iterator_category>::value, "should be random_access_iterator_tag");
};

template struct CheckTypedefs<std::vector<int>::iterator>;
template struct CheckTypedefsConst<std::vector<int>::const_iterator>;

template struct CheckTypedefs<Vector<int>::iterator>;
template struct CheckTypedefsConst<Vector<int>::const_iterator>;
//template struct CheckTypedefs<Vector<int>::reverse_iterator>;
//template struct CheckTypedefsConst<Vector<int>::const_reverse_iterator>;

#pragma endregion Test of typedefs!

#else
void TestIterPart1() {}
void TestIterInAlg() {}
#endif
