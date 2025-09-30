#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <map>

/**
 * @class GameModel
 * @brief 游戏数据模型
 * @details 存储游戏的运行时数据，包括所有卡牌、游戏状态等
 *          支持序列化和反序列化，用于存档功能
 */
class GameModel
{
public:
    /**
     * @brief 构造函数
     */
    GameModel();
    
    /**
     * @brief 析构函数
     */
    ~GameModel();
    
    /**
     * @brief 清空所有数据
     */
    void clear();
    
    /**
     * @brief 添加卡牌到游戏中
     * @param card 卡牌指针
     */
    void addCard(CardModel* card);
    
    /**
     * @brief 根据ID获取卡牌
     * @param cardId 卡牌ID
     * @return 卡牌指针，如果不存在返回nullptr
     */
    CardModel* getCardById(int cardId) const;
    
    /**
     * @brief 移除卡牌
     * @param cardId 卡牌ID
     */
    void removeCard(int cardId);
    
    /**
     * @brief 获取所有卡牌
     * @return 卡牌映射表
     */
    const std::map<int, CardModel*>& getAllCards() const { return _cards; }
    
    /**
     * @brief 获取主牌区的卡牌列表
     * @return 主牌区卡牌ID列表
     */
    const std::vector<int>& getPlayfieldCardIds() const { return _playfieldCardIds; }
    
    /**
     * @brief 设置主牌区卡牌列表
     * @param cardIds 卡牌ID列表
     */
    void setPlayfieldCardIds(const std::vector<int>& cardIds) { _playfieldCardIds = cardIds; }
    
    /**
     * @brief 获取备用牌堆的卡牌列表
     * @return 备用牌堆卡牌ID列表
     */
    const std::vector<int>& getStackCardIds() const { return _stackCardIds; }
    
    /**
     * @brief 设置备用牌堆卡牌列表
     * @param cardIds 卡牌ID列表
     */
    void setStackCardIds(const std::vector<int>& cardIds) { _stackCardIds = cardIds; }
    
    /**
     * @brief 获取当前底牌堆顶部卡牌ID
     * @return 底牌ID，如果没有返回-1
     */
    int getTrayCardId() const { return _trayCardId; }
    
    /**
     * @brief 设置底牌堆顶部卡牌ID
     * @param cardId 卡牌ID
     */
    void setTrayCardId(int cardId) { _trayCardId = cardId; }
    
    /**
     * @brief 从主牌区移除卡牌
     * @param cardId 卡牌ID
     */
    void removeFromPlayfield(int cardId);
    
    /**
     * @brief 从备用牌堆弹出顶部卡牌
     * @return 弹出的卡牌ID，如果牌堆为空返回-1
     */
    int popFromStack();
    
    /**
     * @brief 检查游戏是否胜利
     * @return 如果主牌区为空返回true
     */
    bool isGameWon() const;
    
    /**
     * @brief 序列化为JSON
     * @return JSON文档
     */
    rapidjson::Document serialize() const;
    
    /**
     * @brief 从JSON反序列化
     * @param json JSON文档
     */
    void deserialize(const rapidjson::Document& json);
    
private:
    std::map<int, CardModel*> _cards;           // 所有卡牌的映射表 (cardId -> CardModel)
    std::vector<int> _playfieldCardIds;         // 主牌区卡牌ID列表
    std::vector<int> _stackCardIds;             // 备用牌堆卡牌ID列表
    int _trayCardId;                            // 底牌堆顶部卡牌ID
};

#endif // __GAME_MODEL_H__ 