#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

// 设计分辨率常量（根据需求文档）
static const int kDesignWidth = 1080;
static const int kDesignHeight = 2080;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    // 设置OpenGL上下文属性
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化Director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if(!glview) {
        // 创建窗口（根据需求文档：1080*2080，缩放0.5）
        glview = GLViewImpl::createWithRect("Poker Game", 
                                           cocos2d::Rect(0, 0, kDesignWidth, kDesignHeight), 
                                           0.5f);
        director->setOpenGLView(glview);
    }

    // 设置设计分辨率（固定宽度策略）
    glview->setDesignResolutionSize(kDesignWidth, kDesignHeight, ResolutionPolicy::FIXED_WIDTH);

    // 显示FPS
    director->setDisplayStats(true);

    // 设置FPS为60
    director->setAnimationInterval(1.0f / 60);

    // 创建并运行第一个场景
    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    // 如果有音频，在这里暂停
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    // 如果有音频，在这里恢复
} 