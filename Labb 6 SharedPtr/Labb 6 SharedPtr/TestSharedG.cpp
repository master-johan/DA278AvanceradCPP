#include "Config.h"

#include <cassert>
#include <memory>
#include <iostream>
using std::cout;
using std::cin;

#include "SharedPtr.hpp"

struct C {
    float value;
    C(float value) :value(value) {};
    void CheckC(float v) {
        assert(v == value);
    }
};

//Tries to test assignemnt of A from B
//Only use constructors, == ....
//a and b shood be inititated to different values.
template<class A, class B>
void TestAssignment(A ax, B by) {
    assert(!(ax == by));
    //an = null valued
    //ax = a value(x)
    //a is used below
    // same for b
    A an, a;
    B bn, b;
    assert(a.Invariant());
    assert(an.Invariant());
    assert(ax.Invariant());
    assert(b.Invariant());
    assert(bn.Invariant());
    assert(by.Invariant());
    assert(an == bn);
    an = bn;    //n=b
    assert(an == bn);
    a = an;
    assert(a == an);
    a = by;    //n=y
    assert(a == by);
    a = by;    //y=y
    assert(a == by);
    a = bn;    //y=n
    assert(a == bn);
    a = ax;
    assert(a == ax);
    a = by;    //x=y
    assert(a == by);
    a = ax;
    assert(a == ax);
    a = a;    //self assignment;
    assert(a == ax);
}

// Tries to test move assignemnt of A from B
//Only use constructors, == ....
//a and b shood be inititated to different values.
template<class A, class B>
void TestMoveAssignment(A ax, B by) {
    assert(!(ax == by));
    //an = null valued
    //ax = a value(x)
    //a is used below
    // same for b
    A an, a;
    B bn, b;
    assert(a.Invariant());
    assert(an.Invariant());
    assert(ax.Invariant());
    assert(b.Invariant());
    assert(bn.Invariant());
    assert(by.Invariant());
    assert(an == bn);
    an = std::move(bn);
    assert(an == bn);   //Båda 
    a = ax;  //a är något
    assert(a == ax);
    an = std::move(a);
    assert(an == ax);
    assert(a.Invariant());
    assert(an.Invariant());
    assert(ax.Invariant());
    an = std::move(an); //self move
    assert(an.Invariant());
}



void TestG() {
    //-	Konstruktor som tar:	
    //	o	inget	G
    //	o	En SharedPtr	G
    //	o	En pekare	G

    SharedPtr<C> sp11;
    assert(!sp11);
    SharedPtr<C> p15(nullptr);
    assert(!p15);
    SharedPtr<C> sp12(new C(12));
    assert(sp12);
    SharedPtr<C> sp13(sp11);
    assert(!sp13);

    assert(sp12.use_count() == 1);
    SharedPtr<C> sp14(sp12);
    assert(sp14);
    assert(sp12.use_count() == 2);


    //-	Destruktor	G
    //It will test itself

    //-	Tilldelning från en	
    //	o	En SharedPtr	G
    {
        SharedPtr<C> ax(new C(1)), by(new C(2));
        TestAssignment(ax, by);
        TestMoveAssignment(ax, by);
    }
    {
        SharedPtr<C> sp1(new C(1)), sp2;
        sp1->CheckC(1); assert(sp1.Invariant()); assert(sp2.Invariant());
        // null = nått
        sp2 = sp1;
        sp1->CheckC(1); assert(sp1.Invariant()); sp2->CheckC(1); assert(sp2.Invariant());
        //self assignment
        sp1 = sp1;
        sp1->CheckC(1); assert(sp1.Invariant()); sp2->CheckC(1); assert(sp2.Invariant());
        //same pointer;
        sp1 = sp2;
        sp1->CheckC(1); assert(sp1.Invariant()); sp2->CheckC(1); assert(sp2.Invariant());
        SharedPtr<C> sp10(new C(10));
        //nått = nått annat
        sp1 = sp10;
        sp1->CheckC(10); assert(sp1.Invariant()); sp10->CheckC(10); assert(sp10.Invariant());
        sp1 = nullptr;
        //nått = null
        sp10 = sp1;
        assert(sp1.Invariant()); assert(sp10.Invariant());
        assert(!sp10 && !sp1);
        sp14 = sp12;
        assert(sp14);
    }
    //-	Jämförelse med (== och <)
    SharedPtr<C> sp31(new C(31));
    //	o	En SharedPtr	G
    {
        SharedPtr<C> sp12(new C(12));
        assert(sp12);
        SharedPtr<C> sp11, sp13(sp12), sp14(new C(14));
        assert(!sp11);
        assert(sp13);

        assert(sp11 == nullptr);
        assert(sp11 < sp12);
        assert(!(sp12 < sp11));
        assert(sp13 == sp12);
        assert(!(sp13 == sp14));
        assert((sp13 < sp14) || (sp14 < sp13));
    }
    //get, * och ->
    {
        SharedPtr<C> sp41(new C(41));
        SharedPtr<C> sp42(new C(42));
        assert((sp41->value) == (sp41.get()->value));
        assert((sp41->value) != (sp42.get()->value));
        assert(&(*sp41) == (sp41.get()));

        //move
        SharedPtr<C> sp51(std::move(sp41));
        assert(sp51->value == 41);
        assert(!sp41);

        sp51.reset();
        assert(!sp51);
        
        //swap
        {
            SharedPtr<C> sp1(new C(1.0f)), sp2(new C(2.0f));
            swap(sp1, sp2);
            assert(sp1->value == 2.0f && sp2->value == 1.0f);
        }
        //Make shared
        {
            auto sp1 = MakeShared<C>(1.0f);
            assert(sp1->value == 1);
        }}
}




