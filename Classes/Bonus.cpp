#include "Bonus.h"
#include "Definitions.h"

std::string GetBonuseFile(CBonus::Type const& type)
{
	switch (type)
	{
	case CBonus::Health: return BONUS_HEALTH;
	case CBonus::Ammo: return BONUS_AMMO;

	}
}


CBonus * CBonus::create(cocos2d::Point const& position, CBonus::Type type)

{
	CBonus* pSprite = new CBonus();
	pSprite->m_type = type;
	pSprite->setPosition(position);

	if (pSprite->initWithFile(GetBonuseFile(pSprite->m_type)))
	{
		pSprite->autorelease();

		pSprite->initOptions();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}


void CBonus::Update(float dt)
{
	if (!IsLife())
	{
		Destroy();
	}
}

int GetBitMask(CBonus::Type type)
{
	switch (type)
	{
	case CBonus::Health:
		return HEALTH_BITMASK;
	case CBonus::Ammo:
		return AMMO_BITMASK;
	}
}

void CBonus::initOptions()
{
	this->SetHealthPoint(BONUCE_HP);
	this->schedule(schedule_selector(CBonus::Update), float(1 / 60));

	auto PhysicsBoby = cocos2d::PhysicsBody::createBox(this->getContentSize());
	PhysicsBoby->setCollisionBitmask(GetBitMask(m_type));
	PhysicsBoby->setContactTestBitmask(true);
	PhysicsBoby->setGravityEnable(false);
	PhysicsBoby->setDynamic(true);
	this->setPhysicsBody(PhysicsBoby);
}

CBonus::CBonus() = default;
