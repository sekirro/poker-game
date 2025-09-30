#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/stringbuffer.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    // 根据关卡ID构建文件路径
    std::string filePath = StringUtils::format("level/level_%d.json", levelId);
    return loadFromFile(filePath);
}

LevelConfig* LevelConfigLoader::loadFromFile(const std::string& filePath)
{
    // 读取JSON文件内容
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    if (jsonStr.empty()) {
        CCLOG("LevelConfigLoader: Failed to load file: %s", filePath.c_str());
        return nullptr;
    }
    
    // 解析JSON
    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());
    
    if (doc.HasParseError()) {
        CCLOG("LevelConfigLoader: JSON parse error in file: %s", filePath.c_str());
        return nullptr;
    }
    
    return parseJsonDocument(doc);
}

LevelConfig* LevelConfigLoader::parseJsonDocument(const rapidjson::Document& doc)
{
    LevelConfig* config = new LevelConfig();
    
    // 解析主牌区 (Playfield)
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& playfieldArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++) {
            const auto& cardObj = playfieldArray[i];
            
            CardConfig cardConfig;
            if (cardObj.HasMember("CardFace")) {
                cardConfig.face = parseCardFace(cardObj["CardFace"].GetInt());
            }
            if (cardObj.HasMember("CardSuit")) {
                cardConfig.suit = parseCardSuit(cardObj["CardSuit"].GetInt());
            }
            if (cardObj.HasMember("Position")) {
                const auto& pos = cardObj["Position"];
                cardConfig.position.x = pos["x"].GetFloat();
                cardConfig.position.y = pos["y"].GetFloat();
            }
            
            config->addPlayfieldCard(cardConfig);
        }
    }
    
    // 解析备用牌堆 (Stack)
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& stackArray = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++) {
            const auto& cardObj = stackArray[i];
            
            CardConfig cardConfig;
            if (cardObj.HasMember("CardFace")) {
                cardConfig.face = parseCardFace(cardObj["CardFace"].GetInt());
            }
            if (cardObj.HasMember("CardSuit")) {
                cardConfig.suit = parseCardSuit(cardObj["CardSuit"].GetInt());
            }
            // Stack中的Position可以忽略，因为会统一放置
            
            config->addStackCard(cardConfig);
        }
    }
    
    return config;
}

CardFaceType LevelConfigLoader::parseCardFace(int faceValue)
{
    // faceValue: 0=A, 1=2, 2=3, ..., 12=K
    if (faceValue >= 0 && faceValue < CFT_NUM_CARD_FACE_TYPES) {
        return static_cast<CardFaceType>(faceValue);
    }
    return CFT_NONE;
}

CardSuitType LevelConfigLoader::parseCardSuit(int suitValue)
{
    // suitValue: 0=梅花, 1=方块, 2=红桃, 3=黑桃
    if (suitValue >= 0 && suitValue < CST_NUM_CARD_SUIT_TYPES) {
        return static_cast<CardSuitType>(suitValue);
    }
    return CST_NONE;
} 