#pragma once
#include "cocos2d.h"
#include "Entity.h"
#include "Player.h"
#include <vector>

class CHungry : public CEntity
{
public:
	static CHungry *create(cocos2d::Point const& position, Player * player);
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
	CHungry();
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

