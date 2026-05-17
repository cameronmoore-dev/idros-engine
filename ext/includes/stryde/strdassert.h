#pragma once

#include <iostream>

#ifndef NDEBUG
#define STRD_ASSERT(condition, message)\
    if (!(condition))\
    {\
        std::cerr << "ASSERT FAILED: " << message << std::endl;\
        std::cerr << "File: " << __FILE__ << std::endl;\
        std::cerr << "Line: " << __LINE__ << std::endl;\
        std::abort();\
    }
#else
#define STRD_ASSERT(condition, message)
#endif