#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "../../utils/CardDefines.h"
#include <string>

/**
 * @class CardResConfig
 * @brief 卡牌UI资源配置类
 * @details 提供卡牌纹理资源路径的映射
 */
class CardResConfig
{
public:
    /**
     * @brief 获取卡牌正面纹理路径
     * @param face 牌面类型
     * @param suit 花色类型
     * @return 纹理文件路径
     */
    static std::string getCardFrontTexture(CardFaceType face, CardSuitType suit);
    
    /**
     * @brief 获取卡牌背面纹理路径
     * @return 纹理文件路径
     */
    static std::string getCardBackTexture();
    
    /**
     * @brief 获取牌面名称字符串（用于文件名）
     * @param face 牌面类型
     * @return 牌面名称（如 "A", "2", "J", "Q", "K"）
     */
    static std::string getFaceName(CardFaceType face);
    
    /**
     * @brief 获取花色名称字符串（用于文件名）
     * @param suit 花色类型
     * @return 花色名称（如 "clubs", "diamonds", "hearts", "spades"）
     */
    static std::string getSuitName(CardSuitType suit);
    
    /**
     * @brief 获取花色符号
     * @param suit 花色类型
     * @return 花色符号（如 "♣", "♦", "♥", "♠"）
     */
    static std::string getSuitSymbol(CardSuitType suit);
};

#endif // __CARD_RES_CONFIG_H__ 