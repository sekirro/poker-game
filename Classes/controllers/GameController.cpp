#include "GameController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/GameModelFromLevelGenerator.h"

USING_NS_CC;

GameController::GameController()
    : _gameModel(nullptr)
    , _undoModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
{
}

GameController::~GameController()
{
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoModel);
    CC_SAFE_DELETE(_undoManager);
    // _gameView由Cocos2d-x自动管理，不需要手动delete
}

bool GameController::startGame(int levelId, Node* parentNode)
{
    if (!parentNode) {
        CCLOG("GameController: parentNode is null");
        return false;
    }
    
    // 加载关卡配置
    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig) {
        CCLOG("GameController: Failed to load level config for level %d", levelId);
        return false;
    }
    
    bool success = initGame(levelConfig, parentNode);
    
    delete levelConfig;
    return success;
}

bool GameController::initGame(const LevelConfig* levelConfig, Node* parentNode)
{
    // 生成游戏数据模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    if (!_gameModel) {
        CCLOG("GameController: Failed to generate game model");
        return false;
    }
    
    // 创建撤销数据模型
    _undoModel = new UndoModel();
    
    // 创建撤销管理器
    _undoManager = new UndoManager();
    _undoManager->init(_undoModel);
    
    // 设置撤销动画回调
    _undoManager->setUndoAnimationCallback([this](const UndoAction& action) {
        // 根据撤销类型播放相应的动画
        Vec2 fromPos = action.fromPosition;
        Vec2 toPos = action.toPosition;
        
        switch (action.type) {
            case UAT_REPLACE_TRAY_FROM_STACK:
                // 动画：fromCard从tray位置移回stack位置
                _gameView->playCardMoveAnimation(action.fromCardId, fromPos, 0.3f);
                // 动画：toCard从不可见移回tray位置
                _gameView->playCardMoveAnimation(action.toCardId, toPos, 0.3f);
                break;
                
            case UAT_REPLACE_TRAY_FROM_PLAYFIELD:
                // 动画：fromCard从tray位置移回playfield位置
                _gameView->playCardMoveAnimation(action.fromCardId, fromPos, 0.3f);
                // 动画：toCard从不可见移回tray位置
                _gameView->playCardMoveAnimation(action.toCardId, toPos, 0.3f);
                break;
                
            default:
                break;
        }
        
        updateUndoButtonState();
    });
    
    // 创建游戏视图
    _gameView = GameView::create();
    if (!_gameView) {
        CCLOG("GameController: Failed to create game view");
        return false;
    }
    
    parentNode->addChild(_gameView);
    
    // 初始化游戏视图
    _gameView->initGameView(_gameModel);
    
    // 设置视图回调
    _gameView->setCardClickCallback([this](int cardId) {
        handleCardClick(cardId);
    });
    
    _gameView->setStackClickCallback([this]() {
        handleStackClick();
    });
    
    _gameView->setUndoButtonClickCallback([this]() {
        handleUndoClick();
    });
    
    // 初始化撤销按钮状态
    updateUndoButtonState();
    
    CCLOG("GameController: Game started successfully");
    return true;
}

void GameController::handleCardClick(int cardId)
{
    if (!_gameModel) {
        return;
    }
    
    CardModel* card = _gameModel->getCardById(cardId);
    if (!card) {
        CCLOG("GameController: Card %d not found", cardId);
        return;
    }
    
    // 根据卡牌位置处理点击
    switch (card->getLocation()) {
        case CL_PLAYFIELD:
            handlePlayfieldCardClick(cardId);
            break;
            
        case CL_TRAY:
            // 底牌本身不可点击，忽略
            break;
            
        case CL_STACK:
            // 备用牌堆的牌不可直接点击，通过handleStackClick处理
            break;
            
        default:
            break;
    }
}

void GameController::handlePlayfieldCardClick(int cardId)
{
    if (!canMatchWithTray(cardId)) {
        CCLOG("GameController: Card %d cannot match with tray", cardId);
        return;
    }
    
    replaceTrayFromPlayfield(cardId);
    
    // 检查是否胜利
    if (checkGameWin()) {
        CCLOG("GameController: You Win!");
        // TODO: 显示胜利界面
    }
}

void GameController::handleStackClick()
{
    if (!_gameModel) {
        return;
    }
    
    // 检查备用牌堆是否还有牌
    if (_gameModel->getStackCardIds().empty()) {
        CCLOG("GameController: Stack is empty");
        return;
    }
    
    replaceTrayFromStack();
}

void GameController::handleUndoClick()
{
    performUndo();
}

void GameController::replaceTrayFromStack()
{
    if (!_gameModel || !_undoManager) {
        return;
    }
    
    // 获取当前底牌和备用牌堆顶牌
    int oldTrayCardId = _gameModel->getTrayCardId();
    int newTrayCardId = _gameModel->popFromStack();
    
    if (newTrayCardId == -1) {
        CCLOG("GameController: No cards in stack");
        return;
    }
    
    CardModel* oldTrayCard = _gameModel->getCardById(oldTrayCardId);
    CardModel* newTrayCard = _gameModel->getCardById(newTrayCardId);
    
    if (!oldTrayCard || !newTrayCard) {
        return;
    }
    
    // 记录撤销操作（在修改数据前）
    UndoAction undoAction;
    undoAction.type = UAT_REPLACE_TRAY_FROM_STACK;
    undoAction.fromCardId = newTrayCardId;
    undoAction.toCardId = oldTrayCardId;
    undoAction.fromPosition = _gameView->getStackPosition();  // 备用牌堆位置
    undoAction.toPosition = oldTrayCard->getPosition();       // 当前底牌位置
    _undoManager->recordAction(undoAction);
    
    // 更新数据模型
    Vec2 trayPos = _gameView->getTrayPosition();
    
    newTrayCard->setLocation(CL_TRAY);
    newTrayCard->setFlipped(true);
    newTrayCard->setPosition(trayPos);
    _gameModel->setTrayCardId(newTrayCardId);
    
    // 播放动画
    _gameView->playCardMoveAnimation(newTrayCardId, trayPos, 0.3f);
    
    // 更新撤销按钮状态
    updateUndoButtonState();
    
    CCLOG("GameController: Replaced tray from stack, new tray card: %d", newTrayCardId);
}

void GameController::replaceTrayFromPlayfield(int playfieldCardId)
{
    if (!_gameModel || !_undoManager) {
        return;
    }
    
    CardModel* playfieldCard = _gameModel->getCardById(playfieldCardId);
    int oldTrayCardId = _gameModel->getTrayCardId();
    CardModel* oldTrayCard = _gameModel->getCardById(oldTrayCardId);
    
    if (!playfieldCard || !oldTrayCard) {
        return;
    }
    
    // 记录撤销操作（在修改数据前）
    UndoAction undoAction;
    undoAction.type = UAT_REPLACE_TRAY_FROM_PLAYFIELD;
    undoAction.fromCardId = playfieldCardId;
    undoAction.toCardId = oldTrayCardId;
    undoAction.fromPosition = playfieldCard->getPosition();
    undoAction.toPosition = oldTrayCard->getPosition();
    _undoManager->recordAction(undoAction);
    
    // 更新数据模型
    Vec2 trayPos = _gameView->getTrayPosition();
    
    // 从主牌区移除
    _gameModel->removeFromPlayfield(playfieldCardId);
    
    // 将主牌区卡牌移动到底牌堆
    playfieldCard->setLocation(CL_TRAY);
    playfieldCard->setPosition(trayPos);
    _gameModel->setTrayCardId(playfieldCardId);
    
    // 播放动画
    _gameView->playMatchAnimation(playfieldCardId, oldTrayCardId);
    
    // 更新撤销按钮状态
    updateUndoButtonState();
    
    CCLOG("GameController: Matched card %d with tray", playfieldCardId);
}

void GameController::performUndo()
{
    if (!_undoManager || !_gameModel) {
        return;
    }
    
    if (!_undoManager->canUndo()) {
        CCLOG("GameController: No action to undo");
        return;
    }
    
    _undoManager->performUndo(_gameModel);
}

void GameController::updateUndoButtonState()
{
    if (_gameView && _undoManager) {
        bool canUndo = _undoManager->canUndo();
        _gameView->updateUndoButton(canUndo);
    }
}

bool GameController::canMatchWithTray(int cardId) const
{
    if (!_gameModel) {
        return false;
    }
    
    CardModel* card = _gameModel->getCardById(cardId);
    int trayCardId = _gameModel->getTrayCardId();
    CardModel* trayCard = _gameModel->getCardById(trayCardId);
    
    if (!card || !trayCard) {
        return false;
    }
    
    // 检查是否可以匹配（牌面数值差1）
    return card->canMatchWith(trayCard);
}

bool GameController::checkGameWin()
{
    if (_gameModel) {
        return _gameModel->isGameWon();
    }
    return false;
} 