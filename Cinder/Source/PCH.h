#ifdef _WIN32
#	ifdef _WIN64
#		define PLATFORM_WINDOWS
#	else
#		error 32 bit is not supported!
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error iOS simulator not supported!
#	elif TARGET_OS_IPHONE == 1
#		error iOS is not supported!
#	elif TARGET_OS_MAC == 1
#		define PLATFORM_MAC
#	else
#		error Unknown Apple platform!
#	endif
#elif defined(__ANDROID__)
#	error Android is not supported!
#elif defined(__linux__)
#	define PLATFORM_LINUX
#else
#	error Unknown platform!
#endif

// Portable SIMD intrinsics
#include <immintrin.h>

#include <algorithm>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "yaml-cpp/yaml.h"

#include "Core/Global/Assert.h"
#include "Core/Global/Options.h"
#include "Core/Global/OSAL.h"
#include "Core/Global/Output.h"

#include "Core/Math/Math.h"
#include "Core/Memory/Memory.h"

template<typename T>
using up = std::unique_ptr<T>;
template<typename T>
using sp = std::shared_ptr<T>;
