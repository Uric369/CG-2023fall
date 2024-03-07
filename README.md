# SE3306 计算机图形学 Homework2 

## 项目目标
该项目旨在使用OpenGL和Visual Studio 2022模拟小球击打陶瓷兔子，导致兔子碎裂的过程：

- 导入obj模型
- Obj模型的存储和加载
- 相机矩阵变换
- 使用小球绘制线框模型
- 使用着色器生成内部和外部表面模型、断裂截面、断裂模拟以及光照

## 实施细节
1. **导入obj模型**
2. **Obj模型存储与加载**
3. **相机矩阵变换**
4. **线框模型绘制小球**
5. **用shader生成内外表面模型+断裂切面+碎裂仿真+光照**
   - **Geometry Shader**
     - 计算等距面网格
     - 碎裂动画仿真（碎裂、碎片掉落）
     - 断裂切面条带
   - **Fragment Shader**
     - 内外表面模型和断裂切面渲染基本流程
     - 计算等距面网格，生成内表面
     - 小球撞击 + 模型碎裂动画仿真
     - 断裂切面条带
     - 光照 - Phong光照模型
6. **检测碰撞，计算碰撞位置**
