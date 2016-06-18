#pragma once
#include "cocos2d.h" 
#include "Joystick.h"
#include "Entity.h"
#include <vector>
#include <map>
#include "CollisionMap.h"

class Player : public CEntity
{
public:
	static Player *create(Joystick *moveJoy, Joystick *rotateJoy);
	void Update(float dt) override;
	void Shoot(cocos2d::Layer *layer);
	void ChooseNextGun();

	void SetLight(cocos2d::Sprite * mask_light);
	
	void SetHPLine(cocos2d::Sprite * line);
	bool CanHit();
	void SetHealthLabel(cocos2d::Label * healthLabel);
	void AddHealth(float health);
	void UpdateHPLine();
	void UpdateHPLabel();
private:
	enum ActionState
	{
		ActionStay = 0,
		ActionRun,
		ActionAttack,
		NumOfActions,
	};

	enum WeaponState
	{
		WeaponKnife = 0,
		WeaponPistol,
		WeaponM4,
		NumOfWeapons
	};

	void Run();
	void Stay();

	cocos2d::RefPtr<Animate> GetAnimate(ActionState action, WeaponState weapon);
	int GetAnimateIndex(ActionState action, WeaponState weapon);

	void StopAttack(float dt);

	/*bool CanWalkDirectly(const Vec2 & from, const Vec2 & to) const;
*/
	ActionState m_actionState;
	WeaponState m_weaponState;

	Player();
	void initOptions();
	void SetState(double const& move);
	void SetShootPosition(float angle);

	void initKnifeAnimations();
	void initPistolAnimations();

	void initMainWeaponAnimations();

private:
	Joystick * m_moveController;
	Joystick * m_rotateController;

	std::vector<cocos2d::RefPtr<Animate>> m_anims;

	cocos2d::RefPtr<Animate> m_currentAction;
	
	cocos2d::Vec2 m_shootPosition;
	MovementDirection m_shootDirection;

	cocos2d::Sprite * m_healthLine;
	cocos2d::Sprite * m_mask;
	bool isShoot = false;
	cocos2d::Label * m_healthLabel;
	std::vector<cocos2d::Rect> m_obstacles;
};

