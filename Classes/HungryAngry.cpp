#include "HungryAngry.h"
#include "MathAndLogic.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"

CHungry * CHungry::create(cocos2d::Point const& position, Player * player)
{
	CHungry* pSprite = new CHungry();

	pSprite->setPosition(position);
	pSprite->m_target = player;
	pSprite->initAnimations();
	if (pSprite->initWithSpriteFrameName("zombie_00.png"))
	{
		pSprite->autorelease();

		pSprite->initOptions();

		return pSprite;
	}



	CC_SAFE_DELETE(pSprite);
	return nullptr;
}

void CHungry::initAnimations()
{
	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	std::vector<cocos2d::RefPtr<Animate>> animationsVector;

	auto animFrames = Vector<SpriteFrame*>(20);

	char str[100] = { 0 };
	for (int i = 0; i < 7; i++)
	{
		sprintf(str, "zombie_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_stay = Animate::create(animation);
	animFrames.clear();

	for (int i = 0; i < 7; i++)
	{
		sprintf(str, "zombie_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_run = Animate::create(animation);
	m_attack = m_run;
	animFrames.clear();

	for (int i = 0; i < 7; i++)
	{
		sprintf(str, "zombie_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		//frame->setOffset(this->getOffsetPosition());
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_attack = Animate::create(animation);
}

void CHungry::Update(float dt)
{
	if (this->getBoundingBox().intersectsRect(m_target->getBoundingBox()) && m_target->CanHit())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(WITCH_PAIN_SOUND.c_str());
		Demaged(m_target->GetDamage());
	}
	if (IsLife())
	{
		if (m_angry)
		{
			if (m_isAttack)
			{
					if (this->getBoundingBox().intersectsRect(m_target->getBoundingBox()))
					{

						m_target->Demaged(GetDamage());
						m_target->UpdateHPLine();
					}
					m_isAttack = false;
					Run();

			}
			else
			{

				auto angle = SMath::CalculateAngle(this->getPosition(), m_target->getPosition());
				this->setRotation(angle + 90);
				this->SetDirection(MovementDirection(sin(angle), cos(angle)));
				if (!CanWalkDirectly(getPosition(), GetDeltaPosition(dt)))
				{
					Move(dt);
				}
				if (this->getBoundingBox().intersectsRect(m_target->getBoundingBox()))
				{
					MakeAngry();
					TryToAttack();
				}
			}
		}
	}
	else
	{
		this->Destroy();
	}
}

void CHungry::MakeAngry()
{
	if (!m_angry)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(WITCH_ATTACK_SOUND.c_str());
	}
	m_angry = true;
}


void CHungry::initOptions()
{
	this->setScale(1.5f);
	this->m_angry = false;
	this->m_isAttack = false;
	this->SetSpeed(60);
	this->SetDamage(5.f);
	this->SetHealthPoint(50);
	this->m_action = ActionStay;
	this->schedule(schedule_selector(CHungry::Update), 1 / 60);

	auto playerPhysicsBoby = PhysicsBody::createBox(this->getContentSize());
	playerPhysicsBoby->setCollisionBitmask(ENEMY_BITMASK);
	playerPhysicsBoby->setContactTestBitmask(true);
	playerPhysicsBoby->setGravityEnable(false);
	playerPhysicsBoby->setMass(0);
	this->setPhysicsBody(playerPhysicsBoby);

	this->Stay();
}

CHungry::CHungry() = default;

void CHungry::Run()
{
	stopAllActions();
	runAction(RepeatForever::create(m_run));
}

void CHungry::Stay()
{
	stopAllActions();
	runAction(RepeatForever::create(m_stay));
}

void CHungry::TryToAttack()
{
	m_isAttack = true;
	stopAllActions();
	runAction(m_run);
}