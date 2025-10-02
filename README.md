# 纸牌游戏程序设计文档

## 项目概述

这是一个基于 **Cocos2d-x 3.17** 和 **C++** 开发的纸牌消除游戏。游戏采用严格的 **MVC架构**，实现了卡牌匹配、撤销功能等核心玩法。

## 开发环境

- **引擎**: Cocos2d-x 3.17
- **语言**: C++
- **设计分辨率**: 1080 x 2080
- **窗口大小**: 1080 x 2080 (缩放0.5)

## 架构设计

### MVC架构概述

项目采用经典的 MVC（Model-View-Controller）架构，将数据、视图和业务逻辑分离，确保代码的**可维护性**和**可扩展性**。

```
Classes/
├── configs/          # 静态配置相关类
│   ├── models/       # 配置数据结构
│   └── loaders/      # 配置加载器
├── models/           # 运行时动态数据模型
├── views/            # 视图层（UI展示组件）
├── controllers/      # 控制器层（协调模型和视图）
├── managers/         # 管理器层（提供全局性服务）
├── services/         # 服务层（无状态业务逻辑）
└── utils/            # 工具类（通用功能）
```

### 目录结构详解

#### 1. `configs/` - 静态配置层

**职责**: 管理所有静态配置数据（关卡配置、资源配置等）

**核心类**:
- `LevelConfig`: 关卡配置数据结构
- `LevelConfigLoader`: 从JSON加载关卡配置
- `CardResConfig`: 卡牌资源路径配置

**示例**:
```cpp
// 加载关卡1的配置
LevelConfig* config = LevelConfigLoader::loadLevelConfig(1);
```

#### 2. `models/` - 数据模型层

**职责**: 存储游戏的运行时数据和状态，支持序列化

**核心类**:
- `CardModel`: 单张卡牌的数据（牌面、花色、位置、状态等）
- `GameModel`: 游戏全局数据（所有卡牌、主牌区、备用牌堆等）
- `UndoModel`: 撤销操作的历史记录

**特性**:
- 纯数据存储，不包含复杂业务逻辑
- 支持序列化和反序列化（用于存档功能）
- 提供数据访问接口

**示例**:
```cpp
// 检查卡牌是否可以匹配
CardModel* card1 = gameModel->getCardById(1);
CardModel* card2 = gameModel->getCardById(2);
bool canMatch = card1->canMatchWith(card2);
```

#### 3. `views/` - 视图层

**职责**: 负责UI显示和接收用户输入，不包含业务逻辑

**核心类**:
- `CardView`: 单张卡牌的视图（显示、动画、触摸交互）
- `GameView`: 游戏主视图（管理所有UI组件）

**特性**:
- 可持有 `const` 类型的 Model 指针（只读）
- 通过回调函数与 Controller 交互
- 负责所有动画效果（MoveTo、缩放等）

**示例**:
```cpp
// 设置卡牌点击回调
cardView->setClickCallback([](int cardId) {
    // 通知Controller处理点击
});

// 播放移动动画
cardView->moveToPosition(targetPos, 0.3f);
```

#### 4. `controllers/` - 控制器层

**职责**: 处理用户操作和业务逻辑，连接 View 和 Model

**核心类**:
- `GameController`: 游戏主控制器（管理整个游戏流程）

**特性**:
- 协调 Model 和 View 的交互
- 实现游戏规则和业务逻辑
- 可依赖多个 Service 和 Manager

**示例**:
```cpp
// 处理卡牌点击
void GameController::handlePlayfieldCardClick(int cardId) {
    // 1. 检查是否可以匹配
    if (!canMatchWithTray(cardId)) {
        return;
    }
    
    // 2. 记录撤销操作
    _undoManager->recordAction(action);
    
    // 3. 更新Model
    _gameModel->removeFromPlayfield(cardId);
    
    // 4. 播放动画
    _gameView->playMatchAnimation(cardId, trayId);
}
```

#### 5. `managers/` - 管理器层

**职责**: 提供全局性服务，管理特定功能的数据生命周期

**核心类**:
- `UndoManager`: 撤销功能管理器

**特性**:
- 作为 Controller 的成员变量
- 可持有 Model 数据并进行加工
- **禁止**实现为单例模式
- **禁止**反向依赖 Controller
- 通过回调接口与其他模块交互

**示例**:
```cpp
// 在Controller中使用Manager
_undoManager = new UndoManager();
_undoManager->init(_undoModel);
_undoManager->setUndoAnimationCallback([this](const UndoAction& action) {
    // 播放撤销动画
});
```

#### 6. `services/` - 服务层

**职责**: 提供无状态的业务逻辑服务，不管理数据生命周期

**核心类**:
- `GameModelFromLevelGenerator`: 将静态 LevelConfig 转换为动态 GameModel

**特性**:
- **无状态**：不持有数据
- 通过参数操作数据或返回数据
- 可实现为单例或提供静态方法
- 实现可复用的通用功能

**示例**:
```cpp
// 从关卡配置生成游戏数据
GameModel* gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
```

#### 7. `utils/` - 工具层

**职责**: 提供通用的辅助功能

**核心类**:
- `CardDefines`: 卡牌相关的枚举定义（花色、牌面、位置）

**特性**:
- 不涉及业务逻辑
- 完全独立

---

## 核心功能实现

### 1. 卡牌匹配逻辑

**规则**: 点击主牌区的卡牌，如果其数值与底牌差值为1，则可以匹配

**实现**:
```cpp
bool CardModel::canMatchWith(const CardModel* otherCard) const {
    int thisValue = getFaceValue();   // A=1, 2=2, ..., K=13
    int otherValue = otherCard->getFaceValue();
    return std::abs(thisValue - otherValue) == 1;  // 数值差为1
}
```

**流程**:
1. 用户点击主牌区卡牌 → `CardView` 捕获触摸事件
2. `CardView` 调用点击回调 → 通知 `GameController`
3. `GameController` 检查匹配规则 → `canMatchWithTray()`
4. 如果匹配成功:
   - 记录撤销操作 → `UndoManager::recordAction()`
   - 更新数据 → `GameModel::removeFromPlayfield()`
   - 播放动画 → `GameView::playMatchAnimation()`

### 2. 撤销功能

**实现要点**: 记录每次操作的完整数据，撤销时反向执行

**数据结构**:
```cpp
struct UndoAction {
    UndoActionType type;          // 操作类型
    int fromCardId;               // 源卡牌ID
    int toCardId;                 // 目标卡牌ID
    Vec2 fromPosition;            // 源位置
    Vec2 toPosition;              // 目标位置
};
```

**支持的操作类型**:
- `UAT_REPLACE_TRAY_FROM_STACK`: 从备用牌堆翻牌
- `UAT_REPLACE_TRAY_FROM_PLAYFIELD`: 从主牌区匹配消除

**流程**:
1. 执行操作前，记录当前状态到 `UndoModel`
2. 点击撤销按钮 → `UndoManager::performUndo()`
3. 弹出最后一条操作记录，反向修改 `GameModel`
4. 播放撤销动画（卡牌移回原位置）

### 3. 动画系统

所有动画由 `View` 层实现，通过 Cocos2d-x 的 Action 系统

**卡牌移动动画**:
```cpp
void CardView::moveToPosition(const Vec2& targetPosition, float duration) {
    auto moveTo = MoveTo::create(duration, targetPosition);
    this->runAction(moveTo);
}
```

**匹配动画**:
```cpp
void GameView::playMatchAnimation(int fromCardId, int toCardId) {
    CardView* fromCard = getCardView(fromCardId);
    CardView* toCard = getCardView(toCardId);
    Vec2 targetPos = toCard->getPosition();
    fromCard->moveToPosition(targetPos, 0.3f);
}
```

---

## 数据流与通信流程

### 用户交互流程

```
用户点击卡牌
    ↓
CardView 捕获触摸事件
    ↓
CardView 调用 _clickCallback(cardId)
    ↓
GameController::handleCardClick(cardId)
    ↓
检查业务逻辑（匹配规则）
    ↓
记录撤销操作（UndoManager）
    ↓
更新 GameModel 数据
    ↓
调用 GameView 播放动画
```

### 游戏初始化流程

```
用户选择关卡（levelId）
    ↓
GameController::startGame(levelId)
    ↓
LevelConfigLoader::loadLevelConfig(levelId) 加载JSON配置
    ↓
GameModelFromLevelGenerator::generateGameModel() 生成运行时数据
    ↓
GameView::create() 创建视图
    ↓
GameView::initGameView(gameModel) 初始化UI
    ↓
设置各种回调函数
    ↓
游戏开始
```

---

## 扩展性设计

### 如何添加新卡牌？

1. **更新配置文件** (`Resources/level/level_X.json`)
   ```json
   {
       "CardFace": 5,      // 新卡牌的牌面（0-12）
       "CardSuit": 2,      // 新卡牌的花色（0-3）
       "Position": {"x": 400, "y": 900}
   }
   ```

2. **无需修改代码** - 系统会自动：
   - `LevelConfigLoader` 解析配置
   - `GameModelFromLevelGenerator` 生成 `CardModel`
   - `GameView` 创建 `CardView` 并显示

### 如何添加新的撤销功能类型？

1. **定义新的操作类型** (`models/UndoModel.h`)
   ```cpp
   enum UndoActionType {
       UAT_REPLACE_TRAY_FROM_STACK,
       UAT_REPLACE_TRAY_FROM_PLAYFIELD,
       UAT_YOUR_NEW_ACTION,  // 新增
   };
   ```

2. **在 UndoManager 中实现撤销逻辑** (`managers/UndoManager.cpp`)
   ```cpp
   void UndoManager::performUndo(GameModel* gameModel) {
       UndoAction action = _undoModel->popAction();
       switch (action.type) {
           case UAT_YOUR_NEW_ACTION:
               undoYourNewAction(action, gameModel);
               break;
       }
   }
   
   void UndoManager::undoYourNewAction(const UndoAction& action, GameModel* gameModel) {
       // 实现具体的撤销逻辑
   }
   ```

3. **在 Controller 中记录操作**
   ```cpp
   void GameController::yourNewOperation() {
       UndoAction action;
       action.type = UAT_YOUR_NEW_ACTION;
       // 设置其他字段
       _undoManager->recordAction(action);
   }
   ```

### 如何添加新关卡？

1. 在 `Resources/level/` 目录下创建 `level_X.json`
2. 按照格式填写配置
3. 调用 `GameController::startGame(X, parentNode)` 加载关卡

---

## 编码规范

### 命名规范

- **类名和文件名**: 大写字母开头（`CardModel.h`）
- **函数名和变量名**: 驼峰风格，小写字母开头（`getCardById`）
- **私有成员**: 以 `_` 下划线开头（`_gameModel`）
- **常量**: 以小写 `k` 开头（`kDesignWidth`）


### 模块职责明确

- **Models**: 只存储数据，不包含复杂业务逻辑
- **Views**: 只负责显示和接收输入，不包含业务逻辑
- **Controllers**: 协调 Model 和 View，实现业务逻辑
- **Managers**: 管理数据生命周期，不反向依赖 Controller
- **Services**: 无状态，不持有数据

---

## 构建与运行

### 前置要求

- Cocos2d-x 3.17
- Visual Studio 2015+ (Windows)
- CMake
- Python 2.7

### 运行步骤

1. 确保 Cocos2d-x 3.17 已正确安装
2. 打开项目目录
3. 使用 Visual Studio 或命令行编译
4. 运行程序

---

🟢 绿色 = 梅花 ♣
🔴 红色 = 方块 ♦
🩷 粉色 = 红桃 ♥
🔵 蓝色 = 黑桃 ♠

