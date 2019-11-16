#ifdef  __ORBIS__

#include <algorithm>
#include <assert.h>
#include <errno.h>
#include <kernel.h>
#include <mspace.h>
#include <sceerror.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE (15 * 1024 * 1024) // 15 MB

static SceLibcMspace s_mspace;
static off_t s_memStart;
static size_t s_memLength = 4 * 1024 * 1024;
static size_t s_memAlign = 2 * 1024 * 1024;

extern "C" {
int user_malloc_init(void);
int user_malloc_finalize(void);
void *user_malloc(size_t size);
void user_free(void *ptr);
void *user_calloc(size_t nelem, size_t size);
void *user_realloc(void *ptr, size_t size);
void *user_memalign(size_t boundary, size_t size);
int user_posix_memalign(void **ptr, size_t boundary, size_t size);
void *user_reallocalign(void *ptr, size_t size, size_t boundary);
int user_malloc_stats(SceLibcMallocManagedSize *mmsize);
int user_malloc_stats_fast(SceLibcMallocManagedSize *mmsize);
size_t user_malloc_usable_size(void *ptr);
}

//E Replace _malloc_init function.
int user_malloc_init(void)
{
	int res;
	void *addr;
	
	//E Allocate direct memory
	res = sceKernelAllocateDirectMemory(0, SCE_KERNEL_MAIN_DMEM_SIZE, s_memLength, s_memAlign, SCE_KERNEL_WB_ONION, &s_memStart);
	if (res < 0) {
		//E Error handling
		return 1;
	}

	addr = NULL;
	//E Map direct memory to the process address space
	res = sceKernelMapDirectMemory(&addr, HEAP_SIZE, SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE, 0, s_memStart, s_memAlign);
	if (res < 0) {
		//E Error handling
		return 1;
	}

	//E Generate mspace
	s_mspace = sceLibcMspaceCreate("User Malloc", addr, HEAP_SIZE, 0);
	if (s_mspace == NULL) {
		//E Error handling
		return 1;
	}

	return 0;
}

//E Replace _malloc_finalize function.
int user_malloc_finalize(void)
{
	int res;

	if (s_mspace != NULL) {
		//E Free mspace
		sceLibcMspaceDestroy(s_mspace);
	}

	//E Release direct memory
	res = sceKernelReleaseDirectMemory(s_memStart, s_memLength);
	if (res < 0) {
		//E Error handling
		return 1;
	}

	return 0;
}

//E Replace malloc function.
void *user_malloc(size_t size)
{
	return sceLibcMspaceMalloc(s_mspace, size);
}

//E Replace free function.
void user_free(void *ptr)
{
	sceLibcMspaceFree(s_mspace, ptr);
}

//E Replace calloc function.
void *user_calloc(size_t nelem, size_t size)
{
	return sceLibcMspaceCalloc(s_mspace, nelem, size);
}

//E Replace realloc function.
void *user_realloc(void *ptr, size_t size)
{
	return sceLibcMspaceRealloc(s_mspace, ptr, size);
}

//E Replace memalign function
void *user_memalign(size_t boundary, size_t size)
{
	return sceLibcMspaceMemalign(s_mspace, boundary, size);
}

//E Replace posix_memalign function.
int user_posix_memalign(void **ptr, size_t boundary, size_t size)
{
	return sceLibcMspacePosixMemalign(s_mspace, ptr, boundary, size);
}

//E Replace reallocalign function.
void *user_reallocalign(void *ptr, size_t size, size_t boundary)
{
	return sceLibcMspaceReallocalign(s_mspace, ptr, boundary, size);
}

//E Replace malloc_stats function.
int user_malloc_stats(SceLibcMallocManagedSize *mmsize)
{
	return sceLibcMspaceMallocStats(s_mspace, mmsize);
}

//E Replace malloc_stats_fast function.
int user_malloc_stats_fast(SceLibcMallocManagedSize *mmsize)
{
	return sceLibcMspaceMallocStatsFast(s_mspace, mmsize);
}

//E Replace malloc_usable_size function.
size_t user_malloc_usable_size(void *ptr)
{
	return sceLibcMspaceMallocUsableSize(ptr);
}

#endif //  _ORBIS_