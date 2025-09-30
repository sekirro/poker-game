#include "CardView.h"

USING_NS_CC;

// 卡牌尺寸常量
static const float kCardWidth = 120.0f;
static const float kCardHeight = 168.0f;

CardView::CardView()
    : _cardId(-1)
    , _face(CFT_NONE)
    , _suit(CST_NONE)
    , _isFlipped(false)
    , _isClickable(false)
    , _clickCallback(nullptr)
    , _touchListener(nullptr)
{
}

CardView::~CardView()
{
    if (_touchListener) {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
}

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* view = new (std::nothrow) CardView();
    if (view && view->init(cardModel)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel* cardModel)
{
    if (!cardModel) {
        return false;
    }
    
    // 初始化数据
    _cardId = cardModel->getCardId();
    _face = cardModel->getFace();
    _suit = cardModel->getSuit();
    _isFlipped = cardModel->isFlipped();
    _isClickable = cardModel->isClickable();
    
    // 获取纹理路径
    std::string texturePath;
    if (_isFlipped) {
        texturePath = CardResConfig::getCardFrontTexture(_face, _suit);
    } else {
        texturePath = CardResConfig::getCardBackTexture();
    }
    
    // 初始化Sprite
    if (!Sprite::init()) {
        // 如果纹理加载失败，创建一个占位的ColorRect
        if (!Sprite::initWithFile("HelloWorld.png")) {
            // 创建一个简单的颜色精灵作为占位
            auto texture = Director::getInstance()->getTextureCache()->addImage("HelloWorld.png");
            if (!texture) {
                return false;
            }
        }
        // 设置颜色来区分不同的牌
        this->setColor(Color3B(200, 200, 200));
    }
    
    // 设置卡牌大小
    this->setContentSize(Size(kCardWidth, kCardHeight));
    this->setPosition(cardModel->getPosition());
    
    // 初始化触摸监听器
    initTouchListener();
    
    return true;
}

void CardView::updateDisplay(const CardModel* cardModel)
{
    if (!cardModel) {
        return;
    }
    
    _isFlipped = cardModel->isFlipped();
    _isClickable = cardModel->isClickable();
    
    updateTexture(_isFlipped);
    setClickable(_isClickable);
}

void CardView::setClickCallback(const CardClickCallback& callback)
{
    _clickCallback = callback;
}

void CardView::moveToPosition(const Vec2& targetPosition, float duration, 
                              const std::function<void()>& callback)
{
    auto moveTo = MoveTo::create(duration, targetPosition);
    
    if (callback) {
        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(moveTo, callFunc, nullptr);
        this->runAction(sequence);
    } else {
        this->runAction(moveTo);
    }
}

void CardView::flipCard(bool showFront, float duration)
{
    // 简单的翻牌效果：缩放到0再恢复，中间切换纹理
    auto scaleToZero = ScaleTo::create(duration / 2, 0.0f, 1.0f);
    auto updateTexture = CallFunc::create([this, showFront]() {
        this->updateTexture(showFront);
    });
    auto scaleBack = ScaleTo::create(duration / 2, 1.0f, 1.0f);
    
    auto sequence = Sequence::create(scaleToZero, updateTexture, scaleBack, nullptr);
    this->runAction(sequence);
    
    _isFlipped = showFront;
}

void CardView::setClickable(bool clickable)
{
    _isClickable = clickable;
    
    if (_touchListener) {
        _touchListener->setEnabled(clickable);
    }
}

void CardView::initTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    
    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        if (!_isClickable) {
            return false;
        }
        
        // 检查触摸点是否在卡牌范围内
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        if (rect.containsPoint(locationInNode)) {
            // 点击效果：稍微缩小
            this->setScale(0.95f);
            return true;
        }
        return false;
    };
    
    _touchListener->onTouchEnded = [this](Touch* touch, Event* event) {
        // 恢复原始大小
        this->setScale(1.0f);
        
        // 触发点击回调
        if (_clickCallback) {
            _clickCallback(_cardId);
        }
    };
    
    _touchListener->onTouchCancelled = [this](Touch* touch, Event* event) {
        // 恢复原始大小
        this->setScale(1.0f);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    _touchListener->setEnabled(_isClickable);
}

void CardView::updateTexture(bool showFront)
{
    std::string texturePath;
    if (showFront) {
        texturePath = CardResConfig::getCardFrontTexture(_face, _suit);
    } else {
        texturePath = CardResConfig::getCardBackTexture();
    }
    
    // 尝试加载纹理（实际项目中需要资源文件）
    // 这里暂时保持原样，等资源文件准备好后会自动加载
} 