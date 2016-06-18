#pragma once
#include "cocos2d.h"

using namespace cocos2d;

struct SLine
{
	Vec2 from;
	Vec2 to;

	SLine() = default;
	SLine(const Vec2 &from, const Vec2 &to)
		: from(from)
		, to(to)
	{
	}

	bool Intersects(const SLine &other)const;
	bool Intersects(const Rect &other)const;
};
