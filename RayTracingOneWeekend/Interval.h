#pragma once

class Interval {
public:
	Interval();
	Interval(float min, float max);

	bool Contains(float x) const;
	bool Surrounds(float x) const;
	float Clamp(float x) const;

	float UpperBound() const { return m_Max; }
	float LowerBound() const { return m_Min; }

	static const Interval Empty, Universe;

private:
	float m_Min, m_Max;
};
