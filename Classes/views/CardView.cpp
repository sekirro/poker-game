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
    
    // 初始化Sprite（使用纯色替代，因为暂无图片资源）
    if (!Sprite::init()) {
        return false;
    }
    
    // 创建一个彩色矩形作为卡牌占位符
    auto drawNode = DrawNode::create();
    
    // 根据花色设置不同颜色
    Color4F cardColor;
    switch (_suit) {
        case CST_CLUBS:    cardColor = Color4F(0.2f, 0.8f, 0.2f, 1.0f); break; // 绿色
        case CST_DIAMONDS: cardColor = Color4F(0.8f, 0.2f, 0.2f, 1.0f); break; // 红色
        case CST_HEARTS:   cardColor = Color4F(0.8f, 0.4f, 0.6f, 1.0f); break; // 粉色
        case CST_SPADES:   cardColor = Color4F(0.2f, 0.2f, 0.8f, 1.0f); break; // 蓝色
        default:           cardColor = Color4F(0.5f, 0.5f, 0.5f, 1.0f); break; // 灰色
    }
    
    // 绘制卡牌矩形
    Vec2 rect[4] = {
        Vec2(0, 0),
        Vec2(kCardWidth, 0),
        Vec2(kCardWidth, kCardHeight),
        Vec2(0, kCardHeight)
    };
    drawNode->drawSolidPoly(rect, 4, cardColor);
    
    // 添加边框
    drawNode->drawPoly(rect, 4, true, Color4F::WHITE);
    
    // 添加牌面文字
    auto label = Label::createWithSystemFont(
        CardResConfig::getFaceName(_face), 
        "Arial", 
        40
    );
    label->setPosition(Vec2(kCardWidth/2, kCardHeight/2));
    label->setColor(Color3B::WHITE);
    drawNode->addChild(label);
    
    this->addChild(drawNode);
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