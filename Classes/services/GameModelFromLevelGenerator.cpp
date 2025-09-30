#include "GameModelFromLevelGenerator.h"

int GameModelFromLevelGenerator::s_nextCardId = 0;

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig)
{
    if (!levelConfig) {
        return nullptr;
    }
    
    GameModel* gameModel = new GameModel();
    
    // 重置卡牌ID计数器
    s_nextCardId = 0;
    
    // 生成主牌区卡牌
    const auto& playfieldCards = levelConfig->getPlayfieldCards();
    for (const auto& cardConfig : playfieldCards) {
        int cardId = getNextCardId();
        CardModel* card = new CardModel(cardConfig.face, cardConfig.suit, cardId);
        card->setPosition(cardConfig.position);
        card->setLocation(CL_PLAYFIELD);
        card->setFlipped(true);      // 主牌区的牌默认翻开
        card->setClickable(true);    // 主牌区的牌默认可点击
        
        gameModel->addCard(card);
        
        // 将卡牌ID添加到主牌区列表
        std::vector<int> playfieldIds = gameModel->getPlayfieldCardIds();
        playfieldIds.push_back(cardId);
        gameModel->setPlayfieldCardIds(playfieldIds);
    }
    
    // 生成备用牌堆卡牌
    const auto& stackCards = levelConfig->getStackCards();
    std::vector<int> stackIds;
    
    for (const auto& cardConfig : stackCards) {
        int cardId = getNextCardId();
        CardModel* card = new CardModel(cardConfig.face, cardConfig.suit, cardId);
        card->setLocation(CL_STACK);
        card->setFlipped(false);     // 备用牌堆的牌默认覆盖
        card->setClickable(false);   // 备用牌堆的牌默认不可点击
        
        gameModel->addCard(card);
        stackIds.push_back(cardId);
    }
    
    gameModel->setStackCardIds(stackIds);
    
    // 从备用牌堆弹出第一张牌作为底牌
    if (!stackIds.empty()) {
        int trayCardId = gameModel->popFromStack();
        gameModel->setTrayCardId(trayCardId);
        
        CardModel* trayCard = gameModel->getCardById(trayCardId);
        if (trayCard) {
            trayCard->setLocation(CL_TRAY);
            trayCard->setFlipped(true);      // 底牌翻开
            trayCard->setClickable(false);   // 底牌本身不可点击
        }
    }
    
    return gameModel;
}

int GameModelFromLevelGenerator::getNextCardId()
{
    return s_nextCardId++;
} 