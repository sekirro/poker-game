#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "../utils/CardDefines.h"
#include "json/document.h"

/**
 * @class CardModel
 * @brief 卡牌数据模型
 * @details 存储单张卡牌的运行时数据，包括牌面、花色、位置等信息
 *          支持序列化和反序列化，用于存档功能
 */
class CardModel
{
public:
    /**
     * @brief 构造函数
     * @param face 牌面类型
     * @param suit 花色类型
     * @param cardId 卡牌唯一ID
     */
    CardModel(CardFaceType face = CFT_NONE, CardSuitType suit = CST_NONE, int cardId = -1);
    
    /**
     * @brief 析构函数
     */
    ~CardModel();
    
    // Getters
    int getCardId() const { return _cardId; }
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
    CardLocation getLocation() const { return _location; }
    const cocos2d::Vec2& getPosition() const { return _position; }
    bool isFlipped() const { return _isFlipped; }
    bool isClickable() const { return _isClickable; }
    
    // Setters
    void setCardId(int cardId) { _cardId = cardId; }
    void setFace(CardFaceType face) { _face = face; }
    void setSuit(CardSuitType suit) { _suit = suit; }
    void setLocation(CardLocation location) { _location = location; }
    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    void setFlipped(bool flipped) { _isFlipped = flipped; }
    void setClickable(bool clickable) { _isClickable = clickable; }
    
    /**
     * @brief 检查当前卡牌是否可以与目标卡牌匹配
     * @param otherCard 目标卡牌
     * @return 如果可以匹配返回true，否则返回false
     * @details 匹配规则：牌面数值差为1，无花色限制
     */
    bool canMatchWith(const CardModel* otherCard) const;
    
    /**
     * @brief 获取牌面的数值（A=1, J=11, Q=12, K=13）
     * @return 牌面数值
     */
    int getFaceValue() const;
    
    /**
     * @brief 序列化为JSON
     * @return JSON对象
     */
    rapidjson::Value serialize(rapidjson::Document::AllocatorType& allocator) const;
    
    /**
     * @brief 从JSON反序列化
     * @param json JSON对象
     */
    void deserialize(const rapidjson::Value& json);
    
private:
    int _cardId;                    // 卡牌唯一ID
    CardFaceType _face;             // 牌面类型
    CardSuitType _suit;             // 花色类型
    CardLocation _location;         // 当前位置
    cocos2d::Vec2 _position;        // 屏幕坐标位置
    bool _isFlipped;                // 是否翻开
    bool _isClickable;              // 是否可点击
};

#endif // __CARD_MODEL_H__ 