#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "../configs/models/LevelConfig.h"
#include "../models/GameModel.h"

/**
 * @class GameModelFromLevelGenerator
 * @brief 游戏数据生成器服务
 * @details 无状态服务，将静态的LevelConfig转换为动态的GameModel
 *          不持有数据，通过参数操作和返回数据
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 从关卡配置生成游戏数据模型
     * @param levelConfig 关卡配置
     * @return 生成的游戏数据模型，调用方负责释放内存
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);
    
private:
    /**
     * @brief 生成下一个唯一的卡牌ID
     * @return 卡牌ID
     */
    static int getNextCardId();
    
    static int s_nextCardId;  // 静态卡牌ID计数器
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__ 