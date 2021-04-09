//Inkludera först i alla cpp filer 
#pragma once

//Memory leaks
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#define GBETYG  //Kommentera bort denna rad för VG test
#ifdef GBETYG
#undef VGBETYG
#else
#define VGBETYG
#endif
