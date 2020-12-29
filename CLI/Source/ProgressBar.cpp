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

#include "ProgressBar.h"

#include "fmt/color.h"
#include "fmt/core.h"

ProgressBar::ProgressBar(float min, float max, float step)
	: m_Min(min), m_Max(max), m_Step(step)
{
	Update(min);
}

void ProgressBar::Update(float value)
{
	m_Buffer = "{:.1f}% ";

	float nvalue = value;
	while (nvalue > m_Min)
	{
		m_Buffer += "█";
		nvalue -= m_Step;
	}
	m_Buffer += '\r';

	fmt::print(fg(fmt::color::light_green), m_Buffer, value / (m_Max - m_Min) * 100.f);
}

void ProgressBar::End()
{
	m_Buffer = "{:.1f}% ";

	while (m_Max > m_Min)
	{
		m_Buffer += "█";
		m_Max -= m_Step;
	}
	m_Buffer += "\r\n";

	fmt::print(fg(fmt::color::light_green), m_Buffer, 100.f);
}
