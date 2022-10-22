#pragma once



#if __cpp_lib_unreachable
#define UNREACHABLE std::unreachable()
#elif __clang__
#define UNREACHABLE __builtin_unreachable()
#endif