#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "../models/LevelConfig.h"
#include "json/document.h"
#include <string>

/**
 * @class LevelConfigLoader
 * @brief 关卡配置加载器
 * @details 负责从JSON文件加载关卡配置数据
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 从JSON文件加载关卡配置
     * @param levelId 关卡ID
     * @return 关卡配置指针，加载失败返回nullptr
     */
    static LevelConfig* loadLevelConfig(int levelId);
    
    /**
     * @brief 从JSON文件路径加载关卡配置
     * @param filePath JSON文件路径
     * @return 关卡配置指针，加载失败返回nullptr
     */
    static LevelConfig* loadFromFile(const std::string& filePath);
    
private:
    /**
     * @brief 解析JSON文档为关卡配置
     * @param doc JSON文档
     * @return 关卡配置指针
     */
    static LevelConfig* parseJsonDocument(const rapidjson::Document& doc);
    
    /**
     * @brief 解析CardFaceType
     * @param faceValue 牌面值（0-12对应A-K）
     * @return CardFaceType
     */
    static CardFaceType parseCardFace(int faceValue);
    
    /**
     * @brief 解析CardSuitType
     * @param suitValue 花色值（0-3对应梅花、方块、红桃、黑桃）
     * @return CardSuitType
     */
    static CardSuitType parseCardSuit(int suitValue);
};

#endif // __LEVEL_CONFIG_LOADER_H__ 