#include "LoseScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenu.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* LoseScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoseScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoseScene::init()
{
	//1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->scheduleOnce(schedule_selector(LoseScene::GoToMenuScene), 2);

	auto backgrowndSprite = Sprite::create("lose.png");
	backgrowndSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(backgrowndSprite);

	return true;
}

void LoseScene::GoToMenuScene(float dt)
{
	auto scene = MainMenu::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

