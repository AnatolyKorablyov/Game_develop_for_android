#include "Entity.h"
#include "ui/CocosGUI.h" 
#include "MathAndLogic.h"
#include "Definitions.h"


CEntity::CEntity()
	: m_damage(0)
	, m_health(0)
	, m_isLife(true)
{
}


CEntity::~CEntity()
{
}

void CEntity::Move(float dt)
{
	this->setPosition(MoveToward(this->getPosition(), dt));
}

void CEntity::SetHealthPoint(float health)
{
	m_health = health;
}

float CEntity::GetHealthPoint()
{
	return m_health;
}

void CEntity::SetDamage(float damage)
{
	m_damage = damage;
}

float CEntity::GetDamage()
{
	return m_damage;
}

void CEntity::Demaged(float damage)
{
	m_health -= damage;
	if (m_health <= 0)
	{
		m_isLife = false;
	}
}

bool CEntity::IsLife()
{
	return m_isLife;
}

void CEntity::Destroy()
{
	AddBlood();
	this->removeFromParentAndCleanup(true);
}

bool CEntity::CanWalkDirectly(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to) const
{
	for (auto &it : *m_obstacles)
	{
		if ((it.getMinX() < to.x + this->getPosition().x + this->getContentSize().width * 0.1 || it.getMinX() < to.x + this->getPosition().x - this->getContentSize().width * 0.1)
			&& (to.x + this->getPosition().x < it.getMaxX() + this->getContentSize().width * 0.1 || to.x + this->getPosition().x < it.getMaxX() - this->getContentSize().width * 0.1)
			&& (it.getMinY() < to.y + this->getPosition().y + this->getContentSize().height * 0.1 || it.getMinY() < to.y + this->getPosition().y - this->getContentSize().height * 0.1)
			&& (to.y + this->getPosition().y < it.getMaxY() + this->getContentSize().height * 0.1 || to.y + this->getPosition().y < it.getMaxY() - this->getContentSize().height * 0.1))
		{
			return true;
		}
	}
	return false;
}

void CEntity::SetObstacles(std::vector<cocos2d::Rect>* obstacles)
{
	m_obstacles = obstacles;
}

std::vector<cocos2d::Rect>* CEntity::GetObstacles()
{
	return m_obstacles;
}

cocos2d::Vec2 CEntity::GetDeltaPosition(float dt)
{
	return cocos2d::Vec2(this->GetSpeed() * dt * this->GetDirection().x,
		this->GetSpeed() * dt * this->GetDirection().y);
}

void CEntity::Kill()
{
	m_isLife = false;
}

void CEntity::SetLowerLayer(cocos2d::Layer * layer)
{
	m_lowerLayer = layer;
}

void CEntity::AddBlood()
{
	if (m_lowerLayer != nullptr)
	{
		auto num = cocos2d::RandomHelper::random_int(1, 3);
		auto blood = cocos2d::Sprite::create();
		switch (num)
		{
		case 1: blood->initWithFile(BLOOD1); break;
		case 2: blood->initWithFile(BLOOD2); break;
		case 3: blood->initWithFile(BLOOD3); break;
		}
		blood->setPosition(this->getPosition());
		m_lowerLayer->addChild(blood);
	}
}
