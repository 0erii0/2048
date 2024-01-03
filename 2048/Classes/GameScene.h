#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "vector"
#include "PopUp.h"
#include "ui/UITextField.h"

#define USE_SIMPLE_AUDIO_ENGINE true

class Game : public Scene
{
public:
	Game();
	Game(int row, int col);
    static Scene* createScene();

	//将当前局面写入xml文件
	void WriteXml();
	//从xml文件读取上次保存的局面
	void ReadXml();

	static Scene* createWithWRowCol(int row=4,int col=4);

    virtual bool init();
    CREATE_FUNC(Game);

	//随机生成一个卡片
	void autoCreateCard();

	//判断是否游戏结束
	void checkGameOver();

	//重新开始
	void restart();

	//保存记录
	void Game::Record(int score);
	//维护记录
	void Game::Rank();

	//查看排行
	void Game::ShowRecord();

	//赢了
	inline void showwin();

	//判断是否移动，如果可以移动，就移动
	bool tryMoveLeft();
	bool tryMoveRight();
	bool tryMoveDown();
	bool tryMoveUp();

private:
	PopUp * mmb = nullptr;
	//游戏行列数
	const int row_ ;
	const int col_ ;
	ui::TextField* tfr_ = nullptr;
	ui::TextField* tfc_ = nullptr;

	//记录按下时坐标
	float startX=0.0f, startY= 0.0f;

	//创建鼠标监听
	bool onTouchBegan(Touch *t, Event *e);
	void onTouchEnded(Touch *t, Event *e);

	//创建键盘监听
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	//创建卡片(初始化)
	void createCard(Size size);

	//储存卡片的二维数组，以便控制每一张卡片
	std::vector<std::vector<Card*> > cardDataArr;

	//游戏分数
	int score=0;

	//显示分数的控件
	Label *scoreLabel=nullptr;

	LabelTTF* saveBtn = nullptr;
	LabelTTF* loadBtn = nullptr;
	LabelTTF* phbBtn = nullptr;
	//重新开始游戏按钮
	LabelTTF*restartBtn= nullptr;

	enum class musictype
	{
		move,
		nomove
	};

	void loadmusic();
	void playmusic(musictype t);

	//显示过赢了
	bool showedwin_ = false;
};

#endif // __Game_SCENE_H__
