#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <iostream>
using std::cout;

#include "DLLTest/TestLevel.h"

#include "List.hpp"


//template class List<int>;


void TestBasic();
void TestList();
void TestListIter();

int main() {
#ifdef DBG_NEW
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	TestBasic();
	TestList();
	TestListIter();
	new int(7);
	std::cout << "There should be one memory leak!";
	std::cin.get();

}

//template class List<int>;