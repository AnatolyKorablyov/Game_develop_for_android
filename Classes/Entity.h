#pragma once
#include "cocos2d.h"
#include "MotorSystem.h"


class CEntity
	: public cocos2d::Sprite
	, public CMotorSystem
{
public:
	CEntity();
	~CEntity();

	virtual void Update(float dt) = 0;
	virtual void Move(float dt);

	void SetHealthPoint(float health);
	float GetHealthPoint();
	void SetDamage(float damage);
	float GetDamage();
	void Demaged(float damage);
	bool IsLife();
	void Destroy();
	bool CanWalkDirectly(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to) const;
	void SetObstacles(std::vector<cocos2d::Rect> * obstacles);
	std::vector<cocos2d::Rect> * GetObstacles();
	cocos2d::Vec2 GetDeltaPosition(float dt);
	void Kill();
	void SetLowerLayer(cocos2d::Layer * layer);
	void AddBlood();
private:

	float m_health;
	float m_damage;
	bool m_isLife;
	std::vector<cocos2d::Rect> * m_obstacles;
	cocos2d::Layer * m_lowerLayer = nullptr;
};

