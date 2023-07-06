
#ifndef SIGSCAN_H
#define SIGSCAN_H

#include <stdint.h>

static uintptr_t SigScanMemory(void *start, size_t maxScanBytes, char *pattern, char *ignorePattern);
static uintptr_t SigScan(char *binPath, char *pattern, char *ignorePattern, char *error, size_t errorMaxlen);

#ifdef SIGSCAN_IMPLEMENTATION
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#error Only windows supported.
#endif

static uintptr_t SigScanMemory(void *start, size_t maxScanBytes, char *pattern, char *ignorePattern)
{
	uintptr_t patternLen = strlen(ignorePattern);
	
	char *memory = (char *)start;
	for (uintptr_t i = 0; i < maxScanBytes; i++)
	{
		uintptr_t matches = 0;
		while (memory[i + matches] == pattern[matches] || ignorePattern[matches] != 'x')
		{
			matches++;
			if (matches == 14)
			{
				matches = matches;
			}
			if (matches == patternLen)
			{
				return (uintptr_t)(memory + i);
			}
		}
	}
	return NULL;
}

static uintptr_t SigScan(char *binPath, char *pattern, char *ignorePattern, char *error, size_t errorMaxlen)
{
#ifdef _WIN32
	HMODULE bin = LoadLibrary(binPath);
	
	MODULEINFO info = {0};
	GetModuleInformation(GetCurrentProcess(), bin, &info, sizeof(info));
	
	size_t binBytes = info.SizeOfImage;
#elif __linux__
	// untested
	auto *bin = dlopen(binPath, RTLD_NOW);
	
	s64 binBytes = MEGABYTES(50);
#endif
	uintptr_t result = NULL;
	if (bin)
	{
		result = SigScanMemory(bin, binBytes, pattern, ignorePattern);
	}
	else
	{
		snprintf(error, errorMaxlen, "Could not open %s.", binPath);
	}
	
	return result;
}

#endif // SIGSCAN_IMPLEMENTATION
#endif // SIGSCAN_H