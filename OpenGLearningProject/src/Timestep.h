#pragma once

struct Timestep
{
	Timestep(double time) : Time(time) {}
	inline operator double() const { return Time; }
	inline double GetMilliseconds() const { return Time * (double)1000; }

	double Time;
};