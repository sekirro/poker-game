#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include "../models/GameModel.h"
#include <map>
#include <functional>

/**
 * @class GameView
 * @brief 游戏主视图
 * @details 管理整个游戏界面，包括主牌区、底牌堆、备用牌堆和UI按钮
 *          可持有const类型的model指针，通过回调接口与Controller交互
 */
class GameView : public cocos2d::Layer
{
public:
    /**
     * @brief 卡牌点击回调函数类型
     */
    using CardClickCallback = std::function<void(int cardId)>;
    
    /**
     * @brief 备用牌堆点击回调函数类型
     */
    using StackClickCallback = std::function<void()>;
    
    /**
     * @brief 撤销按钮点击回调函数类型
     */
    using UndoButtonClickCallback = std::function<void()>;
    
    /**
     * @brief 创建游戏视图
     * @return 游戏视图指针
     */
    static GameView* create();
    
    /**
     * @brief 初始化
     * @return 是否初始化成功
     */
    virtual bool init() override;
    
    /**
     * @brief 初始化游戏视图（从GameModel）
     * @param gameModel 游戏数据模型
     */
    void initGameView(const GameModel* gameModel);
    
    /**
     * @brief 获取CardView
     * @param cardId 卡牌ID
     * @return CardView指针，如果不存在返回nullptr
     */
    CardView* getCardView(int cardId) const;
    
    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数
     */
    void setCardClickCallback(const CardClickCallback& callback);
    
    /**
     * @brief 设置备用牌堆点击回调
     * @param callback 回调函数
     */
    void setStackClickCallback(const StackClickCallback& callback);
    
    /**
     * @brief 设置撤销按钮点击回调
     * @param callback 回调函数
     */
    void setUndoButtonClickCallback(const UndoButtonClickCallback& callback);
    
    /**
     * @brief 播放卡牌移动动画
     * @param cardId 卡牌ID
     * @param targetPosition 目标位置
     * @param duration 动画时长
     * @param callback 动画完成回调
     */
    void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, 
                               float duration = 0.3f, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放匹配动画
     * @param fromCardId 源卡牌ID
     * @param toCardId 目标卡牌ID
     * @param callback 动画完成回调
     */
    void playMatchAnimation(int fromCardId, int toCardId, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 更新撤销按钮状态
     * @param enabled 是否启用
     */
    void updateUndoButton(bool enabled);
    
    /**
     * @brief 移除卡牌视图
     * @param cardId 卡牌ID
     */
    void removeCardView(int cardId);
    
private:
    /**
     * @brief 创建主牌区
     */
    void createPlayfieldArea();
    
    /**
     * @brief 创建底牌堆区域
     */
    void createTrayArea();
    
    /**
     * @brief 创建备用牌堆区域
     */
    void createStackArea();
    
    /**
     * @brief 创建UI按钮
     */
    void createUIButtons();
    
    /**
     * @brief 获取底牌堆位置
     */
    cocos2d::Vec2 getTrayPosition() const;
    
    /**
     * @brief 获取备用牌堆位置
     */
    cocos2d::Vec2 getStackPosition() const;
    
private:
    std::map<int, CardView*> _cardViews;        // 所有卡牌视图的映射表
    cocos2d::Layer* _playfieldLayer;            // 主牌区层
    cocos2d::Layer* _trayLayer;                 // 底牌堆层
    cocos2d::Layer* _stackLayer;                // 备用牌堆层
    cocos2d::ui::Button* _undoButton;           // 撤销按钮
    cocos2d::Sprite* _stackSprite;              // 备用牌堆精灵（可点击）
    
    CardClickCallback _cardClickCallback;       // 卡牌点击回调
    StackClickCallback _stackClickCallback;     // 备用牌堆点击回调
    UndoButtonClickCallback _undoButtonClickCallback;  // 撤销按钮点击回调
};

#endif // __GAME_VIEW_H__ 