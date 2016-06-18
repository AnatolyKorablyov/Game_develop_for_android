#pragma once
#include "cocos2d.h"
#include "Joystick.h"
#include "Player.h"
#include "CollisionMap.h"
#include "Bonus.h"
class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

private:
	//void getPlayerCoordinateForView(float x, float y, bool camp);
	bool onTouchBegan(Touch* pTouches, Event* event);

	void onTouchBeganMultiTouch(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * event);

	void initMultiTouch();
	void moveByMultiTouch(const std::vector<cocos2d::Touch*>& touches,
		cocos2d::Event* event);

	void CheckGameCollision(cocos2d::PhysicsBody & first, cocos2d::PhysicsBody & second);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	void InitHealthLabel();
	void UpdateCamera(float dt);
	void SetJoystick(Joystick *joy) { m_joy = joy; };
	void ChooseGun(cocos2d::Ref *sender);

	void MapInit();
	bool LoadObstacles();
	bool LoadEnemy();
	bool LoadHungry();
	bool PlayerSpawn();
	bool LoadHealthBonuses();
	bool LoadExit();
	Rect AsRect(const ValueMap & properties) const;

	void GoToWinScene();

	void GoToLoseScene();

private:
	Joystick *m_joy;
	Joystick *m_joyR;
	Player *m_player;

	Sprite *cameraTarget;
	Follow *camera;
	cocos2d::TMXTiledMap *m_tileMap;
	cocos2d::TMXLayer *m_background;
	std::vector<cocos2d::Rect> m_obstacles;
	
	cocos2d::Label * m_healthLabel;
	cocos2d::Layer * m_upperLayer;
};


