#include "Popup.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIText.h"

bool PopUp::init()
{
	if (!Layer::init())return false;

	const auto director = Director::getInstance();
	const auto visibleSize = director->getVisibleSize();
	const auto origin = director->getVisibleOrigin();

	//初始化背景层
	{
		bkly = Layer::create();
		addChild(bkly);
		sp_bk_ = ui::Scale9Sprite::create("pic/btn/msgbk.png");
		sp_bk_->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));
		sp_bk_->setContentSize(Size(visibleSize.width / 3 * 2, visibleSize.height / 2));
		bkly->addChild(sp_bk_, 2);
	}

	//文字层
	{
		txtly = Layer::create();
		addChild(txtly);
		content_ = Label::createWithTTF("Content", "fonts/arial.ttf", 30);
		auto cr = getcontentrec();
		content_->setPosition(cr.getMidX(), cr.getMidY());
		txtly->addChild(content_, 2);

		title_ = Label::createWithTTF("Title", "fonts/arial.ttf", 30);
		title_->setPosition(gettitlerec().getMidX(), gettitlerec().getMidY());
		txtly->addChild(title_, 2);
	}

	auto bs = sp_bk_->getContentSize();
	auto bp = sp_bk_->getPosition();

	//初始化按钮层
	{
		btnly = Layer::create();
		addChild(btnly);

		sp_yes_ = ui::Scale9Sprite::create("pic/btn/msgok.png");
		sp_yes_->setPosition(Vec2(bp.x,
			origin.y + bp.y - bs.height / 5 * 2));
		sp_yes_->setContentSize(Size(bs.width / 13 * 3, bs.height / 5));
		auto txt = ui::Text::create("Yes", "fonts/jt.ttf", 100);
		txt->setFontSize(sp_yes_->getContentSize().width * 100 / txt->getAutoRenderSize().width*0.8);
		txt->setPosition(sp_yes_->getContentSize() / 2);
		sp_yes_->addChild(txt);
		btnly->addChild(sp_yes_, 2);
	}

	hide();
	return true;
}

void PopUp::setclickcallback(CLICKTYPE t, mymsgcallback fp)
{
	funcmap_[t] = fp;
}

PopUp::CLICKTYPE PopUp::showdialog()
{
	for (auto& n : getChildren())
	{
		n->setVisible(true);
	}
	startlisten();
	return CLICKTYPE();
}

PopUp::CLICKTYPE PopUp::showdialog(const std::string & content, const std::string & title)
{
	content_->setString(content);
	title_ ->setString(title);
	return showdialog();
}

void PopUp::hide()
{
	for(auto& n:getChildren())
	{
		n->setVisible(false);
	}
}

void PopUp::setTitle(const std::string & str)
{
	title_->setString(str);
}

void PopUp::startlisten()
{
	auto listener1 = EventListenerTouchOneByOne::create();
	//事件可以被吞没
	listener1->setSwallowTouches(true);
	listener1->onTouchBegan = [=](Touch* touch, Event* event)->bool {
		auto p = touch->getLocation();
		if(sp_yes_->getBoundingBox().containsPoint(p))
		{
			startbtn_ = CLICKTYPE::YES;
		}
		return true; 
	};
	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto p = touch->getLocation();
		if (sp_yes_->getBoundingBox().containsPoint(p)&& startbtn_ == CLICKTYPE::YES)
		{
			onclickbtn(startbtn_);
			hide();
			_eventDispatcher->removeEventListener(listener1);
		}
	};

	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, btnly);
}

void PopUp::stoplisten()
{
	_eventDispatcher->removeAllEventListeners();
}

Rect PopUp::gettitlerec()
{
	auto r = sp_bk_->getBoundingBox();
	auto s = 5.0f;
	return Rect(r.getMinX(),
		r.getMaxY()- r.size.height/ s,
		r.size.width,
		r.size.height / s);
}

Rect PopUp::getcontentrec()
{
	auto r = sp_bk_->getBoundingBox();
	auto s = 5.0f;
	r.size.height *= 1.0f - 2.0f / s;
	r.origin.y += r.origin.y*2.0f / s;
	return r;
}

void PopUp::onclickbtn(CLICKTYPE t)
{
	auto key = funcmap_.find(t);
	if (key != funcmap_.end())
	{
		key->second();
	}
}

void PopUp::setContent(const std::string& str)
{
	content_->setString(str);
}
