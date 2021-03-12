//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG
//#include <crtdbg.h>

#include "TestLevel.h"

#include "Vector.hpp"

#include <iostream>
using std::cout;

void TestPushBack();

void TestBasic();
void TestVector();
void TestIter();
//void TestIterInAlg();
//void TestRevIterInAlg();

int main() {
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    std::locale::global(std::locale("swedish"));

#ifdef WRITEDESC
    WriteDesc<int>();
#endif

    std::cout <<
        "Ändra i TestLevel.h för att kontrollera vad som testas\n"
        "Gå stegvis upp med LEVEL från 1 till ungefär 25, ändra den sen till 99\n"
        "Och kontrollera vilket betyg som testas genom att definiera VG_BETYG eller G_BETYG\n"
        "Change the includes of Vector.hpp and VectIter.hpp to your files\n\n"
        ;
    std::cout << "Selected is " <<
#ifdef VG_BETYG
        "VG"
#else
#ifdef G_BETYG
        "G"
#else
        "????"
#endif
#endif
        << "\n\n";

    TestBasic();
    TestVector();
    TestIter();
    //TestIterInAlg();
    //TestRevIterInAlg();

//    Dalloc<double>().allocate(1); cout << "det finns en minnesläcka i main, avsiktligt!\n så ni kan se att er minnesläckstest fungerar\n";
    //std::cin.get();
}
