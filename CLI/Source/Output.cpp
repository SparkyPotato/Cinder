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

#include "Output.h"

#include "fmt/color.h"
#include "fmt/core.h"

ProgressBar GBar(0.f, 100.f, 1.f);

void OutVerbose(const char* message)
{
	fmt::print(fg(fmt::color::light_gray), message);
}

void OutLog(const char* message)
{
	fmt::print(fg(fmt::color::light_green), message);
}

void OutWarning(const char* message)
{
	fmt::print(fg(fmt::color::yellow), message);
}

void OutError(const char* message)
{
	fmt::print(fg(fmt::color::red), message);
}

void OutFatal(const char* message)
{
	fmt::print(fg(fmt::color::dark_red), message);
}

void Progress(float progress)
{
	GBar.Update(progress * 100.f);
}
