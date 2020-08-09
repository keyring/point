# Point

## 渲染
### 图元

- 直线光栅化一般有三种算法：数值微分（DDA），中点画线法，Bresenham算法。Bresenham 使用率最高。

- 圆弧光栅化：中点画圆法

- 三角形由三条直线构成

- 三角形填充有两种方式： `Edge Walking` 与 `Edge Equation`。前者将三角形拆分成平底三角形，然后**逐行扫描求交点并填充两点之间的像素**；后者利用三角形的AABB包围盒，然后对包围盒内逐像素判断是否在三角形内，有**面积法**、**内角和法**、**同向法**、**重心坐标法**，最终达到填充目的。两种方式前者计算量小适合CPU，后者计算量大但逻辑简单可以并行适合GPU。重心坐标还能顺便做插值，所以求一次重心坐标，既确定了像素位置，又能用于插值颜色法线Z-BUFFER等等，一举两得，故当前GPU基本都采用重心坐标。[R1](https://www.zhihu.com/question/48299522/answer/799333394) [R2](https://www.zhihu.com/question/34100482/answer/58424576) [R3](http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture7/triangles.html) [R4](https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage)

- 重心坐标求解可以巧妙的借用**向量叉乘**。（因为公式推导出来重心坐标值与两组向量点乘都为0，表示相互垂直。叉乘正好满足求垂直向量）
- 利用重心坐标做插值也顺便解释了zbuffer来自哪里。
- 现代硬件对Z-Buffer 的优化在于深度测试提前到 光栅化之后，PS之前（原先在PS之后），简称 earlyZ。相应的，大部分渲染器为了利用这个硬件特性，会有一个 depth pre pass（preZ）。所以，preZ技术的出现不是孤立的。
- 向量基本运算：加、减、乘、除、点乘、叉乘（仅3D向量有）、求模、求单位向量

## 存在的问题

- 绘制兔子模型的线框模式时，直接读取三个点绘制三角形会出现重复绘制的情况（三角形边共享问题）
- 三角形扫描填充，求交点的算法待优化。