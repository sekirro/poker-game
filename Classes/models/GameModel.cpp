#include "GameModel.h"
#include <algorithm>

GameModel::GameModel()
    : _trayCardId(-1)
{
}

GameModel::~GameModel()
{
    clear();
}

void GameModel::clear()
{
    // 释放所有卡牌内存
    for (auto& pair : _cards) {
        delete pair.second;
    }
    _cards.clear();
    _playfieldCardIds.clear();
    _stackCardIds.clear();
    _trayCardId = -1;
}

void GameModel::addCard(CardModel* card)
{
    if (card) {
        _cards[card->getCardId()] = card;
    }
}

CardModel* GameModel::getCardById(int cardId) const
{
    auto it = _cards.find(cardId);
    if (it != _cards.end()) {
        return it->second;
    }
    return nullptr;
}

void GameModel::removeCard(int cardId)
{
    auto it = _cards.find(cardId);
    if (it != _cards.end()) {
        delete it->second;
        _cards.erase(it);
    }
}

void GameModel::removeFromPlayfield(int cardId)
{
    auto it = std::find(_playfieldCardIds.begin(), _playfieldCardIds.end(), cardId);
    if (it != _playfieldCardIds.end()) {
        _playfieldCardIds.erase(it);
    }
}

int GameModel::popFromStack()
{
    if (_stackCardIds.empty()) {
        return -1;
    }
    
    int cardId = _stackCardIds.back();
    _stackCardIds.pop_back();
    return cardId;
}

bool GameModel::isGameWon() const
{
    return _playfieldCardIds.empty();
}

rapidjson::Document GameModel::serialize() const
{
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();
    
    // 序列化所有卡牌
    rapidjson::Value cardsArray(rapidjson::kArrayType);
    for (const auto& pair : _cards) {
        cardsArray.PushBack(pair.second->serialize(allocator), allocator);
    }
    doc.AddMember("cards", cardsArray, allocator);
    
    // 序列化主牌区ID列表
    rapidjson::Value playfieldArray(rapidjson::kArrayType);
    for (int id : _playfieldCardIds) {
        playfieldArray.PushBack(id, allocator);
    }
    doc.AddMember("playfieldCardIds", playfieldArray, allocator);
    
    // 序列化备用牌堆ID列表
    rapidjson::Value stackArray(rapidjson::kArrayType);
    for (int id : _stackCardIds) {
        stackArray.PushBack(id, allocator);
    }
    doc.AddMember("stackCardIds", stackArray, allocator);
    
    // 序列化底牌ID
    doc.AddMember("trayCardId", _trayCardId, allocator);
    
    return doc;
}

void GameModel::deserialize(const rapidjson::Document& json)
{
    clear();
    
    if (json.HasMember("cards")) {
        const auto& cardsArray = json["cards"];
        for (rapidjson::SizeType i = 0; i < cardsArray.Size(); i++) {
            CardModel* card = new CardModel();
            card->deserialize(cardsArray[i]);
            addCard(card);
        }
    }
    
    if (json.HasMember("playfieldCardIds")) {
        const auto& array = json["playfieldCardIds"];
        for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
            _playfieldCardIds.push_back(array[i].GetInt());
        }
    }
    
    if (json.HasMember("stackCardIds")) {
        const auto& array = json["stackCardIds"];
        for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
            _stackCardIds.push_back(array[i].GetInt());
        }
    }
    
    if (json.HasMember("trayCardId")) {
        _trayCardId = json["trayCardId"].GetInt();
    }
} 