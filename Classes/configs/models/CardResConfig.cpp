#include "CardResConfig.h"
#include "cocos2d.h"

std::string CardResConfig::getCardFrontTexture(CardFaceType face, CardSuitType suit)
{
    // 格式: images/cards/[suit]_[face].png
    // 例如: images/cards/hearts_A.png
    std::string suitName = getSuitName(suit);
    std::string faceName = getFaceName(face);
    return cocos2d::StringUtils::format("images/cards/%s_%s.png", suitName.c_str(), faceName.c_str());
}

std::string CardResConfig::getCardBackTexture()
{
    return "images/cards/card_back.png";
}

std::string CardResConfig::getFaceName(CardFaceType face)
{
    switch (face) {
        case CFT_ACE:   return "A";
        case CFT_TWO:   return "2";
        case CFT_THREE: return "3";
        case CFT_FOUR:  return "4";
        case CFT_FIVE:  return "5";
        case CFT_SIX:   return "6";
        case CFT_SEVEN: return "7";
        case CFT_EIGHT: return "8";
        case CFT_NINE:  return "9";
        case CFT_TEN:   return "10";
        case CFT_JACK:  return "J";
        case CFT_QUEEN: return "Q";
        case CFT_KING:  return "K";
        default:        return "unknown";
    }
}

std::string CardResConfig::getSuitName(CardSuitType suit)
{
    switch (suit) {
        case CST_CLUBS:    return "clubs";
        case CST_DIAMONDS: return "diamonds";
        case CST_HEARTS:   return "hearts";
        case CST_SPADES:   return "spades";
        default:           return "unknown";
    }
}

std::string CardResConfig::getSuitSymbol(CardSuitType suit)
{
    switch (suit) {
        case CST_CLUBS:    return "♣";
        case CST_DIAMONDS: return "♦";
        case CST_HEARTS:   return "♥";
        case CST_SPADES:   return "♠";
        default:           return "";
    }
} 