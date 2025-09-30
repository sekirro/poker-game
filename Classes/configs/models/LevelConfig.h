#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "../../utils/CardDefines.h"
#include <vector>

/**
 * @struct CardConfig
 * @brief 单张卡牌的配置数据
 */
struct CardConfig
{
    CardFaceType face;      // 牌面类型
    CardSuitType suit;      // 花色类型
    cocos2d::Vec2 position; // 位置坐标
    
    CardConfig()
        : face(CFT_NONE)
        , suit(CST_NONE)
        , position(cocos2d::Vec2::ZERO)
    {
    }
    
    CardConfig(CardFaceType f, CardSuitType s, const cocos2d::Vec2& pos)
        : face(f)
        , suit(s)
        , position(pos)
    {
    }
};

/**
 * @class LevelConfig
 * @brief 关卡配置类
 * @details 存储关卡的静态配置数据，包括主牌区和备用牌堆的卡牌配置
 */
class LevelConfig
{
public:
    /**
     * @brief 构造函数
     */
    LevelConfig();
    
    /**
     * @brief 析构函数
     */
    ~LevelConfig();
    
    /**
     * @brief 获取关卡ID
     */
    int getLevelId() const { return _levelId; }
    
    /**
     * @brief 设置关卡ID
     */
    void setLevelId(int levelId) { _levelId = levelId; }
    
    /**
     * @brief 获取主牌区卡牌配置列表
     */
    const std::vector<CardConfig>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * @brief 添加主牌区卡牌配置
     */
    void addPlayfieldCard(const CardConfig& card);
    
    /**
     * @brief 获取备用牌堆卡牌配置列表
     */
    const std::vector<CardConfig>& getStackCards() const { return _stackCards; }
    
    /**
     * @brief 添加备用牌堆卡牌配置
     */
    void addStackCard(const CardConfig& card);
    
    /**
     * @brief 清空所有配置
     */
    void clear();
    
private:
    int _levelId;                           // 关卡ID
    std::vector<CardConfig> _playfieldCards; // 主牌区卡牌配置
    std::vector<CardConfig> _stackCards;     // 备用牌堆卡牌配置
};

#endif // __LEVEL_CONFIG_H__ 