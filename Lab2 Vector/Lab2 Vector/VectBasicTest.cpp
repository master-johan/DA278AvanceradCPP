//Ett simpelt första testprogram för Vect del 1
//Bygger på att man steg för steg testar några funktioner i taget.
//De funktioner som inte är testas behöver inte alls finnas.
//Styrs av en "Macrovariabel" LEVEL som går från 1 .. ???
//Börja med 1 och gå sen uppåt
//Man behöver kunna se att listan har rätt innehåll
//Därför så har vi två testfunktioner

#include <cassert>
#include <iostream>
using std::cout;

#include "TestLevel.h"

#include "Vector.hpp"

struct C { int i; };

#define Vect Vector<char>
#define FOX Vector<char> Fox("Fox");
#define FOX0 Vector<char> Fox;
#define BAR Vector<char> Bar("Bar");
#define BAR0 Vector<char> Bar;

void TestBasic() {
#if LEVEL>=1
    {
        Vector<char> l;
        assert(l.size() == 0);
        assert(l.capacity() >= 0);
        assert(l.Invariant());
    }
#endif
#if LEVEL>=2
    {
        Vector<char> foo("foo");
        assert(foo.size() == 3);
        cout << foo;
        assert(foo == foo);
        Vector<char> bar("bar");
        assert(!(foo == bar));
    }
#endif
#if LEVEL>=3
    {
        Vector<char> foo("foo"), bar("bar");
        assert(foo == foo && !(foo != foo));
        assert(bar < foo && !(bar > foo));
        assert(bar <= foo && !(bar >= foo));
        assert(bar <= bar && foo >= foo);
        Vector<char> fooa("fooa");
        assert(foo<fooa && fooa>foo);
    }
#endif
#if LEVEL>=4
    {
        Vector<char> foo("foo"), bar("bar");
        Vector<char> foo2(foo);
        assert(foo2.Invariant() && foo2.size() == 3);
        assert(foo == foo2);
        foo2 = bar;
        assert(foo2 == bar);
        assert(foo2.size() == 3);
        assert(foo2.capacity() >= 3);
    }
#endif
#if LEVEL>=5
    {
        Vector<char> foo("fox"), bar("");
        assert(foo[0] == 'f' && foo[1] == 'o');
        foo[0] = 'g';
        assert(foo[0] == 'g' && foo[1] == 'o');
        const Vector<char> bar2("bar");
        static_assert(std::is_const_v<std::remove_reference_t< decltype(bar2[0])>>, "fel med const i operator[]");

        foo.at(1);
        bar2.at(2);

    }
#endif
#if LEVEL>=6
    {
        Vector<char> foo("foo"), bar("");
        foo.push_back('x');
        assert(foo == "foox");
        bar.push_back('y');
        assert(bar == "y");
        //not for vector
        //foo.pop_back();
        //assert(foo == "foo");
        //bar.pop_back();
        //assert(bar == "");
    }
#endif
#if LEVEL>=7
    {
        FOX BAR;
        Vect Fox2(std::move(Fox));
        assert(Fox2.Invariant() && Fox.Invariant());
        assert(Fox2 == "Fox" && Fox == "");
        Bar = std::move(Fox2);
        assert(Fox2.Invariant() && Bar.Invariant());
        assert(Bar == "Fox" && Fox2 == "");
    }
#endif
#if LEVEL>=8
    {
        FOX BAR;
        Fox.reserve(100);
        assert(Fox.capacity() == 100);
        Fox.shrink_to_fit();
        assert(Fox.capacity() == 3);
        Fox.resize(10);
        assert(Fox.capacity() >= 10);
        assert(Fox.size() == 10);
        assert(Fox[9] == 0);
        Fox = "Fox";
        Bar = "Bar";
        swap(Fox, Bar);
        assert(Fox == "Bar" && Bar == "Fox");
    }
#endif
#if LEVEL>=9
    {
        FOX BAR;
        Fox.data();
    }
#endif

#if LEVEL>=10
    {
        Vector<char> foo("foo"), bar("");
        Vector<char>::iterator it1 = foo.begin();
        Vector<char>::iterator it2 = bar.begin();
        assert(it1 == it1 && it1 != it2);
        Vector<char>::iterator ite1 = foo.end();
        Vector<char>::iterator ite2 = bar.end();
        assert(it1 != ite1 && it2 == ite2);

    }
#endif
#if LEVEL>=11
    {
        Vector<char> foo("foo"), bar("");
        Vector<char>::iterator it1 = foo.begin();
        Vector<char>::iterator it2 = bar.begin();
        Vector<char>::iterator it1x(it1);
        assert(it1 == it1x);

        assert(*it1 == 'f');
        *it1 = 'x';
        assert(*it1 == 'x');

        Vector<C> clist;
        clist.push_back(C{ 1 });
        assert(clist.begin()->i == 1);
    }
#endif
#if LEVEL>=12
    {
        Vector<char> foo("foo"), bar("");
        Vector<char>::iterator it1 = foo.begin();
        Vector<char>::iterator it2 = bar.begin();
        assert(*++it1 == *it1);
        assert(*it1 == *it1--);
        assert(*it1 == *foo.begin());
        assert(*--it1 == *it1);
    }
#endif
}