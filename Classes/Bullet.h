#pragma once
#include "cocos2d.h"
#include "Entity.h"

class CBullet : public CEntity
{
public: 
	static CBullet *create(MovementDirection const& direction, cocos2d::Point const& position);

	void Update(float dt) override;

private:
	CBullet();
	void initOptions();


};

