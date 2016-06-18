#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "CollisionMap.h"

bool IsBetween(float x, float minX, float maxX)
{
	return (minX <= x) && (x <= maxX);
}

bool SLine::Intersects(const SLine &other) const
{
	const Vec2 deltaThis = to - from;
	const Vec2 deltaOther = other.to - other.from;
	const Vec2 deltaFrom = from - other.from;
	const float denom = deltaOther.y * deltaThis.x - deltaOther.x * deltaThis.y;
	const float epsilon = std::numeric_limits<float>::epsilon();
	// Если знаменатель равен нулю, то прямые параллельны или совпадают
	if (IsBetween(denom, -epsilon, +epsilon))
	{
		return false;
	}
	const float numThis = deltaOther.x * deltaFrom.y - deltaOther.y * deltaFrom.x;
	const float numOther = deltaThis.x * deltaFrom.y - deltaThis.y * deltaFrom.x;
	const float factorThis = numThis / denom;
	const float factorOther = numOther / denom;

	return IsBetween(factorThis, 0, 1) && IsBetween(factorOther, 0, 1);
}

bool SLine::Intersects(const Rect &other) const
{
	Vec2 topLeft(other.getMinX(), other.getMinY());
	Vec2 topRight(other.getMaxX(), other.getMinY());
	Vec2 bottomLeft(other.getMinX(), other.getMaxY());
	Vec2 bottomRight(other.getMaxX(), other.getMaxY());

	return Intersects(SLine(topLeft, topRight))
		|| Intersects(SLine(topLeft, bottomLeft))
		|| Intersects(SLine(topRight, bottomRight))
		|| Intersects(SLine(bottomLeft, bottomRight))
		|| other.containsPoint(from);
}