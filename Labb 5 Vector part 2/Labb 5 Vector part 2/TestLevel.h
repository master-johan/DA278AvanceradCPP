#pragma once
//Använd denna som allocator
#include "Dalloc.hpp"

#define LEVEL 99
#define G_BETYG
#define VG_BETYG

//Tanken är att man höjer Level successivt till sista nivån
//Sen kan man sätta level till 99
//    och definiera G_BETYG så kör den riktiga testen igång.
//Observera att LEVEL testerna är mycket enkla
//det är först G_BETYG som blir en ordentlig test.

/* Så här är nivåerna man kan sätta på LEVEL
#if false
level 1
Vector();
size()
capacity()
Invariant();

level 2
Vector(char*);
operator<<
operator==

level 3
operator<, >= ,,, allihop

level 4
    Vector(const Vector&);
    operator=

Level 5
operator[]
at

level 6
push_back

level 7
Move Constructor
Move Assignment

Level 8:
reserve
shrink_to_fit
resize
swap

Level 9:
data()
typedefen testas inte!

level 10
VectorIter(const Link* node = nullptr)
begin()
end()
operator==
operator!=

Level 11
VectorIter(const VectorIter&)
VectorIter& operator=(const VectorIter&)

operator*()
operator->()

level 12
operator++
operator++(int)

operator--
operator--(int)

level 13
operator+=
operator+
operator-(it)
operator-(int)
#endif

level 20
emplace_back
#endif

*/