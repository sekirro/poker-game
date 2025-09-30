#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include <vector>

/**
 * @enum UndoActionType
 * @brief 撤销操作类型枚举
 */
enum UndoActionType
{
    UAT_NONE = -1,
    UAT_REPLACE_TRAY_FROM_STACK,    // 从备用牌堆翻牌替换底牌
    UAT_REPLACE_TRAY_FROM_PLAYFIELD // 从主牌区匹配替换底牌
};

/**
 * @struct UndoAction
 * @brief 单次撤销操作的数据结构
 */
struct UndoAction
{
    UndoActionType type;            // 操作类型
    int fromCardId;                 // 源卡牌ID（移动的卡牌）
    int toCardId;                   // 目标卡牌ID（被替换的卡牌）
    cocos2d::Vec2 fromPosition;     // 源位置
    cocos2d::Vec2 toPosition;       // 目标位置
    
    UndoAction()
        : type(UAT_NONE)
        , fromCardId(-1)
        , toCardId(-1)
        , fromPosition(cocos2d::Vec2::ZERO)
        , toPosition(cocos2d::Vec2::ZERO)
    {
    }
};

/**
 * @class UndoModel
 * @brief 撤销操作数据模型
 * @details 存储游戏的撤销操作历史记录
 */
class UndoModel
{
public:
    /**
     * @brief 构造函数
     */
    UndoModel();
    
    /**
     * @brief 析构函数
     */
    ~UndoModel();
    
    /**
     * @brief 添加一条撤销记录
     * @param action 撤销操作
     */
    void pushAction(const UndoAction& action);
    
    /**
     * @brief 弹出最后一条撤销记录
     * @return 撤销操作，如果没有记录返回空操作
     */
    UndoAction popAction();
    
    /**
     * @brief 清空所有撤销记录
     */
    void clear();
    
    /**
     * @brief 检查是否有可撤销的操作
     * @return 如果有返回true
     */
    bool canUndo() const;
    
    /**
     * @brief 获取撤销记录数量
     * @return 记录数量
     */
    int getActionCount() const;
    
private:
    std::vector<UndoAction> _actions;  // 撤销操作历史记录栈
};

#endif // __UNDO_MODEL_H__ 