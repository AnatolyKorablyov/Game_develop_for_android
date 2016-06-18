#pragma once
#include "cocos2d.h"
#include "Entity.h"
#include "Player.h"
#include <vector>

class CEnemy : public CEntity
{
public:
	static CEnemy *create(cocos2d::Point const& position, Player * player);
	void Update(float dt) override;
	void MakeAngry();
private:
	enum ActionState
	{
		ActionStay = 0,
		ActionRun,
		ActionAttack,
		NumOfActions,
	};

	ActionState m_action;
	bool m_angry;
	bool m_isAttack;
	CEnemy();
	void initOptions();
	void initAnimations();
	void Run();
	void Stay();
	void TryToAttack();

	std::vector<cocos2d::RefPtr<Animate>> m_anims;
	cocos2d::RefPtr<Player> m_target;

	cocos2d::RefPtr<Animate> m_stay;
	cocos2d::RefPtr<Animate> m_run;
	cocos2d::RefPtr<Animate> m_attack;
};

