//Ett simpelt första testprogram för DLL
//Bygger på att man steg för steg testar några funktioner i taget.
//De funktioner som inte är testas behöver inte alls finnas.
//Styrs av en "Macrovariabel" LEVEL som går från 1 .. ???
//Börja med 1 och gå sen uppåt
//Man behöver kunna se att listan har rätt innehåll
//Därför så har vi två testfunktioner

#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <cassert>
#include <iostream>
using std::cout;

#include "TestLevel.h"

#include "../List.hpp"
#include "../Flist.hpp"

struct C { int i; };

void TestBasic() {
#if LEVEL>=1
    {
        List<char> l;
        assert(l.Count() == 0);
        assert(l.Invariant());
    }
#endif
#if LEVEL>=2
    {
#if 0 // 1 for test on Flist, 0 for test on List
        Flist<char> foo("foo");
        //assert(foo.size() == 3);
        //foo.Print(cout);
        assert(foo == foo);
        Flist<char> bar("bar");
        assert(!(foo == bar));
#else
        List<char> foo("foo");
        assert(foo.Count() == 3);
        foo.Print(cout);
        assert(foo == foo);
        List<char> bar("bar");
        assert(!(foo == bar));
#endif
    }
#endif
#if LEVEL>=3
    {
        List<char> foo("foo"), bar("bar");
        assert(foo == foo && !(foo != foo));
        assert(bar < foo && !(bar > foo));
        assert(bar <= foo && !(bar >= foo));
        assert(bar <= bar && foo >= foo);
        List<char> fooa("fooa");
        assert(foo<fooa&& fooa>foo);
    }
#endif
#if LEVEL>=4
    {
        List<char> foo("foo"), bar("bar");
        List<char> foo2(foo);
        assert(foo2.Invariant() && foo2.Count() == 3);
        assert(foo == foo2);
    }
#endif
#if LEVEL>=5
    {
        List<char> foo("foo"), bar("");
        assert(!foo.empty() && bar.empty());

        const        List<char> bar2("bar");
        assert(bar2.front() == 'b' && bar2.back() == 'r');
    }
#endif
#if LEVEL>=6
    {
        List<char> foo("foo"), bar("");
        foo.push_front('x');
        assert(foo == "xfoo");
        bar.push_front('y');
        assert(bar == "y");
        foo.pop_front();
        assert(foo == "foo");
        bar.pop_front();
        assert(bar == "");
    }
#endif
#if LEVEL>=7
    {
        List<char> foo("foo"), bar("");
        foo.push_back('x');
        assert(foo == "foox");
        bar.push_back('y');
        assert(bar == "y");
        foo.pop_back();
        assert(foo == "foo");
        bar.pop_back();
        assert(bar == "");
    }
#endif

#if LEVEL>=10
    {
        List<char> foo("foo"), bar("");
        List<char>::iterator it1 = foo.begin();
        List<char>::iterator it2 = bar.begin();
        assert(it1 == it1 && it1 != it2);
        List<char>::iterator ite1 = foo.end();
        List<char>::iterator ite2 = bar.end();
        assert(it1 != ite1 && it2 == ite2);

    }
#endif
#if LEVEL>=11
    {
        List<char> foo("foo"), bar("");
        List<char>::iterator it1 = foo.begin();
        List<char>::iterator it2 = bar.begin();
        List<char>::iterator it1x(it1);
        assert(it1 == it1x);

        assert(*it1 == 'f');
        *it1 = 'x';
        assert(*it1 == 'x');

        List<C> clist;
        clist.push_front(C{ 1 });
        assert(clist.begin()->i == 1);
    }
#endif
#if LEVEL>=12
    {
        List<char> foo("foo"), bar("");
        List<char>::iterator it1 = foo.begin();
        List<char>::iterator it2 = bar.begin();
        assert(*++it1 == *it1);
        assert(*it1 == *it1--);
        assert(*it1 == *foo.begin());
        assert(*--it1 == *it1);
    }
#endif
}