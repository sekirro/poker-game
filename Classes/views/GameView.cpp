#include "GameView.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

// 布局常量（根据需求文档：设计分辨率1080*2080）
static const float kDesignWidth = 1080.0f;
static const float kDesignHeight = 2080.0f;
static const float kPlayfieldHeight = 1500.0f;
static const float kStackAreaHeight = 580.0f;
static const float kPlayfieldOffsetY = 700.0f;  // 主牌区整体向上偏移
static const float kTrayPosY = 400.0f;          // 底牌堆Y坐标（向上移动）
static const float kStackPosY = 400.0f;         // 备用牌堆Y坐标（和底牌堆同高）
static const float kUndoButtonPosY = 250.0f;    // 撤销按钮Y坐标（也向上移）

GameView* GameView::create()
{
    GameView* view = new (std::nothrow) GameView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    // 创建各个区域
    createPlayfieldArea();
    createTrayArea();
    createStackArea();
    createUIButtons();
    
    return true;
}

void GameView::initGameView(const GameModel* gameModel)
{
    if (!gameModel) {
        return;
    }
    
    // 清空现有卡牌视图
    for (auto& pair : _cardViews) {
        pair.second->removeFromParent();
    }
    _cardViews.clear();
    
    // 创建所有卡牌视图
    const auto& allCards = gameModel->getAllCards();
    for (const auto& pair : allCards) {
        const CardModel* cardModel = pair.second;
        CardView* cardView = CardView::create(cardModel);
        
        if (cardView) {
            _cardViews[cardModel->getCardId()] = cardView;
            
            // 根据卡牌位置添加到相应的层
            switch (cardModel->getLocation()) {
                case CL_PLAYFIELD:
                    _playfieldLayer->addChild(cardView);
                    // 主牌区卡牌整体向上偏移
                    cardView->setPositionY(cardView->getPositionY() + kPlayfieldOffsetY);
                    break;
                case CL_TRAY:
                    _trayLayer->addChild(cardView);
                    break;
                case CL_STACK:
                    _stackLayer->addChild(cardView);
                    break;
                default:
                    break;
            }
            
            // 设置卡牌点击回调
            cardView->setClickCallback([this](int cardId) {
                if (_cardClickCallback) {
                    _cardClickCallback(cardId);
                }
            });
        }
    }
}

CardView* GameView::getCardView(int cardId) const
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) {
        return it->second;
    }
    return nullptr;
}

void GameView::setCardClickCallback(const CardClickCallback& callback)
{
    _cardClickCallback = callback;
}

void GameView::setStackClickCallback(const StackClickCallback& callback)
{
    _stackClickCallback = callback;
}

void GameView::setUndoButtonClickCallback(const UndoButtonClickCallback& callback)
{
    _undoButtonClickCallback = callback;
}

void GameView::playCardMoveAnimation(int cardId, const Vec2& targetPosition, 
                                     float duration, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView) {
        cardView->moveToPosition(targetPosition, duration, callback);
    }
}

void GameView::playMatchAnimation(int fromCardId, int toCardId, const std::function<void()>& callback)
{
    CardView* fromCard = getCardView(fromCardId);
    CardView* toCard = getCardView(toCardId);
    
    if (fromCard && toCard) {
        Vec2 targetPos = toCard->getPosition();
        fromCard->moveToPosition(targetPos, 0.3f, callback);
    }
}

void GameView::updateUndoButton(bool enabled)
{
    if (_undoButton) {
        _undoButton->setEnabled(enabled);
        _undoButton->setBright(enabled);
    }
}

void GameView::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) {
        it->second->removeFromParent();
        _cardViews.erase(it);
    }
}

void GameView::createPlayfieldArea()
{
    _playfieldLayer = Layer::create();
    this->addChild(_playfieldLayer, 1);
}

void GameView::createTrayArea()
{
    _trayLayer = Layer::create();
    this->addChild(_trayLayer, 2);
}

void GameView::createStackArea()
{
    _stackLayer = Layer::create();
    this->addChild(_stackLayer, 2);
    
    // 创建备用牌堆的可点击区域（使用DrawNode绘制）
    auto drawNode = DrawNode::create();
    Vec2 stackPos = getStackPosition();
    
    // 绘制一个灰色矩形表示牌堆
    Vec2 rect[4] = {
        Vec2(-60, -84),
        Vec2(60, -84),
        Vec2(60, 84),
        Vec2(-60, 84)
    };
    drawNode->drawSolidPoly(rect, 4, Color4F(0.3f, 0.3f, 0.3f, 1.0f));
    drawNode->drawPoly(rect, 4, true, Color4F::WHITE);
    
    // 添加"STACK"文字
    auto label = Label::createWithSystemFont("STACK", "Arial", 24);
    label->setPosition(Vec2(0, 0));
    label->setColor(Color3B::WHITE);
    drawNode->addChild(label);
    
    drawNode->setPosition(stackPos);
    _stackLayer->addChild(drawNode);
    _stackSprite = drawNode;
    
    // 添加触摸监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this, drawNode](Touch* touch, Event* event) -> bool {
        Vec2 locationInNode = drawNode->convertToNodeSpace(touch->getLocation());
        Rect touchRect = Rect(-60, -84, 120, 168);
        
        if (touchRect.containsPoint(locationInNode)) {
            drawNode->setScale(0.95f);
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this, drawNode](Touch* touch, Event* event) {
        drawNode->setScale(1.0f);
        if (_stackClickCallback) {
            _stackClickCallback();
        }
    };
    
    listener->onTouchCancelled = [this, drawNode](Touch* touch, Event* event) {
        drawNode->setScale(1.0f);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, drawNode);
}

void GameView::createUIButtons()
{
    // 创建撤销按钮
    _undoButton = ui::Button::create();
    _undoButton->setTitleText("Undo");
    _undoButton->setTitleFontSize(32);
    _undoButton->setPosition(Vec2(kDesignWidth - 100, kUndoButtonPosY));
    _undoButton->addClickEventListener([this](Ref* sender) {
        if (_undoButtonClickCallback) {
            _undoButtonClickCallback();
        }
    });
    this->addChild(_undoButton, 10);
}

Vec2 GameView::getTrayPosition() const
{
    // 底牌堆位置：屏幕中央偏左
    return Vec2(kDesignWidth / 2 - 150, kTrayPosY);
}

Vec2 GameView::getStackPosition() const
{
    // 备用牌堆位置：屏幕中央
    return Vec2(kDesignWidth / 2, kStackPosY);
} 