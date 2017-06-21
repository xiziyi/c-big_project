#ifndef _LOSS_LAYER_H_
#define _LOSS_LAYER_H_

#include "cocos2d.h"

class LossLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LossLayer);
};
#endif
