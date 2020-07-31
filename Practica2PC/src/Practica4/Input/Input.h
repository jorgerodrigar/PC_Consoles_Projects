#ifdef __ORBIS__
#include <Input/PS4/PS4Input.h>
using Input = PS4Input;
#elif _WIN64 || _WIN32 
#include <Input/PC/PCInput.h>
using Input = PCInput;
#else
#error Platform not supported
#endif


