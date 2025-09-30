#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../configs/models/CardResConfig.h"
#include <functional>

/**
 * @class CardView
 * @brief 卡牌视图
 * @details UI视图层，负责单张卡牌的显示和交互
 *          可持有const类型的model指针，通过回调接口与Controller交互
 */
class CardView : public cocos2d::Sprite
{
public:
    /**
     * @brief 卡牌点击回调函数类型
     */
    using CardClickCallback = std::function<void(int cardId)>;
    
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌数据模型（const指针）
     * @return 卡牌视图指针
     */
    static CardView* create(const CardModel* cardModel);
    
    /**
     * @brief 初始化
     * @param cardModel 卡牌数据模型（const指针）
     * @return 是否初始化成功
     */
    bool init(const CardModel* cardModel);
    
    /**
     * @brief 获取卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 更新卡牌显示（根据model的状态）
     * @param cardModel 卡牌数据模型
     */
    void updateDisplay(const CardModel* cardModel);
    
    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数
     */
    void setClickCallback(const CardClickCallback& callback);
    
    /**
     * @brief 执行移动到目标位置的动画
     * @param targetPosition 目标位置
     * @param duration 动画时长（秒）
     * @param callback 动画完成回调
     */
    void moveToPosition(const cocos2d::Vec2& targetPosition, float duration, 
                       const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 翻牌动画
     * @param showFront 是否显示正面
     * @param duration 动画时长（秒）
     */
    void flipCard(bool showFront, float duration = 0.3f);
    
    /**
     * @brief 设置是否可点击
     * @param clickable 是否可点击
     */
    void setClickable(bool clickable);
    
private:
    /**
     * @brief 构造函数
     */
    CardView();
    
    /**
     * @brief 析构函数
     */
    virtual ~CardView();
    
    /**
     * @brief 初始化触摸事件监听器
     */
    void initTouchListener();
    
    /**
     * @brief 更新卡牌纹理
     * @param showFront 是否显示正面
     */
    void updateTexture(bool showFront);
    
private:
    int _cardId;                                    // 卡牌ID
    CardFaceType _face;                             // 牌面类型
    CardSuitType _suit;                             // 花色类型
    bool _isFlipped;                                // 是否翻开
    bool _isClickable;                              // 是否可点击
    CardClickCallback _clickCallback;               // 点击回调
    cocos2d::EventListenerTouchOneByOne* _touchListener;  // 触摸监听器
};

#endif // __CARD_VIEW_H__ 