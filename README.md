# arkarrot-refactor
该项目是对应用软件设计模式中的所有方法进行代码重构的试验，尝试在保证使用了所有的设计模式的前提下保持项目可以运行

具体应用的设计模式说明：

- `Singleton`：单例模式，用于`CSVManager`、`LevelScene`等
- `Decorator`：装饰模式，用于定制子弹效果
- `Composite`：组合模式，用于优化障碍物的创建和销毁逻辑
- `State`：状态模式，用于优化怪物减速逻辑
- `Null-object`：空对象模式，用于优化炮塔索敌判断逻辑

## commit01说明
结合了所有成员的重构代码，并进行了以下调整：

- 将子弹特效处理代码全部提取到了`BulletDecorator`类中，去掉了`explode`函数，不再需要创建`BulletBase`的子类来定制子弹
- 经过研究发现`Observer`模式不适合用于该项目的重构，直接在`BulletBase`类的`update`逻辑中通过`VictimBase`的`isdead`字段判断对象存活情况即可实现子弹的自动爆炸

# arkarrot
保卫萝卜
这是程序设计范式的期末项目
成员：640三剑客 丁宇轩 程翔宇 柴泉庚

哦，对了！要让 `build.win32` 出现，我们需要打开PowerShell，念动奇妙的咒语——

```bash
 cmake -B build.win32 -G "Visual Studio 17 2022" -A win32 -Tv143
```