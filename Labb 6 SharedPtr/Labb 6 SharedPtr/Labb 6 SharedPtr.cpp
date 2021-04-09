#include "Config.h"

#include <iostream>
using std::cout;


void TestG();
void TestVGWeak();
void TestVG();

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    std::locale::global(std::locale("swedish"));

    TestG();
    cout << "Test for G finished\n";

#ifdef  VGBETYG
    TestVG();
    TestVGWeak();

    cout << "Test for VG finished\n";
#endif //  VGBETYG
    new int; cout << "you should get a memory leak (new int)\n";
    //    std::cin.get();
}


