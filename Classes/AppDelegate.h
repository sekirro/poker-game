#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
 * @class AppDelegate
 * @brief Cocos2d-x应用程序委托
 * @details 应用程序的入口点，负责初始化和配置Cocos2d-x引擎
 */
class AppDelegate : private cocos2d::Application
{
public:
    /**
     * @brief 构造函数
     */
    AppDelegate();
    
    /**
     * @brief 析构函数
     */
    virtual ~AppDelegate();

    /**
     * @brief 初始化OpenGL上下文属性
     */
    virtual void initGLContextAttrs();

    /**
     * @brief 应用程序启动时调用
     * @return 如果初始化成功返回true
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * @brief 应用程序进入后台时调用
     */
    virtual void applicationDidEnterBackground();

    /**
     * @brief 应用程序进入前台时调用
     */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_ 