#include "GameSequence.h"
// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// ƒGƒ‰[‘S•”–³Ž‹‚·‚é‚â‚Â
//#pragma warning(disable : 4996)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameSequence::GetInstance()->MainGameLoop();

	_CrtDumpMemoryLeaks();
	return 0;
}
