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

#include "PCH.h"

Options GOptions;
fmt::memory_buffer GFormatBuffer;

const char* GNewLineStart = "\r\n\0";
const char* GNewLineEnd = GNewLineStart + 3;
std::string GDebug = "Debug: ";
std::string GVerbose = "Verbose: ";
std::string GLog = "Log: ";
std::string GWarning = "Warning: ";
std::string GError = "Error: ";
std::string GFatal = "Fatal: ";
