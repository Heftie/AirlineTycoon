#include "stdafx.h"

#include <stdio.h>
#include <stdexcept>

const char* ExcAssert           = "Assert (called from %s:%li) failed!";
const char* ExcGuardian         = "Con/Des guardian %lx failed!";
const char* ExcStrangeMem       = "Strange new: %li bytes!";
const char* ExcOutOfMem         = "Couldn't allocate %li bytes!";
const char* ExcNotImplemented   = "Function not implemented!";
const char* ExcImpossible       = "Impossible Event %s occured";

HDU Hdu;

HDU::HDU()
    : Log(NULL)
{
    char* base = SDL_GetBasePath();
    const char* file = "debug.log";
    BUFFER<char> path(strlen(base) + strlen(file) + 1);
    strcpy(path, base);
    strcat(path, file);
    Log = fopen(path, "w");
}

HDU::~HDU()
{
    Close();
}

void HDU::Close()
{
    if (Log)
        fclose (Log);
    Log = NULL;
}

void HDU::HercPrintf(int, char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(Log, format, args);
    va_end(args);
    fprintf(Log, "\n");
    fflush(Log);
}

void HDU::HercPrintf(char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(Log, format, args);
    va_end(args);
    fprintf(Log, "\n");
    fflush(Log);
}

void here(char *file, int line)
{
    Hdu.HercPrintf(0, "Here in %s, line %li", file, line);
}

int TeakLibW_Exception(char* file, int line, const char* format, ...)
{
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    //MessageBeep(0);
    //MessageBeep(0x30u);
    Hdu.HercPrintf(1, "====================================================================");
    Hdu.HercPrintf(0, "Exception in File %s, Line %li:", file, line);
    Hdu.HercPrintf(0, buffer);
    Hdu.HercPrintf(1, "====================================================================");
    Hdu.HercPrintf(0, "C++ Exception thrown. Programm will probably be terminated.");
    Hdu.Close();
    //DebugBreak();
    throw std::runtime_error(buffer);
}
