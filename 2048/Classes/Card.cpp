#include "Card.h"

Card* Card::createCard(int num, int cardWidth, int cardHeight, float positionX, float positionY)
{
	auto card = new Card();
	if (card && card->init())
	{
		card->autorelease();
		card->cardInit(num, cardWidth, cardHeight, positionX, positionY);
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
}

bool Card::init()
{
	if (!Sprite::init())return false;

	return true;
}

void Card::runcreateaction()
{
	cardBgColour->setScale(0.0f);
	auto act = ScaleTo::create(0.2f, 1.0f, 1.0f);
	cardBgColour->runAction(act);
}

void Card::setNumber(int num)
{
	number = num;
	//显示数字
	if (number > 0)
	{
		cardLabel->setString(__String::createWithFormat("%d", number)->getCString());
		auto act = ScaleBy::create(0.1f, 1.8f, 1.8f);
		auto seq = Sequence::create(act->clone(), act->clone()->reverse(), nullptr);
		cardLabel->runAction(seq->clone());
	}
	else
	{
		cardLabel->setString("");
	}
	//设置字体的大小
    cardLabel->setSystemFontSize(50);

	//设置数字颜色
	if (number == 0) {
		cardBgColour->setColor(Color3B(200, 190, 180));
	}
	if (number == 2) {
		cardBgColour->setColor(Color3B(240, 230, 220));
	}
	if (number == 4) {
		cardBgColour->setColor(Color3B(51, 153, 51));
	}
	if (number == 8) {
		cardBgColour->setColor(Color3B(255, 153, 102));
	}
	if (number == 16) {
		cardBgColour->setColor(Color3B(153, 204, 153));
	}
	if (number == 32) {
		cardBgColour->setColor(Color3B(153, 204, 255));
	}
	if (number == 64) {
		cardBgColour->setColor(Color3B(255, 204, 204));
	}
	if (number == 128) {
		cardBgColour->setColor(Color3B(204, 102, 0));
	}
	if (number == 256) {
		cardBgColour->setColor(Color3B(153, 204, 51));
	}
	if (number == 512) {
		cardBgColour->setColor(Color3B(255, 102, 102));
	}
	if (number == 1024) {
		cardBgColour->setColor(Color3B(204, 204, 255));
	}
	if (number == 2048) {
		cardBgColour->setColor(Color3B(255, 204, 00));
	}
	if (number == 4096) {
		cardBgColour->setColor(Color3B(255, 128, 00));
	}
	if (number == 8192) {
		cardBgColour->setColor(Color3B(255, 00, 00));
	}
}

int Card::getNumber() const
{
	return number;
}
std::string Card::GetNumStr()
{
	std::string s1 = StringUtils::format("%d", number);
	return s1;
}
bool Card::moveto(Vec2 nextpos,float time)
{
	return true;
}

Vec2 Card::getpos() const
{
	return cardBgColour->getPosition();
}

void Card::cardInit(int num, int cardWidth, int cardHeight, float positionX, float positionY)
{
	number = num;

	//添加卡片背景颜色
	cardBgColour = LayerColor::create(Color4B(200, 190, 180, 255), cardWidth - 5, cardHeight - 5);
	cardBgColour->setPosition(positionX, positionY);

	//判断num是否大于0，是就显示，否则显示空
	if (num > 0)
	{
		//设置字体
		cardLabel = Label::createWithTTF(__String::createWithFormat("%d", number)->getCString(), "fonts/arial.ttf", 50);
		cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
		cardBgColour->addChild(cardLabel);
	}
	else
	{
		cardLabel = Label::createWithTTF("", "fonts/arial.ttf", 50);
		cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
		cardBgColour->addChild(cardLabel);
	}
	this->addChild(cardBgColour);
}