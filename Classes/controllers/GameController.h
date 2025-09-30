#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../models/UndoModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"
#include "../configs/models/LevelConfig.h"

/**
 * @class GameController
 * @brief 游戏控制器
 * @details 控制器层，管理整个游戏流程，协调Model和View
 *          处理用户操作的业务逻辑，连接视图和模型
 */
class GameController : public cocos2d::Ref
{
public:
    /**
     * @brief 构造函数
     */
    GameController();
    
    /**
     * @brief 析构函数
     */
    virtual ~GameController();
    
    /**
     * @brief 开始游戏（加载关卡）
     * @param levelId 关卡ID
     * @param parentNode 父节点，用于添加GameView
     * @return 是否成功开始游戏
     */
    bool startGame(int levelId, cocos2d::Node* parentNode);
    
    /**
     * @brief 处理卡牌点击事件
     * @param cardId 卡牌ID
     */
    void handleCardClick(int cardId);
    
    /**
     * @brief 处理备用牌堆点击事件
     */
    void handleStackClick();
    
    /**
     * @brief 处理撤销按钮点击事件
     */
    void handleUndoClick();
    
    /**
     * @brief 检查游戏是否胜利
     * @return 如果胜利返回true
     */
    bool checkGameWin();
    
private:
    /**
     * @brief 初始化游戏数据和视图
     * @param levelConfig 关卡配置
     * @param parentNode 父节点
     * @return 是否成功初始化
     */
    bool initGame(const LevelConfig* levelConfig, cocos2d::Node* parentNode);
    
    /**
     * @brief 处理主牌区卡牌点击（匹配逻辑）
     * @param cardId 卡牌ID
     */
    void handlePlayfieldCardClick(int cardId);
    
    /**
     * @brief 从备用牌堆翻牌到底牌堆
     */
    void replaceTrayFromStack();
    
    /**
     * @brief 用主牌区的卡牌替换底牌
     * @param playfieldCardId 主牌区卡牌ID
     */
    void replaceTrayFromPlayfield(int playfieldCardId);
    
    /**
     * @brief 执行撤销操作
     */
    void performUndo();
    
    /**
     * @brief 更新撤销按钮状态
     */
    void updateUndoButtonState();
    
    /**
     * @brief 检查主牌区卡牌是否可以与底牌匹配
     * @param cardId 主牌区卡牌ID
     * @return 如果可以匹配返回true
     */
    bool canMatchWithTray(int cardId) const;
    
private:
    GameModel* _gameModel;          // 游戏数据模型
    UndoModel* _undoModel;          // 撤销数据模型
    GameView* _gameView;            // 游戏视图
    UndoManager* _undoManager;      // 撤销管理器
};

#endif // __GAME_CONTROLLER_H__ 