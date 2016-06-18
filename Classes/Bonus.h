#pragma once
#include "cocos2d.h"
#include "Entity.h"

class CBonus : public CEntity
{
public:	
	enum Type
	{
		Health = 0,
		Ammo
	};

	static CBonus *create(cocos2d::Point const& position, Type type);

	void Update(float dt) override;
private:
	CBonus();
	void initOptions();
	Type m_type;

	int m_bitmask;
};

