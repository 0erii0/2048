#pragma once

#include "cocos2d.h"

USING_NS_CC;
class Card : public Sprite
{
public:
	//初始化
	static Card* createCard(int num, int cardWidth, 
		int cardHeight,float positionX, float positionY);
	bool init() override;

	//运行创建的动画
	void runcreateaction();

	CREATE_FUNC(Card);

	//设置数字并显示的方法
	void setNumber(int num);

	//获取数字的方法
	int getNumber() const;
	std::string GetNumStr();

	//移动动画
	bool moveto(Vec2 nextpos, float time = 0.1f);

	//获取位置
	Vec2 getpos() const;
private:
	//卡片中的数字
	int number = 0;;
	void cardInit(int num, int cardWidth,
		int cardHeight, float positionX, float positionY);

	//用于显示数字的控件
	Label *cardLabel=nullptr;
	
	//控件的背景颜色
	LayerColor *cardBgColour=nullptr;
};
