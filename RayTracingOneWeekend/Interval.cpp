#include "Interval.h"

#include "Common.h"

Interval::Interval()
	: m_Min(+infinity), m_Max(-infinity) {}

Interval::Interval(float min, float max)
	: m_Min(min), m_Max(max) {}

bool Interval::Contains(float x) const {
	return m_Min <= x && x <= m_Max;
}

bool Interval::Surrounds(float x) const {
	return m_Min < x && x < m_Max;
}

float Interval::Clamp(float x) const {
	return std::clamp(x, m_Min, m_Max);
}

const static Interval Empty(+infinity, -infinity);
const static Interval Universe(-infinity, +infinity);
