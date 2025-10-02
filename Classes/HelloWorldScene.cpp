#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    // 调用父类的init方法
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 添加标题标签
    auto label = Label::createWithTTF("Poker Card Game", "fonts/Marker Felt.ttf", 48);
    if (label)
    {
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));
        this->addChild(label, 1);
    }

    // 创建游戏控制器并开始游戏
    _gameController = new GameController();
    _gameController->retain();
    
    // 启动关卡1
    if (!_gameController->startGame(1, this)) {
        CCLOG("Failed to start game!");
    }

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    // 关闭应用程序
    Director::getInstance()->end();
} 