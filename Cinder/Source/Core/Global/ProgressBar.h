#pragma once

class ProgressBar
{
public:
	ProgressBar(float min, float max, float step);

	void Update(float value);
	void End();

private:
	float m_Min, m_Max, m_Step;
	std::string m_Buffer;
};
