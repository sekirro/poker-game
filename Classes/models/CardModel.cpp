#include "CardModel.h"
#include <cmath>

CardModel::CardModel(CardFaceType face, CardSuitType suit, int cardId)
    : _cardId(cardId)
    , _face(face)
    , _suit(suit)
    , _location(CL_NONE)
    , _position(cocos2d::Vec2::ZERO)
    , _isFlipped(false)
    , _isClickable(false)
{
}

CardModel::~CardModel()
{
}

bool CardModel::canMatchWith(const CardModel* otherCard) const
{
    if (!otherCard) {
        return false;
    }
    
    // 获取两张牌的牌面数值
    int thisValue = getFaceValue();
    int otherValue = otherCard->getFaceValue();
    
    // 数值差为1即可匹配（无花色限制）
    return std::abs(thisValue - otherValue) == 1;
}

int CardModel::getFaceValue() const
{
    // A=1, 2=2, ..., 10=10, J=11, Q=12, K=13
    return static_cast<int>(_face) + 1;
}

rapidjson::Value CardModel::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value obj(rapidjson::kObjectType);
    
    obj.AddMember("cardId", _cardId, allocator);
    obj.AddMember("face", static_cast<int>(_face), allocator);
    obj.AddMember("suit", static_cast<int>(_suit), allocator);
    obj.AddMember("location", static_cast<int>(_location), allocator);
    
    rapidjson::Value pos(rapidjson::kObjectType);
    pos.AddMember("x", _position.x, allocator);
    pos.AddMember("y", _position.y, allocator);
    obj.AddMember("position", pos, allocator);
    
    obj.AddMember("isFlipped", _isFlipped, allocator);
    obj.AddMember("isClickable", _isClickable, allocator);
    
    return obj;
}

void CardModel::deserialize(const rapidjson::Value& json)
{
    if (json.HasMember("cardId")) {
        _cardId = json["cardId"].GetInt();
    }
    if (json.HasMember("face")) {
        _face = static_cast<CardFaceType>(json["face"].GetInt());
    }
    if (json.HasMember("suit")) {
        _suit = static_cast<CardSuitType>(json["suit"].GetInt());
    }
    if (json.HasMember("location")) {
        _location = static_cast<CardLocation>(json["location"].GetInt());
    }
    if (json.HasMember("position")) {
        const auto& pos = json["position"];
        _position.x = pos["x"].GetFloat();
        _position.y = pos["y"].GetFloat();
    }
    if (json.HasMember("isFlipped")) {
        _isFlipped = json["isFlipped"].GetBool();
    }
    if (json.HasMember("isClickable")) {
        _isClickable = json["isClickable"].GetBool();
    }
} 