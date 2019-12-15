#ifdef __ORBIS__
#include <Renderer/PS4/PS4Renderer.h>
using Renderer = PS4Renderer;
#define RGBA(r, g, b, a) ((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + ((b & 0xff))
#elif _WIN64 || _WIN32 
#include <Renderer/PC/PCRenderer.h>
using Renderer = PCRenderer;
#else
#error Platform not supported
#endif