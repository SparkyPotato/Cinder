#include "PCH.h"
#include "Core/Global/ProgressBar.h"

ProgressBar::ProgressBar(float min, float max, float step)
	: m_Min(min), m_Max(max), m_Step(step)
{
	Update(min);
}

void ProgressBar::Update(float value)
{
	if (GQuiet) { return; }

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
