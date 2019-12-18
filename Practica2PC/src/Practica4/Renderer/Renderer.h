#ifdef __ORBIS__
#include <Renderer/PS4/PS4Renderer.h>
using Renderer = PS4Renderer;
#elif _WIN64 || _WIN32 
#include <Renderer/PC/PCRenderer.h>
using Renderer = PCRenderer;
#else
#error Platform not supported
#endif