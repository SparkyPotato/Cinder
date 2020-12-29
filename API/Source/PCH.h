//    Copyright 2021 SparkyPotato
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//    
//    http://www.apache.org/licenses/LICENSE-2.0
//    
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#ifdef _WIN32
#	ifdef _WIN64
#		define PLATFORM_WINDOWS
#		ifdef BUILD_CINDER
#			define CINDER_API __declspec(dllexport)
#		else
#			define CINDER_API __declspec(dllimport)
#		endif
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
#include "Core/Global/OSAL.h"
#include "Core/Global/Output.h"

#include "Core/Math/Math.h"
#include "Core/Memory/Memory.h"

template<typename T>
using up = std::unique_ptr<T>;
template<typename T>
using sp = std::shared_ptr<T>;
