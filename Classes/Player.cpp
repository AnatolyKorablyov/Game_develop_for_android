#include "Player.h"
#include "Bullet.h"
#include "Definitions.h"
#include <iterator>
#include "SimpleAudioEngine.h"

Player::Player() = default;

void Player::initOptions()
{
	this->setScale(1.5f);
	this->SetDamage(10);
	this->SetHealthPoint(100);
	this->SetSpeed(120);
	this->SetDirection(MovementDirection(0, 0));
	this->m_actionState = this->ActionStay;
	this->m_weaponState = this->WeaponState::WeaponKnife;
	this->runAction(RepeatForever::create(GetAnimate(m_actionState, m_weaponState)));
	this->m_currentAction = GetAnimate(ActionAttack, m_weaponState);
	this->runAction(m_currentAction);
	this->schedule(schedule_selector(Player::Update), 1 / 60);

	auto playerPhysicsBoby = PhysicsBody::createBox(this->getContentSize());
	playerPhysicsBoby->setCollisionBitmask(PLAYER_BITMASK);
	playerPhysicsBoby->setContactTestBitmask(true);
	playerPhysicsBoby->setDynamic(false);
	this->setPhysicsBody(playerPhysicsBoby);

}

void Player::SetState(double const & move)
{
	if (m_currentAction->isDone())
	{
		if (m_actionState == ActionStay && move != 0)
		{
			this->stopAllActions();
			this->Run();
			this->m_actionState = ActionRun;
		}
		else if (m_actionState == ActionRun && move == 0)
		{
			this->stopAllActions();
			this->Stay();
			this->m_actionState = ActionStay;
		}
	}
}

void Player::SetShootPosition(float angle)
{
	auto bulX = 20;
	auto bulY = 60;

	auto bulPosX = bulX * cos(angle) - bulY * sin(angle);
	auto bulPosY = bulX * sin(angle) + bulY * cos(angle);
	m_shootPosition = cocos2d::Vec2(float(this->getPosition().x + bulPosX), float(this->getPosition().y + bulPosY));
}

void Player::ChooseNextGun()
{
	if ((m_weaponState == WeaponState::WeaponKnife))
	{
		m_weaponState = WeaponState::WeaponPistol;
	}
	else if ((m_weaponState == WeaponState::WeaponPistol))
	{
		m_weaponState = WeaponState::WeaponM4;
	}
	else
	{
		m_weaponState = WeaponState::WeaponKnife;
	}
	m_currentAction = GetAnimate(ActionAttack, m_weaponState);

	this->stopAllActions();
	m_currentAction == ActionRun ? Run() : Stay();
	runAction(m_currentAction);
}

void Player::SetLight(cocos2d::Sprite * mask_light)
{
	m_mask = mask_light;
}


void Player::SetHPLine(cocos2d::Sprite * line)
{
	m_healthLine = line;
}


bool Player::CanHit()
{
	if (m_weaponState == WeaponKnife)
	{
		return isShoot;
	}
	return false;
}

void Player::SetHealthLabel(cocos2d::Label * healthLabel)
{
	m_healthLabel = healthLabel;
}

void Player::AddHealth(float health)
{
	auto NewHealth = (GetHealthPoint() + health) > PLAYER_HP ? PLAYER_HP : (GetHealthPoint() + health);
	SetHealthPoint(NewHealth);
	UpdateHPLine();
}

void Player::UpdateHPLine()
{
	std::stringstream ss;
	ss << GetHealthPoint();
	m_healthLabel->setString(ss.str());
}

void Player::UpdateHPLabel()
{

	std::stringstream ss;
	ss << GetHealthPoint();
	m_healthLabel->setString(ss.str());
}

void Player::initKnifeAnimations()
{
	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	std::vector<cocos2d::RefPtr<Animate>> animationsVector;

	auto animFrames = Vector<SpriteFrame*>(20);

	char str[100] = { 0 };
	for (int i = 0; i < 6; i++)
	{
		sprintf(str, "player_move_axe_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionStay, WeaponKnife)] = Animate::create(animation);
	animFrames.clear();

	for (int i = 0; i < 6; i++)
	{
		sprintf(str, "player_move_axe_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionRun, WeaponKnife)] = Animate::create(animation);

	animFrames.clear();

	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "player_attack_axe_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		frame->setOffset(this->getOffsetPosition());
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionAttack, WeaponKnife)] = Animate::create(animation);
}

void Player::initPistolAnimations()
{
	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	std::vector<cocos2d::RefPtr<Animate>> animationsVector;

	auto animFrames = Vector<SpriteFrame*>(20);

	char str[100] = { 0 };
	for (int i = 0; i < 12; i++)
	{
		sprintf(str, "player_second_weapon_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionStay, WeaponPistol)] = Animate::create(animation);

	animFrames.clear();

	for (int i = 0; i < 12; i++)
	{
		sprintf(str, "player_second_weapon_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionRun, WeaponPistol)] = Animate::create(animation);


	animFrames.clear();

	for (int i = 0; i < 12; i++)
	{
		sprintf(str, "player_second_weapon_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionAttack, WeaponPistol)] = Animate::create(animation);
}

void Player::initMainWeaponAnimations()
{
	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	std::vector<cocos2d::RefPtr<Animate>> animationsVector;

	auto animFrames = Vector<SpriteFrame*>(20);

	char str[100] = { 0 };
	for (int i = 0; i < 14; i++)
	{
		sprintf(str, "player_weapon_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionStay, WeaponM4)] = Animate::create(animation);

	animFrames.clear();

	for (int i = 0; i < 14; i++)
	{
		sprintf(str, "player_weapon_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionRun, WeaponM4)] = Animate::create(animation);


	animFrames.clear();

	for (int i = 0; i < 14; i++)
	{
		sprintf(str, "player_weapon_%02d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, 0.05);
	animationsVector.push_back(Animate::create(animation));
	m_anims[GetAnimateIndex(ActionAttack, WeaponM4)] = Animate::create(animation);
}

Player* Player::create(Joystick *moveJoy, Joystick *rotateJoy)

{
	Player* pSprite = new Player();
	
	pSprite->m_moveController = moveJoy;
	pSprite->m_rotateController = rotateJoy;
	pSprite->m_anims.resize(NumOfActions * NumOfWeapons);

	pSprite->initKnifeAnimations();
	pSprite->initPistolAnimations();
	pSprite->initMainWeaponAnimations();

	if (pSprite->initWithSpriteFrameName("player_move_axe_00.png"))
	{
		pSprite->autorelease();
		pSprite->initOptions();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}

void Player::Update(float dt)
{
	if (IsLife())
	{
		if (m_weaponState == WeaponKnife && m_currentAction->isDone())
		{
			this->schedule(schedule_selector(Player::StopAttack), 0.1f);
		}
		// направление (куда)
		double x = m_moveController->getVelocity().x;
		double y = m_moveController->getVelocity().y;

		double dx = 0;
		double dy = 0;


		auto angle = this->getRotation();
		angle = (abs(360 - angle));
		angle = (int(angle) % 360 * M_PI) / 180;
		if (m_moveController->getVelocity().x != 0)
		{
			dx = x * cos(angle) - y * sin(angle);
		}
		if (m_moveController->getVelocity().y != 0)
		{
			dy = x * sin(angle) + y * cos(angle);
		}
		// поворот системы координаты
		x = dx + this->getPosition().x;
		y = dy + this->getPosition().y;
		SetShootPosition(angle);
		this->SetDirection(MovementDirection(m_moveController->getVelocity().x, m_moveController->getVelocity().y));

		m_shootDirection = (MovementDirection(-sin(angle), cos(angle)));
		if (!CanWalkDirectly(this->getPosition(), this->GetDeltaPosition(dt)))
		{
			this->Move(dt);
		}
		SetState(dx + dy);
		////func
		double rotateX = m_rotateController->getVelocity().x;

		this->setRotation(this->getRotation() + rotateX * ROTATE_SPEED);
		this->m_mask->setPosition(this->getPosition().x, this->getPosition().y);
		this->m_mask->setRotation(this->getRotation() + rotateX * ROTATE_SPEED);
	}
	else
	{
		Destroy();
	}
}

void Player::Shoot(cocos2d::Layer * layer)
{
	isShoot = true;
	if (m_weaponState != WeaponState::WeaponKnife)
	{
		if (m_weaponState == WeaponState::WeaponM4)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SHOOT_SOUND.c_str());
		}
		if (m_weaponState == WeaponState::WeaponPistol)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SECOND_SOUND.c_str());
		}
		auto bullet = CBullet::create(this->m_shootDirection, this->m_shootPosition);
		bullet->SetObstacles(GetObstacles());
		bullet->setRotation(this->getRotation() - 90);
		layer->addChild(bullet);
	}
	
	
	this->stopAllActions();
	m_actionState = ActionRun;
	m_currentAction = GetAnimate(ActionAttack, m_weaponState);
	this->runAction(m_currentAction);
}


void Player::Run()
{
	runAction(RepeatForever::create(GetAnimate(ActionRun, m_weaponState)));
}

void Player::Stay()
{
	runAction(RepeatForever::create(GetAnimate(ActionStay, m_weaponState)));
}

cocos2d::RefPtr<Animate> Player::GetAnimate(ActionState action, WeaponState weapon)
{
	return m_anims[GetAnimateIndex(action, weapon)];
}

int Player::GetAnimateIndex(ActionState action, WeaponState weapon)
{
	return ActionState::NumOfActions * weapon + action;
}
void Player::StopAttack(float dt)
{
	isShoot = false;
}
//
//
//bool Player::CanWalkDirectly(const Vec2 &from, const Vec2 &to) const
//{
//	for (auto &it : m_obstacles)
//	{
//		/*if (it.intersectsRect(this->getBoundingBox()))
//		{*/
//			if (it.getMinX() < to.x + this->getPosition().x && to.x + this->getPosition().x < it.getMaxX()
//				&& it.getMinY() < to.y + this->getPosition().y && to.y + this->getPosition().y < it.getMaxY())
//			{
//				return true;
//			}
//		/*}*/
//	}
//	return false;
//	//Sprite* Child_1 = Sprite::create();
//	//SLine path(Child_1->convertToNodeSpace(from),
//	//	Child_1->convertToNodeSpace(to));
//	//return std::none_of(m_obstacles.begin(), m_obstacles.end(), [=](const Rect &rect) {
//	//	return path.Intersects(rect);
//	//});
//}