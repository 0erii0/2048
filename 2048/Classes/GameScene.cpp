#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIScale9Sprite.h"
//#include "StringManager.h"
#include "ui/CocosGUI.h"
#include <sstream>
#include <tinyxml2/tinyxml2.h>
#include <vector>

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocos2d;
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

string Records[6] = {};

Game::Game() :row_(4), col_(4)
{
}

Game::Game(int row, int col) : row_(row), col_(col)
{
}

Scene* Game::createScene()
{
	return create();
}

//将当前局面写入xml文件
void Game::WriteXml()
{

	auto doc = new tinyxml2::XMLDocument();

	std::string str1 = "";
	for (int y = 0; y < row_; y++)
	{
		std::string ss = cardDataArr[0][y]->GetNumStr();
		str1 = str1 + "," + ss;
	}
	tinyxml2::XMLElement* blog = doc->NewElement("0");
	tinyxml2::XMLText* text = doc->NewText(str1.c_str());
	blog->InsertEndChild(text);
	doc->InsertEndChild(blog);

	str1 = "";
	for (int y = 0; y < row_; y++)
	{
		std::string ss = cardDataArr[1][y]->GetNumStr();
		str1 = str1 + "," + ss;
	}
	tinyxml2::XMLElement* blog1 = doc->NewElement("1");
	tinyxml2::XMLText* text1 = doc->NewText(str1.c_str());
	blog1->InsertEndChild(text1);
	doc->InsertEndChild(blog1);

	str1 = "";
	for (int y = 0; y < row_; y++)
	{
		std::string ss = cardDataArr[2][y]->GetNumStr();
		str1 = str1 + "," + ss;
	}
	tinyxml2::XMLElement* blog2 = doc->NewElement("2");
	tinyxml2::XMLText* text2 = doc->NewText(str1.c_str());
	blog2->InsertEndChild(text2);
	doc->InsertEndChild(blog2);

	str1 = "";
	for (int y = 0; y < row_; y++)
	{
		std::string ss = cardDataArr[3][y]->GetNumStr();
		str1 = str1 + "," + ss;
	}
	tinyxml2::XMLElement* blog3 = doc->NewElement("3");
	tinyxml2::XMLText* text3 = doc->NewText(str1.c_str());
	blog3->InsertEndChild(text3);
	doc->InsertEndChild(blog3);

	std::string testpath = FileUtils::getInstance()->fullPathForFilename("test.xml");
	doc->SaveFile(testpath.c_str());

}

// 使用字符串分割
void Stringsplit(const std::string& str, const std::string& splits, vector<std::string>& res)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + splits;
	size_t pos = strs.find(splits);
	int step = splits.size();

	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + step, strs.size());
		pos = strs.find(splits);
	}
}

//从xml文件读取上次保存的局面
void Game::ReadXml()
{
	auto doc = new tinyxml2::XMLDocument();

	std::string testpath = FileUtils::getInstance()->fullPathForFilename("test.xml");
	doc->Parse(FileUtils::getInstance()->getStringFromFile(testpath).c_str());

	tinyxml2::XMLElement* blog0 = doc->FirstChildElement("0");
	auto plk0 = blog0->GetText();
	std::string sp0(plk0);
	vector<string> strLi0;
	Stringsplit(sp0, ",", strLi0);
	for (int i = 0; i < 4; i++)
	{
		cardDataArr[0][i]->setNumber(stoi(strLi0[i + 1]));
	}

	tinyxml2::XMLElement* blog1 = doc->FirstChildElement("1");
	auto plk1 = blog1->GetText();
	std::string sp1(plk1);
	vector<string> strLi1;
	Stringsplit(sp1, ",", strLi1);
	for (int i = 0; i < 4; i++)
	{
		cardDataArr[1][i]->setNumber(stoi(strLi1[i + 1]));
	}

	tinyxml2::XMLElement* blog2 = doc->FirstChildElement("2");
	auto plk2 = blog2->GetText();
	std::string sp2(plk2);
	vector<string> strLi2;
	Stringsplit(sp2, ",", strLi2);
	for (int i = 0; i < 4; i++)
	{
		cardDataArr[2][i]->setNumber(stoi(strLi2[i + 1]));
	}

	tinyxml2::XMLElement* blog3 = doc->FirstChildElement("3");
	auto plk3 = blog3->GetText();
	std::string sp3(plk3);
	vector<string> strLi3;
	Stringsplit(sp3, ",", strLi3);
	for (int i = 0; i < 4; i++)
	{
		cardDataArr[3][i]->setNumber(stoi(strLi3[i + 1]));
	}
}

Scene* Game::createWithWRowCol(const int row, const int col)
{
	auto pRet = new(std::nothrow) Game(row, col);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

// 游戏初始化
bool Game::init()
{
	if (!Scene::init())
	{
		return false;
	}

	const auto director = Director::getInstance();
	const auto visibleSize = director->getVisibleSize();
	const Vec2 origin = director->getVisibleOrigin();

	//弹窗
	{
		mmb = PopUp::create();
		addChild(mmb, 2);
	}

	//加入游戏背景
	{
		auto layerClourBG = LayerColor::create(Color4B(38, 180, 69, 255));
		this->addChild(layerClourBG);
	}

	//加入游戏标题
	auto title = Label::createWithTTF("2048GAME", "fonts/arial.ttf", 60);
	title->setColor(Color3B(255, 255, 153));
	title->setPosition(Point(visibleSize.width / 2, visibleSize.height - 50));
	addChild(title);

	//加入保存按钮
	saveBtn = LabelTTF::create("保存", "Consola", 20);
	saveBtn->setColor(Color3B(204, 255, 253));
	saveBtn->setPosition(Point(visibleSize.width / 5, visibleSize.height - 110));
	addChild(saveBtn);

	//加入载入按钮
	loadBtn = LabelTTF::create("加载", "Consola", 20);
	loadBtn->setPosition(Point(visibleSize.width / 5 * 2, visibleSize.height - 110));
	loadBtn->setColor(Color3B(204, 255, 253));
	addChild(loadBtn);

	//加入排行按钮
	phbBtn = LabelTTF::create("排行榜", "Consola", 20);
	phbBtn->setColor(Color3B(204, 255, 253));
	phbBtn->setPosition(Point(visibleSize.width / 5 * 3, visibleSize.height - 110));
	addChild(phbBtn);

	//加入重新开始按钮
	restartBtn = LabelTTF::create("重新开始", "Consola", 20);
	restartBtn->setColor(Color3B(204, 255, 253));
	restartBtn->setPosition(Point(visibleSize.width / 5 * 4, visibleSize.height - 110));
	addChild(restartBtn);

	//加入游戏分数
	auto slabel = LabelTTF::create("分数：", "Consola", 30);
	slabel->setPosition(Point(visibleSize.width / 2, visibleSize.height - 150));
	addChild(slabel);

	//实时分数
	score = 0;
	scoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 30);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + 80, visibleSize.height - 150));
	addChild(scoreLabel);

	//创建鼠标监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建键盘监听
	EventListenerKeyboard* listenerKeyboard = cocos2d::EventListenerKeyboard::create();
	listenerKeyboard->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);

	//调用卡片生成方法
	createCard(visibleSize);

	//游戏开始生成2个数
	autoCreateCard();
	autoCreateCard();

	return true;
}

//创建键盘监听，键盘方向键进行游戏
void Game::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW: {//方向上键
		if (tryMoveUp())//向上移动
		{
			autoCreateCard();//移动后产生新数字
			playmusic(musictype::move);//移动时有背景音乐
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW: {//方向左键
		if (tryMoveLeft())//向左移动
		{
			autoCreateCard();
			playmusic(musictype::move);
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW: {//方向下键
		if (tryMoveDown())//向下移动
		{
			autoCreateCard();
			playmusic(musictype::move);
		}
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: {//方向右键
		if (tryMoveRight())//向右移动
		{
			autoCreateCard();
			playmusic(musictype::move);
		}
		break;
	}
	}
	//判断游戏是否结束
	checkGameOver();
}

////创建鼠标监听
bool Game::onTouchBegan(Touch* t, Event* e)
{
	//记录开始点击的坐标
	auto startPoint = t->getLocation();
	startX = startPoint.x;
	startY = startPoint.y;

	if (restartBtn->getBoundingBox().containsPoint(startPoint))//点击 重新开始
	{
		restart();//重新开始
	}
	if (saveBtn->getBoundingBox().containsPoint(startPoint))//点击 保存
	{
		WriteXml();//将当前局面保存到xml
	}
	else if (loadBtn->getBoundingBox().containsPoint(startPoint))//点击 载入
	{
		ReadXml();//从xml文件读取上次保存的局面
	}
	else if (phbBtn->getBoundingBox().containsPoint(startPoint))//点击 排行
	{
		ShowRecord();//从xml文件读取并显示排行
	}
	return true;
}
void Game::onTouchEnded(Touch* t, Event* e){}

//向左移动
bool Game::tryMoveLeft()
{
	bool moved = false;
	//对每一行进行遍历
	for (int y = 0; y < row_; y++)
	{   //判断有4个相同数字可以合并，合并得分为单个数字8倍
		if (cardDataArr[0][y]->getNumber() > 0 &&
			cardDataArr[0][y]->getNumber() == cardDataArr[1][y]->getNumber() &&
			cardDataArr[1][y]->getNumber() == cardDataArr[2][y]->getNumber() &&
			cardDataArr[2][y]->getNumber() == cardDataArr[3][y]->getNumber()) {
			cardDataArr[0][y]->setNumber(cardDataArr[0][y]->getNumber() * 8);
			cardDataArr[1][y]->setNumber(0);
			cardDataArr[2][y]->setNumber(0);
			cardDataArr[3][y]->setNumber(0);
			score += (cardDataArr[0][y]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}//判断有3个相同数字可以合并，合并得分为单个数字4倍，形如[2][2][2][4]
		else if (cardDataArr[0][y]->getNumber() > 0 &&
			cardDataArr[0][y]->getNumber() == cardDataArr[1][y]->getNumber() &&
			cardDataArr[1][y]->getNumber() == cardDataArr[2][y]->getNumber()) {
			if (cardDataArr[3][y]->getNumber() > 0) {
				cardDataArr[0][y]->setNumber(cardDataArr[0][y]->getNumber() * 4);
				cardDataArr[1][y]->setNumber(cardDataArr[3][y]->getNumber());
				cardDataArr[2][y]->setNumber(0);
				cardDataArr[3][y]->setNumber(0);
				score += (cardDataArr[0][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[3][y]->getNumber() == 0) {
				cardDataArr[0][y]->setNumber(cardDataArr[0][y]->getNumber() * 4);
				cardDataArr[1][y]->setNumber(0);
				cardDataArr[2][y]->setNumber(0);
				cardDataArr[3][y]->setNumber(0);
				score += (cardDataArr[0][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}//判断有3个相同数字可以合并，合并得分为单个数字4倍，形如[4][2][2][2]
		else if (cardDataArr[1][y]->getNumber() > 0 &&
			cardDataArr[1][y]->getNumber() == cardDataArr[2][y]->getNumber() &&
			cardDataArr[2][y]->getNumber() == cardDataArr[3][y]->getNumber()) {
			if (cardDataArr[0][y]->getNumber() > 0) {
				cardDataArr[1][y]->setNumber(cardDataArr[1][y]->getNumber() * 4);
				cardDataArr[2][y]->setNumber(0);
				cardDataArr[3][y]->setNumber(0);
				score += (cardDataArr[1][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[0][y]->getNumber() == 0) {
				cardDataArr[0][y]->setNumber(cardDataArr[1][y]->getNumber() * 4);
				cardDataArr[1][y]->setNumber(0);
				cardDataArr[2][y]->setNumber(0);
				cardDataArr[3][y]->setNumber(0);
				score += (cardDataArr[0][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}//判断有3个相同数字可以合并，合并得分为单个数字4倍，形如[2][2][2][0]及[0][2][2][2]
		else if ((cardDataArr[0][y]->getNumber()  > 0 &&
			cardDataArr[0][y]->getNumber() == cardDataArr[1][y]->getNumber() &&
			cardDataArr[1][y]->getNumber() == cardDataArr[3][y]->getNumber() &&
			cardDataArr[2][y]->getNumber() == 0) || (
				cardDataArr[0][y]->getNumber() > 0 &&
				cardDataArr[0][y]->getNumber() == cardDataArr[2][y]->getNumber() &&
				cardDataArr[2][y]->getNumber() == cardDataArr[3][y]->getNumber() &&
				cardDataArr[1][y]->getNumber() == 0)) {
			cardDataArr[0][y]->setNumber(cardDataArr[0][y]->getNumber() * 4);
			cardDataArr[1][y]->setNumber(0);
			cardDataArr[2][y]->setNumber(0);
			cardDataArr[3][y]->setNumber(0);
			score += (cardDataArr[0][y]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}//判断有2个相同数字可以合并，合并得分为单个数字2倍
		else {
			//遍历一遍，所有数字向左紧密排列，除去数字的间隔
			for (int i = 0; i <= 2; i++)
			{
				for (int j = 0; j <= 2 - i; j++)
				{
					if (cardDataArr[j][y]->getNumber() == 0 &&
						cardDataArr[j + 1][y]->getNumber()  > 0)
					{
						cardDataArr[j][y]->setNumber(cardDataArr[j + 1][y]->getNumber());
						cardDataArr[j + 1][y]->setNumber(0);
						moved = true;
					}
				}
			}
			//合并2个数字相同数字，合并后左移，不会出现多次合并的错误，例如[2][2][4][0]->[4][0][4][0]不会出现[8][0][0][0]
			for (int i = 0; i <= 2; i++)
			{
				if (cardDataArr[i][y]->getNumber() == cardDataArr[i + 1][y]->getNumber() &&
					cardDataArr[i][y]->getNumber() > 0)
				{
					cardDataArr[i][y]->setNumber(cardDataArr[i][y]->getNumber() * 2);
					cardDataArr[i + 1][y]->setNumber(0);
					//合并时同时加分
					score += (cardDataArr[i][y]->getNumber());
					scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
					moved = true;
				}
			}
			//遍历一遍，所有数字向左紧密排列，除去数字的间隔
			for (int i = 0; i <= 2; i++)
			{
				for (int j = 0; j <= 2 - i; j++)
				{
					if (cardDataArr[j][y]->getNumber() == 0 &&
						cardDataArr[j + 1][y]->getNumber() > 0)
					{
						cardDataArr[j][y]->setNumber(cardDataArr[j + 1][y]->getNumber());
						cardDataArr[j + 1][y]->setNumber(0);
						moved = true;
					}
				}
			}
		}
	}
	return moved;
}

//向右移动，与向左移动逻辑相同
bool Game::tryMoveRight()
{
	bool moved = false;
	for (int y = 0; y < row_; y++)
	{   //4in1
		if (cardDataArr[0][y]->getNumber() > 0 &&
			cardDataArr[0][y]->getNumber() == cardDataArr[1][y]->getNumber() &&
			cardDataArr[1][y]->getNumber() == cardDataArr[2][y]->getNumber() &&
			cardDataArr[2][y]->getNumber() == cardDataArr[3][y]->getNumber()) {
			cardDataArr[3][y]->setNumber(cardDataArr[3][y]->getNumber() * 8);
			cardDataArr[0][y]->setNumber(0);
			cardDataArr[1][y]->setNumber(0);
			cardDataArr[2][y]->setNumber(0);
			score += (cardDataArr[3][y]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}
		else if (cardDataArr[3][y]->getNumber() > 0 &&
			cardDataArr[3][y]->getNumber() == cardDataArr[2][y]->getNumber() &&
			cardDataArr[2][y]->getNumber() == cardDataArr[1][y]->getNumber()) {
			if (cardDataArr[0][y]->getNumber() > 0) {
				cardDataArr[3][y]->setNumber(cardDataArr[3][y]->getNumber() * 4);
				cardDataArr[2][y]->setNumber(cardDataArr[0][y]->getNumber());
				cardDataArr[1][y]->setNumber(0);
				cardDataArr[0][y]->setNumber(0);
				score += (cardDataArr[3][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[0][y]->getNumber() == 0) {
				cardDataArr[3][y]->setNumber(cardDataArr[3][y]->getNumber() * 4);
				cardDataArr[2][y]->setNumber(0);
				cardDataArr[1][y]->setNumber(0);
				cardDataArr[0][y]->setNumber(0);
				score += (cardDataArr[3][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}
		else if (cardDataArr[2][y]->getNumber() > 0 &&
			cardDataArr[2][y]->getNumber() == cardDataArr[1][y]->getNumber() &&
			cardDataArr[1][y]->getNumber() == cardDataArr[0][y]->getNumber()) {
			if (cardDataArr[3][y]->getNumber() > 0) {
				cardDataArr[2][y]->setNumber(cardDataArr[2][y]->getNumber() * 4);
				cardDataArr[1][y]->setNumber(0);
				cardDataArr[0][y]->setNumber(0);
				score += (cardDataArr[2][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[3][y]->getNumber() == 0) {
				cardDataArr[3][y]->setNumber(cardDataArr[2][y]->getNumber() * 4);
				cardDataArr[2][y]->setNumber(0);
				cardDataArr[1][y]->setNumber(0);
				cardDataArr[0][y]->setNumber(0);
				score += (cardDataArr[3][y]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}
		else if ((cardDataArr[0][y]->getNumber() > 0 &&
			cardDataArr[0][y]->getNumber() == cardDataArr[1][y]->getNumber() &&
			cardDataArr[1][y]->getNumber() == cardDataArr[3][y]->getNumber() &&
			cardDataArr[2][y]->getNumber() == 0) || (
				cardDataArr[0][y]->getNumber() > 0 &&
				cardDataArr[0][y]->getNumber() == cardDataArr[2][y]->getNumber() &&
				cardDataArr[2][y]->getNumber() == cardDataArr[3][y]->getNumber() &&
				cardDataArr[1][y]->getNumber() == 0)) {
			cardDataArr[3][y]->setNumber(cardDataArr[3][y]->getNumber() * 4);
			cardDataArr[0][y]->setNumber(0);
			cardDataArr[1][y]->setNumber(0);
			cardDataArr[2][y]->setNumber(0);
			score += (cardDataArr[3][y]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}
		else {
			for (int i = 2; i >= 0; i--)
			{
				for (int j = 2; j >= 2 - i; j--)
				{
					if (cardDataArr[j + 1][y]->getNumber() == 0 &&
						cardDataArr[j][y]->getNumber() > 0)
					{
						cardDataArr[j + 1][y]->setNumber(cardDataArr[j][y]->getNumber());
						cardDataArr[j][y]->setNumber(0);
						moved = true;
					}
				}
			}
			for (int i = 2; i >= 0; i--)
			{
				if (cardDataArr[i][y]->getNumber() == cardDataArr[i + 1][y]->getNumber() &&
					cardDataArr[i][y]->getNumber() > 0)
				{
					cardDataArr[i + 1][y]->setNumber(cardDataArr[i + 1][y]->getNumber() * 2);
					cardDataArr[i][y]->setNumber(0);
					//合并时同时加分
					score += (cardDataArr[i + 1][y]->getNumber());
					scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
					moved = true;
				}
			}
			for (int i = 2; i >= 0; i--)
			{
				for (int j = 2; j >= 2 - i; j--)
				{
					if (cardDataArr[j + 1][y]->getNumber() == 0 &&
						cardDataArr[j][y]->getNumber() > 0)
					{
						cardDataArr[j + 1][y]->setNumber(cardDataArr[j][y]->getNumber());
						cardDataArr[j][y]->setNumber(0);
						moved = true;
					}
				}
			}
		}
	}
	return moved;
}

//向下移动，与向左移动逻辑相同
bool Game::tryMoveDown()
{
	bool moved = false;
	for (int x = 0; x < col_; x++)
	{   //4in1
		if (cardDataArr[x][0]->getNumber() > 0 &&
			cardDataArr[x][0]->getNumber() == cardDataArr[x][1]->getNumber() &&
			cardDataArr[x][1]->getNumber() == cardDataArr[x][2]->getNumber() &&
			cardDataArr[x][2]->getNumber() == cardDataArr[x][3]->getNumber()) {
			cardDataArr[x][0]->setNumber(cardDataArr[x][0]->getNumber() * 8);
			cardDataArr[x][1]->setNumber(0);
			cardDataArr[x][2]->setNumber(0);
			cardDataArr[x][3]->setNumber(0);
			score += (cardDataArr[x][0]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}
		else if (cardDataArr[x][0]->getNumber() > 0 &&
			cardDataArr[x][0]->getNumber() == cardDataArr[x][1]->getNumber() &&
			cardDataArr[x][1]->getNumber() == cardDataArr[x][2]->getNumber()) {
			if (cardDataArr[x][3]->getNumber() > 0) {
				cardDataArr[x][0]->setNumber(cardDataArr[x][0]->getNumber() * 4);
				cardDataArr[x][1]->setNumber(cardDataArr[x][3]->getNumber());
				cardDataArr[x][2]->setNumber(0);
				cardDataArr[x][3]->setNumber(0);
				score += (cardDataArr[x][0]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[x][3]->getNumber() == 0) {
				cardDataArr[x][0]->setNumber(cardDataArr[x][0]->getNumber() * 4);
				cardDataArr[x][1]->setNumber(0);
				cardDataArr[x][2]->setNumber(0);
				cardDataArr[x][3]->setNumber(0);
				score += (cardDataArr[x][0]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}
		else if (cardDataArr[x][1]->getNumber() > 0 &&
			cardDataArr[x][1]->getNumber() == cardDataArr[x][2]->getNumber() &&
			cardDataArr[x][2]->getNumber() == cardDataArr[x][3]->getNumber()) {
			if (cardDataArr[x][0]->getNumber() > 0) {
				cardDataArr[x][1]->setNumber(cardDataArr[x][1]->getNumber() * 4);
				cardDataArr[x][2]->setNumber(0);
				cardDataArr[x][3]->setNumber(0);
				score += (cardDataArr[x][1]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[x][0]->getNumber() == 0) {
				cardDataArr[x][0]->setNumber(cardDataArr[x][1]->getNumber() * 4);
				cardDataArr[x][1]->setNumber(0);
				cardDataArr[x][2]->setNumber(0);
				cardDataArr[x][3]->setNumber(0);
				score += (cardDataArr[x][0]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}
		else if ((cardDataArr[x][0]->getNumber() > 0 &&
			cardDataArr[x][0]->getNumber() == cardDataArr[x][1]->getNumber() &&
			cardDataArr[x][1]->getNumber() == cardDataArr[x][3]->getNumber() &&
			cardDataArr[x][2]->getNumber() == 0) || (
				cardDataArr[x][0]->getNumber() > 0 &&
				cardDataArr[x][0]->getNumber() == cardDataArr[x][2]->getNumber() &&
				cardDataArr[x][2]->getNumber() == cardDataArr[x][3]->getNumber() &&
				cardDataArr[x][1]->getNumber() == 0)) {
			cardDataArr[x][0]->setNumber(cardDataArr[x][0]->getNumber() * 4);
			cardDataArr[x][1]->setNumber(0);
			cardDataArr[x][2]->setNumber(0);
			cardDataArr[x][3]->setNumber(0);
			score += (cardDataArr[x][0]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}
		else {
			for (int i = 0; i <= 2; i++)
			{
				for (int j = 0; j <= 2 - i; j++)
				{
					if (cardDataArr[x][j]->getNumber() == 0 &&
						cardDataArr[x][j + 1]->getNumber() > 0)
					{
						cardDataArr[x][j]->setNumber(cardDataArr[x][j + 1]->getNumber());
						cardDataArr[x][j + 1]->setNumber(0);
						moved = true;
					}
				}
			}
			for (int i = 0; i <= 2; i++)
			{
				if (cardDataArr[x][i]->getNumber() == cardDataArr[x][i + 1]->getNumber() &&
					cardDataArr[x][i]->getNumber() > 0)
				{
					cardDataArr[x][i]->setNumber(cardDataArr[x][i]->getNumber() * 2);
					cardDataArr[x][i + 1]->setNumber(0);
					//合并时同时加分
					score += (cardDataArr[x][i]->getNumber());
					scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
					moved = true;
				}
			}
			for (int i = 0; i <= 2; i++)
			{
				for (int j = 0; j <= 2 - i; j++)
				{
					if (cardDataArr[x][j]->getNumber() == 0 &&
						cardDataArr[x][j + 1]->getNumber() > 0)
					{
						cardDataArr[x][j]->setNumber(cardDataArr[x][j + 1]->getNumber());
						cardDataArr[x][j + 1]->setNumber(0);
						moved = true;
					}
				}
			}
		}
	}
	return moved;
}

//向上移动，与向左移动逻辑相同
bool Game::tryMoveUp()
{
	bool moved = false;

	for (int x = 0; x < col_; x++)
	{   //4in1
		if (cardDataArr[x][0]->getNumber() > 0 &&
			cardDataArr[x][3]->getNumber() == cardDataArr[x][2]->getNumber() &&
			cardDataArr[x][2]->getNumber() == cardDataArr[x][1]->getNumber() &&
			cardDataArr[x][1]->getNumber() == cardDataArr[x][0]->getNumber()) {
			cardDataArr[x][3]->setNumber(cardDataArr[x][3]->getNumber() * 8);
			cardDataArr[x][2]->setNumber(0);
			cardDataArr[x][1]->setNumber(0);
			cardDataArr[x][0]->setNumber(0);
			score += (cardDataArr[x][3]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}
		else if (cardDataArr[x][3]->getNumber() > 0 &&
			cardDataArr[x][3]->getNumber() == cardDataArr[x][2]->getNumber() &&
			cardDataArr[x][2]->getNumber() == cardDataArr[x][1]->getNumber()) {
			if (cardDataArr[x][0]->getNumber() > 0) {
				cardDataArr[x][3]->setNumber(cardDataArr[x][3]->getNumber() * 4);
				cardDataArr[x][2]->setNumber(cardDataArr[x][0]->getNumber());
				cardDataArr[x][1]->setNumber(0);
				cardDataArr[x][0]->setNumber(0);
				score += (cardDataArr[x][3]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[x][0]->getNumber() == 0) {
				cardDataArr[x][3]->setNumber(cardDataArr[x][3]->getNumber() * 4);
				cardDataArr[x][2]->setNumber(0);
				cardDataArr[x][1]->setNumber(0);
				cardDataArr[x][0]->setNumber(0);
				score += (cardDataArr[x][3]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}
		else if (cardDataArr[x][2]->getNumber() > 0 &&
			cardDataArr[x][2]->getNumber() == cardDataArr[x][1]->getNumber() &&
			cardDataArr[x][1]->getNumber() == cardDataArr[x][0]->getNumber()) {
			if (cardDataArr[x][3]->getNumber() > 0) {
				cardDataArr[x][2]->setNumber(cardDataArr[x][2]->getNumber() * 4);
				cardDataArr[x][1]->setNumber(0);
				cardDataArr[x][0]->setNumber(0);
				score += (cardDataArr[x][2]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
			else if (cardDataArr[x][3]->getNumber() == 0) {
				cardDataArr[x][3]->setNumber(cardDataArr[x][2]->getNumber() * 4);
				cardDataArr[x][2]->setNumber(0);
				cardDataArr[x][1]->setNumber(0);
				cardDataArr[x][0]->setNumber(0);
				score += (cardDataArr[x][3]->getNumber());
				scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
				moved = true;
			}
		}
		else if ((cardDataArr[x][3]->getNumber() > 0 &&
			cardDataArr[x][3]->getNumber() == cardDataArr[x][2]->getNumber() &&
			cardDataArr[x][2]->getNumber() == cardDataArr[x][0]->getNumber() &&
			cardDataArr[x][1]->getNumber() == 0) || (
				cardDataArr[x][3]->getNumber() > 0 &&
				cardDataArr[x][3]->getNumber() == cardDataArr[x][1]->getNumber() &&
				cardDataArr[x][1]->getNumber() == cardDataArr[x][0]->getNumber() &&
				cardDataArr[x][2]->getNumber() == 0)) {
			cardDataArr[x][3]->setNumber(cardDataArr[x][3]->getNumber() * 4);
			cardDataArr[x][2]->setNumber(0);
			cardDataArr[x][1]->setNumber(0);
			cardDataArr[x][0]->setNumber(0);
			score += (cardDataArr[x][3]->getNumber());
			scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
			moved = true;
		}
		else {
			for (int i = 2; i >= 0; i--)
			{
				for (int j = 2; j >= 2 - i; j--)
				{
					if (cardDataArr[x][j + 1]->getNumber() == 0 &&
						cardDataArr[x][j]->getNumber() > 0)
					{
						cardDataArr[x][j + 1]->setNumber(cardDataArr[x][j]->getNumber());
						cardDataArr[x][j]->setNumber(0);
						moved = true;
					}
				}
			}
			for (int i = 2; i >= 0; i--)
			{
				if (cardDataArr[x][i]->getNumber() == cardDataArr[x][i + 1]->getNumber() &&
					cardDataArr[x][i]->getNumber() > 0)
				{
					cardDataArr[x][i + 1]->setNumber(cardDataArr[x][i + 1]->getNumber() * 2);
					cardDataArr[x][i]->setNumber(0);
					//合并时同时加分
					score += (cardDataArr[x][i + 1]->getNumber());
					scoreLabel->setString(__String::createWithFormat("%i", score)->getCString());
					moved = true;
				}
			}
			for (int i = 2; i >= 0; i--)
			{
				for (int j = 2; j >= 2 - i; j--)
				{
					if (cardDataArr[x][j + 1]->getNumber() == 0 &&
						cardDataArr[x][j]->getNumber() > 0)
					{
						cardDataArr[x][j + 1]->setNumber(cardDataArr[x][j]->getNumber());
						cardDataArr[x][j]->setNumber(0);
						moved = true;
					}
				}
			}
		}
	}
	return moved;
}

//生成4X4的游戏卡片
void Game::createCard(Size size)
{
	cardDataArr.reserve(col_);
	cardDataArr.resize(col_);
	std::for_each(cardDataArr.begin(), cardDataArr.end(), [=](decltype(*cardDataArr.begin())& c)
		{
			c.reserve(row_);
			c.resize(row_);
		});
	//求出单元格的宽高
	auto cardheight = (size.width - 20) / row_;
	auto cardwidth = (size.width - 20) / col_;

	//4*4单元格
	for (int i = 0; i < col_; i++)
	{
		for (int j = 0; j < row_; j++)
		{  //在特定位置生成卡片
			Card* cards = Card::createCard(0, cardwidth, cardheight,
				cardwidth * i + 12, cardheight * j + 12);
			addChild(cards);
			//把卡片存在一个二维数组里面
			cardDataArr[i][j] = cards;
		}
	}
}

//加载背景音乐
using CocosDenshion::SimpleAudioEngine;
void Game::loadmusic()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("music/effect/move.mp3");
}

//游戏时每走一步都有背景音乐
void Game::playmusic(musictype t)
{
	auto audio = SimpleAudioEngine::getInstance();
	switch (t)
	{
	case musictype::move:
		audio->playEffect("music/effect/move.mp3");
		break;
	case musictype::nomove:
		break;
	default:
		break;
	}
}

//生成新数字
void Game::autoCreateCard()
{
	//随机生成横坐标和纵坐标
	int row = 0;
	int col = 0;
	do
	{
		row = CCRANDOM_0_1() * row_;
		col = CCRANDOM_0_1() * col_;
		// 判断位置是否已经存在数字
	} while (cardDataArr[row][col]->getNumber() > 0);

	//生成新数字，新数字2,4,8出现的概率比是16:4:1
	cardDataArr[row][col]->runcreateaction();
	cardDataArr[row][col]->setNumber(CCRANDOM_0_1() * 21 < 16 ? 2 : (CCRANDOM_0_1() * 5 < 1 ? 8 : 4));
}

//游戏结束逻辑判断
void Game::checkGameOver()
{
	bool isGameOver = true;
	int tn = 0;

	//达到或超过上限，胜利
	tn = cardDataArr[0][0]->getNumber();
	if (tn >= 8192)
	{
		showwin();
	}

	do
	{//最下面的行
		for (int x = 1; x < col_; ++x)
		{
			tn = cardDataArr[x][0]->getNumber();
			if (tn >= 8192)
			{
				showwin();
			}
			if (tn == 0 || tn == cardDataArr[x - 1][0]->getNumber())
			{
				isGameOver = false;
			}
		}
		//最左边的列
		for (int y = 1; y < row_; ++y)
		{
			tn = cardDataArr[0][y]->getNumber();
			if (tn >= 8192)
			{
				showwin();
			}
			if (tn == 0 || tn == cardDataArr[0][y - 1]->getNumber())
			{
				isGameOver = false;
			}
		}
		//剩下的遍历
		for (int y = 1; y < row_; y++)
		{
			for (int x = 1; x < col_; x++)
			{
				tn = cardDataArr[x][y]->getNumber();
				if (tn >= 8192)
				{
					showwin();
				}
				if (tn == 0 ||//有空位
					tn == cardDataArr[x - 1][y]->getNumber() || //判断左边是否相同
					tn == cardDataArr[x][y - 1]->getNumber() //判断下面是否相同
					)
				{
					isGameOver = false;//满足以上条件则游戏继续
				}
			}
		}
	} while (false);
	//不能继续游戏，调用弹窗显示失败，并记录成绩
	if (isGameOver)
	{
		mmb->setContent("score and restart?");
		mmb->setTitle("You Lose");
		mmb->setclickcallback(PopUp::CLICKTYPE::YES, [=]()
			{
				Record(score);
				Rank();//记入排行
				restart();//重新开始游戏
			});
		mmb->showdialog();
	}
}

//重新开始游戏
void Game::restart()
{
	int col = 4, row = 4;

	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.4f, createWithWRowCol(row, col)));
}

//调用弹窗，显示游戏胜利，记录成绩
void Game::showwin()
{
	if (!showedwin_)
	{
		mmb->setContent("score and restart?");
		mmb->setTitle("You Win");
		mmb->setclickcallback(PopUp::CLICKTYPE::YES, [=]()
			{
				Record(score);
				Rank();//记录成绩
				restart();//并重新开始
			});
		mmb->showdialog();
	}
}

//记录游戏分数，写入排行榜
void Game::Record(int score)
{
	// 临时存储排行，前五个为历史排行，第六个为待排序新成绩
	int Rank[6] = {};
	//从xml文件读取历史排行
	auto doc = new tinyxml2::XMLDocument();

	std::string rankpath = FileUtils::getInstance()->fullPathForFilename("rank.xml");
	doc->Parse(FileUtils::getInstance()->getStringFromFile(rankpath).c_str());

	tinyxml2::XMLElement* blog0 = doc->FirstChildElement("0");
	auto plk0 = blog0->GetText();
	std::string sp0(plk0);
	vector<string> strLi;
	Stringsplit(sp0, ",", strLi);
	for (int i = 0; i < 6 - 1; i++)
	{
		Records[i] = strLi[i + 1];
	}
	
	Rank[5] = score;
	//类型转换
	for (int i = 0; i < 6 - 1; i++)
	{
		Rank[i] = stoi(Records[i]);
	}
	//历史成绩排序
	int temp;
	for (int i = 0; i < 6 - 1; i++)
	{
		int count = 0;
		for (int j = 0; j < 6 - 1 - i; j++)
		{
			if (Rank[j] < Rank[j + 1])
			{
				temp = Rank[j];
				Rank[j] = Rank[j + 1];
				Rank[j + 1] = temp;
				count = 1;
			}
		}
		if (count == 0)
			break;
	}
	//类型转换
	for (int i = 0; i < 6; i++)
	{
		Records[i] = to_string(Rank[i]);
	}
}

//维护xml文件，删除旧有内容，写入新内容
void Game::Rank()
{
	//存储xml
	auto doc = new tinyxml2::XMLDocument();
	std::string str1 = "";
	for (int i = 0; i < 6; i++)
	{
		str1 = str1 + "," + Records[i];
	}
	tinyxml2::XMLElement* blog = doc->NewElement("0");
	tinyxml2::XMLText* text = doc->NewText(str1.c_str());
	blog->InsertEndChild(text);
	doc->InsertEndChild(blog);

	std::string rankpath = FileUtils::getInstance()->fullPathForFilename("rank.xml");
	doc->SaveFile(rankpath.c_str());
}

//调用弹窗，显示排行榜
void Game::ShowRecord()
{
	//临时存储排行榜
	string Record[5] = {};
	//读取xml文件
	auto doc = new tinyxml2::XMLDocument();

	std::string rankpath = FileUtils::getInstance()->fullPathForFilename("rank.xml");
	doc->Parse(FileUtils::getInstance()->getStringFromFile(rankpath).c_str());

	tinyxml2::XMLElement* blog0 = doc->FirstChildElement("0");
	auto plk0 = blog0->GetText();
	std::string sp0(plk0);
	vector<string> strLi;
	Stringsplit(sp0, ",", strLi);
	for (int i = 0; i < 5; i++)
	{
		Record[i] = strLi[i + 1];
	}
	//生成排行榜显示内容
	std::string str = "";
	str = str + "HIGHEST" + "     " + Record[0] + "\n"
		+ "SECOND" + "     " + Record[1] + "\n"
		+ "THIRD" + "          " + Record[2] + "\n"
		+ "FOURTH" + "      " + Record[3] + "\n"
		+ "FIFTH" + "           " + Record[4];
	mmb->setContent(str);
	mmb->setTitle("RANK");
	mmb->showdialog();
}