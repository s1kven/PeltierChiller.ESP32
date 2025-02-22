#pragma once
#ifndef _FileInfo_
#define _FileInfo_ 
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Helpers
{
    struct FileInfo 
    {
        String name;
        uint64_t size;
        struct tm creationTime;
    };
}
#endif