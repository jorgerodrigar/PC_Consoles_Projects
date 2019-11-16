#ifdef __ORBIS__
#include <Renderer/PS4/PS4Renderer.h>
using Renderer = PS4Renderer;
#elif _WIN64 || _WIN32 
#include <Renderer/PC/PCRenderer.h>
using Renderer = PCRenderer;
#else
#error Platform not supported
#endif

// macro de conversion de rgb a color en hexadecimal
#define RGB(r, g, b) ((r & 0xff) << 16) + ((g & 0xff) << 8) + ((b & 0xff))
#define RGBA(r, g, b, a) ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + ((a & 0xff))


