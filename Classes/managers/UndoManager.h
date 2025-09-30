#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include <functional>

/**
 * @class UndoManager
 * @brief 撤销管理器
 * @details 管理撤销功能，持有UndoModel数据并提供撤销操作
 *          作为Controller的成员变量，不实现为单例
 *          通过回调接口与其他模块交互
 */
class UndoManager
{
public:
    /**
     * @brief 撤销动画完成回调函数类型
     * @details 参数为撤销的操作
     */
    using UndoAnimationCallback = std::function<void(const UndoAction&)>;
    
    /**
     * @brief 构造函数
     */
    UndoManager();
    
    /**
     * @brief 析构函数
     */
    ~UndoManager();
    
    /**
     * @brief 初始化
     * @param undoModel 撤销数据模型
     */
    void init(UndoModel* undoModel);
    
    /**
     * @brief 记录一次操作（用于撤销）
     * @param action 操作数据
     */
    void recordAction(const UndoAction& action);
    
    /**
     * @brief 执行撤销操作
     * @param gameModel 游戏数据模型
     * @return 如果成功撤销返回true
     * @details 该方法会修改gameModel的数据，并通过回调通知View层执行动画
     */
    bool performUndo(GameModel* gameModel);
    
    /**
     * @brief 检查是否可以撤销
     * @return 如果可以撤销返回true
     */
    bool canUndo() const;
    
    /**
     * @brief 清空所有撤销记录
     */
    void clearHistory();
    
    /**
     * @brief 设置撤销动画回调
     * @param callback 回调函数
     */
    void setUndoAnimationCallback(const UndoAnimationCallback& callback);
    
private:
    /**
     * @brief 撤销"从备用牌堆翻牌"操作
     * @param action 操作数据
     * @param gameModel 游戏数据模型
     */
    void undoReplaceTrayFromStack(const UndoAction& action, GameModel* gameModel);
    
    /**
     * @brief 撤销"从主牌区匹配"操作
     * @param action 操作数据
     * @param gameModel 游戏数据模型
     */
    void undoReplaceTrayFromPlayfield(const UndoAction& action, GameModel* gameModel);
    
private:
    UndoModel* _undoModel;                      // 撤销数据模型
    UndoAnimationCallback _animationCallback;   // 动画回调
};

#endif // __UNDO_MANAGER_H__ 