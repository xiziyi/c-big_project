#ifndef _VECTOR_LAYER_H_
#define _VECTOR_LAYER_H_

#include "cocos2d.h"

class VectorLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(VectorLayer);
};
#endif
