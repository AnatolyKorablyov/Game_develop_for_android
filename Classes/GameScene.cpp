#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenu.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "HungryAngry.h"
#include "Bonus.h"
#include "WinScene.h"
#include "LoseScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	scene->addChild(layer->m_upperLayer);
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//1. super init first
	if (!Layer::init())
	{
		return false;
	}

	/*CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(POINT_SOUND.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(LOSE_SOUND.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(DROP_SOUND.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BG_SOUND.c_str());*/
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	MapInit();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	this->addChild(edgeNode);

	auto joy = Joystick::create();
	auto joyR = Joystick::create();
	joyR->SetCenter(Director::getInstance()->getVisibleSize().width - 128, 0);
	m_joy = joy;
	m_joyR = joyR;

	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	
	sharedSpriteFrameCache->addSpriteFramesWithFile("gorbachev.plist");

	sharedSpriteFrameCache->addSpriteFramesWithFile("witch_sleep.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("witch_run.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("witch_attack.plist");

	sharedSpriteFrameCache->addSpriteFramesWithFile("player_m4.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("player_pistol.plist");
	//sharedSpriteFrameCache->addSpriteFramesWithFile("player_moveAXE.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("player_moveable_axe.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("player_attackAXE.plist");

	m_player = Player::create(joy, joyR);
	this->m_player->setPosition(572, 873);
	this->addChild(m_player);


	m_upperLayer = cocos2d::Layer::create();
	m_upperLayer->addChild(m_joy);
	m_upperLayer->addChild(m_joyR);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchBeganMultiTouch, this);
	listener->onTouchesBegan = CC_CALLBACK_2(GameScene::moveByMultiTouch, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	//////listener->onTouchMoved = CC_CALLBACK_2(GameScene::UpdateCamera, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(GameScene::UpdateCamera), float(1 / 60));

	auto playItem = MenuItemImage::create("GUN.png", "GUN.png", CC_CALLBACK_1(GameScene::ChooseGun, this));
	playItem->setPosition(Point(visibleSize.width - playItem->getContentSize().width + origin.x, visibleSize.height / 2 + origin.y));

	auto menu = Menu::create(playItem, NULL);
	menu->setPosition(Point::ZERO);
	m_upperLayer->addChild(menu);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	auto mask_light = cocos2d::Sprite::create("mask.png", Rect(0, 0, 3000, 3000));
	//mask_light->initWithFile("mask.png");
	mask_light->setPosition(m_player->getPosition());
	m_player->SetLight(mask_light);
	this->addChild(mask_light, 12);
	//m_upperLayer->addChild(mask_light);

	auto HPLine = cocos2d::Sprite::create();
	HPLine->initWithFile("health_line.png");
	HPLine->setPosition(visibleSize.width - 15, visibleSize.height / 2);
	
	m_player->SetHPLine(HPLine);
	//m_upperLayer->addChild(HPLine);


	LoadObstacles();
	PlayerSpawn();
	LoadEnemy();
	LoadHungry();
	LoadHealthBonuses();
	LoadExit();
	InitHealthLabel();
	m_player->SetObstacles(&m_obstacles);
	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(m_player->getPosition().x);
	cameraTarget->setPositionY(m_player->getPosition().y);
	cameraTarget->retain();

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();

	this->runAction(camera);

	return true;
}

bool GameScene::onTouchBegan(Touch * pTouches, Event * event)
{
	if (!m_joy->isContainTouch(pTouches) && !m_joyR->isContainTouch(pTouches))
	{
		m_player->Shoot(this);
	}
	return true;
}

void GameScene::onTouchBeganMultiTouch(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * event)
{
	for (Touch* touch : touches) {
		Vec2 touchLocation = touch->getLocation();
		if (!m_joy->isContainTouch(touch) && !m_joyR->isContainTouch(touch))
		{
			m_player->Shoot(this);
		}
	}
}


void GameScene::initMultiTouch()
{
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2
	(GameScene::onTouchBeganMultiTouch, this);
	listener->onTouchesMoved = CC_CALLBACK_2
	(GameScene::moveByMultiTouch, this);
	listener->onTouchesEnded = [](const std::vector<Touch*>&
		touches, Event* event) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::moveByMultiTouch(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * event)
{
	for (Touch* touch : touches) {
		Vec2 touchLocation = touch->getLocation();
		m_joy->isContainTouch(touch);
		m_joyR->isContainTouch(touch);
	}
}

void GameScene::CheckGameCollision(cocos2d::PhysicsBody & first, cocos2d::PhysicsBody & second)
{
	auto firstBitmask = first.getCollisionBitmask();
	auto secondBitmask = second.getCollisionBitmask();

	if (firstBitmask == PLAYER_BITMASK && secondBitmask == ENEMY_BITMASK)
	{
		static_cast<CEnemy*>(second.getNode())->MakeAngry();
	}
	else if (firstBitmask == ENEMY_BITMASK && secondBitmask == PLAYER_BITMASK)
	{
		static_cast<CEnemy*>(first.getNode())->MakeAngry();
	}
	else if (firstBitmask == BULLET_BITMASK && secondBitmask == ENEMY_BITMASK)
	{
		auto enemy = static_cast<CEnemy*>(second.getNode());
		auto bullet = static_cast<CBullet*>(first.getNode());
		enemy->Demaged(bullet->GetDamage());
		bullet->Destroy();
		enemy->MakeAngry();
	}
	else if (firstBitmask == ENEMY_BITMASK && secondBitmask == BULLET_BITMASK)
	{
		auto enemy = static_cast<CEnemy*>(first.getNode());
		auto bullet = static_cast<CBullet*>(second.getNode());
		enemy->Demaged(bullet->GetDamage());
		bullet->Destroy();
		enemy->MakeAngry();
	}
	else if (firstBitmask == PLAYER_BITMASK && secondBitmask >= AMMO_BITMASK && secondBitmask <= HEALTH_BITMASK)
	{
		auto player = static_cast<Player*>(first.getNode());
		auto bonuce = static_cast<CBonus*>(second.getNode());
		if (secondBitmask == HEALTH_BITMASK)
		{
			player->AddHealth(bonuce->GetHealthPoint());
		}
		bonuce->Kill();
	}
	else if ((firstBitmask == EXIT_BITMASK && secondBitmask == PLAYER_BITMASK) 
		|| (firstBitmask == PLAYER_BITMASK && secondBitmask == EXIT_BITMASK))
	{
		auto player = static_cast<Player*>(second.getNode());
		GoToWinScene();
	}
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact & contact)
{
	PhysicsBody *first = contact.getShapeA()->getBody();
	PhysicsBody *second = contact.getShapeB()->getBody();
	CheckGameCollision(*first, *second);
	return true;

}

void GameScene::InitHealthLabel()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::stringstream ss;
	ss << m_player->GetHealthPoint();
	m_healthLabel = Label::createWithTTF(ss.str(), AMMO_FONT, visibleSize.height * FONT_SIZE);
	m_healthLabel->setColor(Color3B::RED);
	m_healthLabel->setPosition(visibleSize.width - 50, visibleSize.height - 30);
	m_player->SetHealthLabel(m_healthLabel);
	m_upperLayer->addChild(m_healthLabel);
}

void GameScene::UpdateCamera(float dt)
{
	cameraTarget->setPositionX(m_player->getPosition().x);
	cameraTarget->setPositionY(m_player->getPosition().y);
	cameraTarget->retain();

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();

	this->runAction(camera);
	if (!m_player->IsLife())
	{
		GoToLoseScene();
	}
}

void GameScene::ChooseGun(cocos2d::Ref *sender)
{
	m_player->ChooseNextGun();
}

void GameScene::MapInit()
{
	m_tileMap = new CCTMXTiledMap();
	m_tileMap->initWithTMXFile("office.tmx");
	m_background = m_tileMap->layerNamed("background");
	this->addChild(m_tileMap);

}

bool GameScene::LoadObstacles()
{
	CCTMXObjectGroup *objectGroup = m_tileMap->objectGroupNamed("solid");
	try
	{
		for (Value object : objectGroup->getObjects())
		{
			m_obstacles.emplace_back(AsRect(object.asValueMap()));
		}
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}

bool GameScene::LoadEnemy()
{
	CCTMXObjectGroup *objectGroup = m_tileMap->objectGroupNamed("enemy");
	try
	{
		for (Value object : objectGroup->getObjects())
		{
			Rect enemy = AsRect(object.asValueMap());
			auto testEnemy = CEnemy::create(Vec2(enemy.getMidX(), enemy.getMidY()), m_player);
			testEnemy->SetObstacles(&m_obstacles);
			//testEnemy->MakeAngry();
			this->addChild(testEnemy, 10);
		}
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}
bool GameScene::LoadHungry()
{
	CCTMXObjectGroup *objectGroup = m_tileMap->objectGroupNamed("hungry");
	try
	{
		for (Value object : objectGroup->getObjects())
		{
			Rect enemy = AsRect(object.asValueMap());
			auto testEnemy = CHungry::create(Vec2(enemy.getMidX(), enemy.getMidY()), m_player);
			testEnemy->SetObstacles(&m_obstacles);
			//testEnemy->MakeAngry();
			this->addChild(testEnemy, 10);
		}
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}
bool GameScene::PlayerSpawn()
{
	CCTMXObjectGroup *objectGroup = m_tileMap->objectGroupNamed("playerSpawn");
	try
	{
		for (Value object : objectGroup->getObjects())
		{
			Rect enemy = AsRect(object.asValueMap());
			m_player->setPosition(enemy.getMidX(), enemy.getMidY());
		}
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}

bool GameScene::LoadHealthBonuses()
{
	CCTMXObjectGroup *objectGroup = m_tileMap->objectGroupNamed("health");
	try
	{
		for (Value object : objectGroup->getObjects())
		{
			Rect bonuce = AsRect(object.asValueMap());
			auto bon = CBonus::create(Vec2(bonuce.getMidX(), bonuce.getMidY()), CBonus::Health);
			this->addChild(bon);
		}
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}

bool GameScene::LoadExit()
{
	TMXObjectGroup *objectGroup = m_tileMap->objectGroupNamed("exit");
	try
	{
		for (Value object : objectGroup->getObjects())
		{
			Rect exit = AsRect(object.asValueMap());
			auto ex = CCNode::create();
			ex->setPosition(Vec2(exit.getMidX(), exit.getMidY()));
			ex->setContentSize(cocos2d::Size(30, 30));
			auto PhysicsBoby = cocos2d::PhysicsBody::createBox(this->getContentSize());
			PhysicsBoby->setCollisionBitmask(EXIT_BITMASK);
			PhysicsBoby->setContactTestBitmask(true);
			PhysicsBoby->setGravityEnable(false);
			PhysicsBoby->setMass(10000.0f);
			PhysicsBoby->setDynamic(true);
			ex->setPhysicsBody(PhysicsBoby);
			this->addChild(ex);
		}
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}

Rect GameScene::AsRect(const ValueMap &properties) const
{
	Rect rect;
	rect.origin.x = properties.at("x").asFloat();
	rect.origin.y = properties.at("y").asFloat();
	rect.size.width = properties.at("width").asFloat();
	rect.size.height = properties.at("height").asFloat();
	return rect;
}

void GameScene::GoToWinScene()
{
	auto scene = WinScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameScene::GoToLoseScene()
{
	auto scene = LoseScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
