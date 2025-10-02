#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "controllers/GameController.h"

/**
 * @class HelloWorld
 * @brief 游戏主场景
 * @details 游戏的主场景，负责创建和管理GameController
 */
class HelloWorld : public cocos2d::Scene
{
public:
    /**
     * @brief 创建场景
     * @return 场景指针
     */
    static cocos2d::Scene* createScene();

    /**
     * @brief 初始化场景
     * @return 如果初始化成功返回true
     */
    virtual bool init();
    
    /**
     * @brief 菜单关闭回调
     * @param pSender 发送者
     */
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // 实现create方法
    CREATE_FUNC(HelloWorld);

private:
    GameController* _gameController;  // 游戏控制器
};

#endif // __HELLOWORLD_SCENE_H__ 