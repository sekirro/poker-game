#ifndef __CARD_DEFINES_H__
#define __CARD_DEFINES_H__

/**
 * @file CardDefines.h
 * @brief 卡牌相关的基础枚举定义
 * @details 定义了卡牌的花色类型和牌面类型，用于整个游戏系统
 */

/**
 * @enum CardSuitType
 * @brief 卡牌花色类型枚举
 */
enum CardSuitType
{
    CST_NONE = -1,      // 无花色
    CST_CLUBS,          // 梅花 ♣
    CST_DIAMONDS,       // 方块 ♦
    CST_HEARTS,         // 红桃 ♥
    CST_SPADES,         // 黑桃 ♠
    CST_NUM_CARD_SUIT_TYPES  // 花色总数
};

/**
 * @enum CardFaceType
 * @brief 卡牌牌面类型枚举
 */
enum CardFaceType
{
    CFT_NONE = -1,      // 无牌面
    CFT_ACE,            // A (1)
    CFT_TWO,            // 2
    CFT_THREE,          // 3
    CFT_FOUR,           // 4
    CFT_FIVE,           // 5
    CFT_SIX,            // 6
    CFT_SEVEN,          // 7
    CFT_EIGHT,          // 8
    CFT_NINE,           // 9
    CFT_TEN,            // 10
    CFT_JACK,           // J (11)
    CFT_QUEEN,          // Q (12)
    CFT_KING,           // K (13)
    CFT_NUM_CARD_FACE_TYPES  // 牌面总数
};

/**
 * @enum CardLocation
 * @brief 卡牌位置类型枚举
 */
enum CardLocation
{
    CL_NONE = -1,       // 无位置
    CL_PLAYFIELD,       // 主牌区（桌面区）
    CL_TRAY,            // 底牌堆（手牌区顶部牌）
    CL_STACK            // 备用牌堆
};

#endif // __CARD_DEFINES_H__ 