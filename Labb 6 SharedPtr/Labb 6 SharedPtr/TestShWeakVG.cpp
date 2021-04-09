#include "Config.h"
#ifdef VGBETYG
#include <memory>

#include "SharedPtr.hpp"
#include "WeakPtr.hpp"

namespace {
    struct C {
        float value;
        C(float value) :value(value) {};
        void CheckC(float v) {
            assert(v == value);
        }
    };
}

void TestVGWeak() {
    //Weak pointer skall ha det som det står VG på nedan
    //-	Konstruktor som tar:		
    //	o	inget	G	VG
    //	o	En SharedPtr	G	VG
    //	o	En WeakPtr	VG	VG

    WeakPtr<C> wp11;
    assert(wp11.expired());
    SharedPtr<C> sp12(new C(12));
    WeakPtr<C> wp13(wp11);
    assert(wp13.expired());
    WeakPtr<C> wp14(sp12);
    assert(!wp14.expired());

    SharedPtr<C> sp17(wp14);
    assert(sp17);

    {
        auto w = WeakPtr<int>(SharedPtr<int>());
    }

    //-	Destruktor	G	VG
    //	It will test itself
    //-	Tilldelning från en		
    //	o	En SharedPtr	G	VG
    //	o	En WeakPtr			VG
    WeakPtr<C> wp15;
    wp14 = wp11;
    assert(wp14.expired());

    SharedPtr<C> sp33(new C(33));
    wp14 = sp33;
    assert(!wp14.expired());
    wp14 = wp14;
    assert(!wp14.expired());

    sp33.reset();
    assert(!sp33);
    assert(wp14.expired());
    {
        auto w = WeakPtr<int>(SharedPtr<int>());
        w = SharedPtr<int>();
    }

    //Shared(weak)
    try {
        SharedPtr<C> slask(wp14);
        assert(false);
    }
    catch (std::bad_weak_ptr&) {
        ;
    }

    //-	funktioner:		
    //	o	lock()		VG
    auto sp51 = wp11.lock();
    assert(!sp51);

    SharedPtr<C>  sp55(new C(55));
    wp14 = sp55;
    sp51 = wp14.lock();
    assert(sp51);
    ////	o	expired()		VG	Redan testat

    //move
    SharedPtr<C> sp61(std::move(sp51));
    assert(sp61->value == 55);
    assert(!sp51);

    sp51 = std::move(sp61);
    sp51 = std::move(sp51);
    assert(sp51->value == 55);



    //-	Tilldelning från en	
    //	o	En SharedPtr<D>	G
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
    }



}

void TestVG() {
    //Weak pointer skall ha det som det står VG på nedan
    //-	Konstruktor som tar:		
    //	o	inget	G	VG
    //	o	En SharedPtr	G	VG
    //	o	En WeakPtr	VG	VG

    WeakPtr<C> wp11;
    assert(wp11.expired());
    SharedPtr<C> sp12(new C(12));
    WeakPtr<C> wp13(wp11);
    assert(wp13.expired());
    WeakPtr<C> wp14(sp12);
    assert(!wp14.expired());

    SharedPtr<C> sp17(wp14);
    assert(sp17);

    //-	Destruktor	G	VG
    //	It will test itself
    //-	Tilldelning från en		
    //	o	En SharedPtr	G	VG
    //	o	En WeakPtr			VG
    WeakPtr<C> wp15;
    wp14 = wp11;
    assert(wp14.expired());

    SharedPtr<C> sp33(new C(33));
    wp14 = sp33;
    assert(!wp14.expired());
    wp14 = wp14;
    assert(!wp14.expired());

    sp33.reset();
    assert(!sp33);
    assert(wp14.expired());

    //Shared(weak)
    try {
        SharedPtr<C> slask(wp14);
        assert(false);
    }
    catch (const std::bad_weak_ptr&) {
        ;
    }

    //-	funktioner:		
    //	o	lock()		VG
    auto sp51 = wp11.lock();
    assert(!sp51);

    SharedPtr<C>  sp55(new C(55));
    wp14 = sp55;
    sp51 = wp14.lock();
    assert(sp51);
    ////	o	expired()		VG	Redan testat

    //move
    SharedPtr<C> sp61(std::move(sp51));
    assert(sp61->value == 55);
    assert(!sp51);

    sp51 = std::move(sp61);
    sp51 = std::move(sp51);
    assert(sp51->value == 55);



    //-	Tilldelning från en	
    //	o	En SharedPtr<D>	G
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
    }

}

#endif VGBETYG

