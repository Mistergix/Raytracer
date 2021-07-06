#pragma once
#include "Color.h"
#include "Vector3.h"

struct Background
{
	const Color sky{ 0.21763764f * 255.0f, 0.45626346f * 255.0f, 1.0f * 255.0f };
	const Color ground{ 1.f * 255.0f, 1.f * 255.0f, 1.f * 255.0f };

	Color Get(const Vector3& dir)
	{
		// conversion [-1;+1] vers [0;1]
		// equivalent de (y + 1) / 2
		float t = dir.y * 0.5f + 0.5f;
		// simple interpolation lineaire (lerp) 
		// A*t + B*(1 - t)
		// A + t*(B - A)
		return sky * t + ground * (1.f - t);
	}
};