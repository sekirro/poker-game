#include "LevelConfig.h"

LevelConfig::LevelConfig()
    : _levelId(0)
{
}

LevelConfig::~LevelConfig()
{
    clear();
}

void LevelConfig::addPlayfieldCard(const CardConfig& card)
{
    _playfieldCards.push_back(card);
}

void LevelConfig::addStackCard(const CardConfig& card)
{
    _stackCards.push_back(card);
}

void LevelConfig::clear()
{
    _playfieldCards.clear();
    _stackCards.clear();
} 