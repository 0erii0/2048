#pragma once
#include "cocos2d.h"
#include <map>
#include "ui/UIButton.h"
USING_NS_CC;
class PopUp:public Layer
{
public:
	using mymsgcallback = std::function<void()>;
	enum class CLICKTYPE
	{
		YES
	};
	bool init() override;

	//设置回调函数
	/**
	 * \brief 
	 * \param t 点击类型
	 * \param fp 回调函数
	 */
	void setclickcallback(CLICKTYPE t, mymsgcallback fp);

	//显示模态对话框
	/**
	 * \brief 
	 * \return 结果
	 */
	CLICKTYPE showdialog();

	/**
	 * \brief 显示模态对话框
	 * \param content 内容
	 * \param title 标题
	 * \return 结果
	 */
	CLICKTYPE showdialog(const std::string& content,const std::string& title="");

	/**
	 * \brief 隐藏
	 */
	void hide();

	/**
	 * \brief 设置内容文字
	 * \param str 内容
	 */
	void setContent(const std::string& str);

	/**
	* \brief 设置标题文字
	* \param str 标题
	*/
	void setTitle(const std::string& str);

	CREATE_FUNC(PopUp)
private:
	void startlisten();
	void stoplisten();
	//获取标题区域
	Rect gettitlerec();
	//获取内容区域
	Rect getcontentrec();
	//点击按钮
	void onclickbtn(CLICKTYPE t);
	std::map<CLICKTYPE, mymsgcallback> funcmap_;

	//背景层
	Layer* bkly = nullptr;
	//按钮层
	Layer* btnly = nullptr;
	//文字层
	Layer* txtly = nullptr;
	////确定按钮
	//ui::Button* btnyes = nullptr;
	////取消按钮
	//ui::Button* btncancel = nullptr;
	////否定按钮
	//ui::Button* btnno = nullptr;
	//背景
	Sprite *sp_bk_ = nullptr;
	Sprite *sp_yes_ = nullptr;
	Label* content_ = nullptr;
	Label* title_ = nullptr;
	//触摸开始的按钮
	CLICKTYPE startbtn_= CLICKTYPE::YES;
};