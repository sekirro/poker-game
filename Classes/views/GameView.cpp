#include "GameView.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

// 布局常量（根据需求文档：设计分辨率1080*2080）
static const float kDesignWidth = 1080.0f;
static const float kDesignHeight = 2080.0f;
static const float kPlayfieldHeight = 1500.0f;
static const float kStackAreaHeight = 580.0f;
static const float kTrayPosY = 400.0f;      // 底牌堆Y坐标
static const float kStackPosY = 200.0f;     // 备用牌堆Y坐标
static const float kUndoButtonPosY = 80.0f; // 撤销按钮Y坐标

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
    
    // 创建备用牌堆的可点击区域
    _stackSprite = Sprite::create();
    if (!_stackSprite) {
        _stackSprite = Sprite::create("HelloWorld.png");
    }
    
    if (_stackSprite) {
        _stackSprite->setPosition(getStackPosition());
        _stackSprite->setContentSize(Size(120, 168));
        _stackSprite->setColor(Color3B(150, 150, 200));
        _stackLayer->addChild(_stackSprite);
        
        // 添加触摸监听器
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
            Vec2 locationInNode = _stackSprite->convertToNodeSpace(touch->getLocation());
            Size size = _stackSprite->getContentSize();
            Rect rect = Rect(0, 0, size.width, size.height);
            
            if (rect.containsPoint(locationInNode)) {
                _stackSprite->setScale(0.95f);
                return true;
            }
            return false;
        };
        
        listener->onTouchEnded = [this](Touch* touch, Event* event) {
            _stackSprite->setScale(1.0f);
            if (_stackClickCallback) {
                _stackClickCallback();
            }
        };
        
        listener->onTouchCancelled = [this](Touch* touch, Event* event) {
            _stackSprite->setScale(1.0f);
        };
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _stackSprite);
    }
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
    return Vec2(kDesignWidth / 2, kTrayPosY);
}

Vec2 GameView::getStackPosition() const
{
    return Vec2(kDesignWidth / 2, kStackPosY);
} 