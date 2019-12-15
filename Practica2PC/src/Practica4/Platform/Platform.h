#ifdef __ORBIS__
#include <Platform/PS4/PS4Platform.h>
#define HEADER_PATH "/app0/"
using Platform = PS4Platform;
#elif _WIN64 || _WIN32 
#include <Platform/PC/PCPlatform.h>
using Platform = PCPlatform;
#else
#error Platform not supported
#endif