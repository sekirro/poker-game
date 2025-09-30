#include "UndoManager.h"
#include "cocos2d.h"

USING_NS_CC;

UndoManager::UndoManager()
    : _undoModel(nullptr)
    , _animationCallback(nullptr)
{
}

UndoManager::~UndoManager()
{
}

void UndoManager::init(UndoModel* undoModel)
{
    _undoModel = undoModel;
}

void UndoManager::recordAction(const UndoAction& action)
{
    if (_undoModel) {
        _undoModel->pushAction(action);
        CCLOG("UndoManager: Recorded action type=%d, fromCardId=%d, toCardId=%d", 
              action.type, action.fromCardId, action.toCardId);
    }
}

bool UndoManager::performUndo(GameModel* gameModel)
{
    if (!_undoModel || !gameModel || !canUndo()) {
        CCLOG("UndoManager: Cannot undo");
        return false;
    }
    
    // 弹出最后一个操作
    UndoAction action = _undoModel->popAction();
    
    CCLOG("UndoManager: Performing undo, type=%d, fromCardId=%d, toCardId=%d", 
          action.type, action.fromCardId, action.toCardId);
    
    // 根据操作类型执行相应的撤销逻辑
    switch (action.type) {
        case UAT_REPLACE_TRAY_FROM_STACK:
            undoReplaceTrayFromStack(action, gameModel);
            break;
            
        case UAT_REPLACE_TRAY_FROM_PLAYFIELD:
            undoReplaceTrayFromPlayfield(action, gameModel);
            break;
            
        default:
            CCLOG("UndoManager: Unknown action type");
            return false;
    }
    
    // 通知View层执行撤销动画
    if (_animationCallback) {
        _animationCallback(action);
    }
    
    return true;
}

bool UndoManager::canUndo() const
{
    return _undoModel && _undoModel->canUndo();
}

void UndoManager::clearHistory()
{
    if (_undoModel) {
        _undoModel->clear();
    }
}

void UndoManager::setUndoAnimationCallback(const UndoAnimationCallback& callback)
{
    _animationCallback = callback;
}

void UndoManager::undoReplaceTrayFromStack(const UndoAction& action, GameModel* gameModel)
{
    // 原操作：从备用牌堆弹出fromCard，替换trayCard（toCard）
    // 撤销：将fromCard放回备用牌堆，恢复toCard为trayCard
    
    int fromCardId = action.fromCardId;  // 当前的trayCard（需要放回stack）
    int toCardId = action.toCardId;      // 原来的trayCard（需要恢复）
    
    CardModel* fromCard = gameModel->getCardById(fromCardId);
    CardModel* toCard = gameModel->getCardById(toCardId);
    
    if (!fromCard || !toCard) {
        CCLOG("UndoManager: Card not found in undoReplaceTrayFromStack");
        return;
    }
    
    // 将fromCard放回备用牌堆顶部
    fromCard->setLocation(CL_STACK);
    fromCard->setFlipped(false);
    fromCard->setClickable(false);
    fromCard->setPosition(action.fromPosition);
    
    std::vector<int> stackIds = gameModel->getStackCardIds();
    stackIds.push_back(fromCardId);
    gameModel->setStackCardIds(stackIds);
    
    // 恢复toCard为底牌
    toCard->setLocation(CL_TRAY);
    toCard->setFlipped(true);
    toCard->setClickable(false);
    toCard->setPosition(action.toPosition);
    
    gameModel->setTrayCardId(toCardId);
}

void UndoManager::undoReplaceTrayFromPlayfield(const UndoAction& action, GameModel* gameModel)
{
    // 原操作：点击主牌区的fromCard，替换trayCard（toCard）
    // 撤销：将fromCard放回主牌区，恢复toCard为trayCard
    
    int fromCardId = action.fromCardId;  // 当前的trayCard（需要放回playfield）
    int toCardId = action.toCardId;      // 原来的trayCard（需要恢复）
    
    CardModel* fromCard = gameModel->getCardById(fromCardId);
    CardModel* toCard = gameModel->getCardById(toCardId);
    
    if (!fromCard || !toCard) {
        CCLOG("UndoManager: Card not found in undoReplaceTrayFromPlayfield");
        return;
    }
    
    // 将fromCard放回主牌区
    fromCard->setLocation(CL_PLAYFIELD);
    fromCard->setFlipped(true);
    fromCard->setClickable(true);
    fromCard->setPosition(action.fromPosition);
    
    std::vector<int> playfieldIds = gameModel->getPlayfieldCardIds();
    playfieldIds.push_back(fromCardId);
    gameModel->setPlayfieldCardIds(playfieldIds);
    
    // 恢复toCard为底牌
    toCard->setLocation(CL_TRAY);
    toCard->setFlipped(true);
    toCard->setClickable(false);
    toCard->setPosition(action.toPosition);
    
    gameModel->setTrayCardId(toCardId);
} 