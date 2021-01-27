#pragma once

#define LEVEL 2
//#define G
//#define VG

//Tanken är att man höjer Level successivt till 12
//Sen kan man sätta level till 99
//    och definiera G så kör den riktiga testen igång.
//Observera att LEVEL testerna är mycket enkla
//det är först G som blir en ordentlig test.

/* Så här är nivåerna man kan sätta på LEVEL
#if false
level 1
List();
Count()
Invariant();

level 2
List(char*);
Print()
operator==

level 3
operator<, >= ,,, allihop

level 4
    List(const List&);

Level 5
empty()
front(och const)
back(och const)

level 6
push_back
pop_back

level 7
push_front
pop_front

level 10
ListIter(const Link* node = nullptr)
begin()
end()
operator==
operator!=

Level 11
ListIter(const ListIter&)
ListIter& operator=(const ListIter&)

operator*()
operator->()

level 12
operator++
operator++(int)

operator--
operator--(int)

#endif
*/