
#pragma once
#include "cocos2d.h" 

using namespace cocos2d;

class IJoystick
{
public:
	virtual ~IJoystick() = default;
	virtual CCPoint getVelocity() = 0;
	// TODO: add other required methods.
};

class Joystick : public IJoystick, public CCLayer
{

public:

    virtual bool init();  
    CCPoint getVelocity() {return velocity;}
    CREATE_FUNC(Joystick);


	bool onTouchesBegan(Touch *pTouches, CCEvent *pEvent);
	void onTouchesMoved(Touch *, CCEvent *pEvent);
	void onTouchesCancelled(Touch *, CCEvent *pEvent);
	void onTouchesEnded(Touch *, CCEvent *pEvent);

	// TODO: rename to DoesContainTouch();
	bool isContainTouch(Touch *pTouches);

	void SetCenter(float const& x, float const& y);
private:

    CCPoint kCenter;
	CCPoint kNodeCenter;

    CCSprite *thumb;
    bool isPressed;

    CCPoint velocity;

    void updateVelocity(CCPoint point);
    void resetJoystick();
    bool handleLastTouch();


};
