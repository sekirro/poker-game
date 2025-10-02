# 构建和运行指南

本文档说明如何编译和运行纸牌游戏项目。

## 前置条件

### 1. 安装 Cocos2d-x 3.17

**下载地址**: http://www.cocos2d-x.org/filedown/cocos2d-x-3.17.2.zip

下载后解压到一个目录，例如：`D:/cocos2d-x-3.17`

### 2. 开发工具（Windows）

- **Visual Studio 2015/2017/2019**
- **CMake 3.6+** (https://cmake.org/download/)

## 方法一：手动创建项目（最简单）⭐

由于我们的代码是基于标准 Cocos2d-x 结构编写的，最简单的方法是：

### 步骤：

1. **使用 Cocos2d-x 创建一个新项目**

```bash
# 在Cocos2d-x根目录下执行（需要Python 2.7）
python setup.py
source ~/.bash_profile  # Mac/Linux
# 或在Windows上配置环境变量

# 创建新项目
cocos new PokerGame -p com.mycompany.poker -l cpp -d D:/Projects
```

2. **替换源代码**

将我们创建的所有文件复制到新项目中：
- 复制 `Classes/` 目录下的所有文件 → 覆盖新项目的 `Classes/`
- 复制 `Resources/` 目录下的所有文件 → 覆盖新项目的 `Resources/`

3. **编译运行**

```bash
cd D:/Projects/PokerGame
cocos compile -p win32
cocos run -p win32
```

---

## 方法二：使用 CMake（需要配置）

### 前提：需要将 Cocos2d-x 源码放在项目目录

1. **下载 Cocos2d-x 3.17 并放在项目根目录**

```bash
cd D:/Desktop/poker
# 将cocos2d-x解压到这里，形成如下结构：
# poker/
# ├── cocos2d/          # Cocos2d-x引擎源码
# ├── Classes/
# ├── Resources/
# └── CMakeLists.txt
```

2. **使用 CMake 生成项目**

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017"  # 根据你的VS版本选择
```

3. **打开并编译**

```bash
# 打开生成的 .sln 文件
start PokerGame.sln

# 或者使用命令行编译
cmake --build . --config Debug
```

---

## 方法三：手动创建 Visual Studio 项目（高级）

### 步骤：

1. **创建空的 Visual Studio C++ 项目**
   - 新建项目 → Visual C++ → 空项目
   - 项目名称：PokerGame

2. **配置项目属性**

   **包含目录** (C/C++ → 常规 → 附加包含目录)：
   ```
   $(COCOS2DX_ROOT)\cocos
   $(COCOS2DX_ROOT)\cocos\2d
   $(COCOS2DX_ROOT)\cocos\base
   $(COCOS2DX_ROOT)\cocos\ui
   $(COCOS2DX_ROOT)\external
   $(ProjectDir)..\Classes
   ```

   **库目录** (链接器 → 常规 → 附加库目录)：
   ```
   $(COCOS2DX_ROOT)\build\Debug.win32
   ```

   **依赖项** (链接器 → 输入 → 附加依赖项)：
   ```
   libcocos2d.lib
   ```

   **预处理器定义**：
   ```
   WIN32
   _WINDOWS
   _USE_MATH_DEFINES
   GL_GLEXT_PROTOTYPES
   COCOS2D_DEBUG=1
   ```

3. **添加源文件**
   - 右键项目 → 添加 → 现有项
   - 添加所有 `.cpp` 文件
   - 添加 `proj.win32/main.cpp`

4. **设置工作目录**
   - 项目属性 → 调试 → 工作目录 → 设置为 `$(ProjectDir)..`

5. **编译运行**

---

## 推荐方案 ⭐

**如果您是初学者，强烈推荐使用方法一**：
1. 使用 `cocos new` 创建标准项目
2. 复制我们的代码文件覆盖
3. 使用 `cocos compile` 和 `cocos run` 运行

这样可以避免复杂的配置问题。

---

## 常见问题

### Q1: 找不到 Cocos2d-x 头文件

**解决**: 设置环境变量 `COCOS2DX_ROOT` 指向 Cocos2d-x 安装目录

### Q2: 编译错误：找不到 rapidjson

**解决**: Cocos2d-x 3.17 自带 rapidjson，确保包含了 `$(COCOS2DX_ROOT)\external`

### Q3: 运行时找不到 DLL

**解决**: 
- 将 `$(COCOS2DX_ROOT)\build\Debug.win32\*.dll` 复制到项目目录
- 或设置工作目录为项目根目录

### Q4: 缺少资源文件

**解决**: 确保 `Resources/level/level_1.json` 存在

---

## 快速验证

运行成功后，您应该看到：
- ✅ 窗口大小：540x1040 (1080x2080缩放0.5)
- ✅ 标题显示 "Poker Card Game"
- ✅ 主牌区显示6张卡牌
- ✅ 底牌堆显示1张卡牌
- ✅ 右下角有 "Undo" 按钮
- ✅ 控制台输出 "GameController: Game started successfully"

---

## 下一步

运行成功后，您可以：
1. 修改 `Resources/level/level_1.json` 调整关卡配置
2. 添加卡牌图片资源到 `Resources/images/cards/`
3. 扩展新功能（参考 README.md 的扩展性设计章节）

---

## 需要帮助？

如果遇到问题，请检查：
1. ✅ Cocos2d-x 3.17 是否正确安装
2. ✅ Visual Studio 是否支持 C++11
3. ✅ 所有源文件是否都添加到项目中
4. ✅ 包含目录和库目录是否正确配置 