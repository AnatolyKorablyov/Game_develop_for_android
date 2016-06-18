#include "Bullet.h"
#include "Definitions.h"

CBullet * CBullet::create(MovementDirection const& direction, cocos2d::Point const& position)

{
	CBullet* pSprite = new CBullet();
	pSprite->setPosition(position);
	pSprite->SetDirection(direction);


	if (pSprite->initWithFile("bullet.png"))
	{
		pSprite->autorelease();

		pSprite->initOptions();

		//	pSprite->runAction(RepeatForever::create(Animate::create(pSprite->stayWithKnife)));
		return pSprite;
	}



	CC_SAFE_DELETE(pSprite);
	return nullptr;
}


void CBullet::Update(float dt)
{
	CanWalkDirectly(getPosition(), GetDeltaPosition(dt)) ? Kill() : Move(dt);
	if (!IsLife())
	{
		Destroy();
	}
}

void CBullet::initOptions()
{
	this->SetSpeed(1000);
	this->SetDamage(10);
	this->SetHealthPoint(1);
	this->schedule(schedule_selector(CBullet::Update), 1 / 60);

	auto playerPhysicsBoby = cocos2d::PhysicsBody::createBox(this->getContentSize());
	playerPhysicsBoby->setCollisionBitmask(BULLET_BITMASK);
	playerPhysicsBoby->setContactTestBitmask(true);
	playerPhysicsBoby->setDynamic(false);
	playerPhysicsBoby->setMass(0);
	this->setPhysicsBody(playerPhysicsBoby);
}

CBullet::CBullet() = default;
