# TODO
### Sturcture
Shader Manager
Sence Manager
Render Manager:管理整体渲染流程
Buffer Manager:

### Sth new
色彩控制：GAMMA校准、HDR高动态范围、
物理模拟：石头的控制（如果实现了，我们可以做有棱角的石头，有刚体碰撞，可以停住）
天空盒
模型加载
打印状态（debug和no debug）：帧率、速度
2DSSR

### Game Interaction
移动
死亡判定
空气墙
获取宝石

### Map
中间四周分布，上下通
地图分布预选项
所有的地图都可以用TRANSFORM FEEDBACK模拟其特定要素

雨天（TRANSFORM FEEDBACK模拟雨点）
草原（TS、GS的应用）
雪地、沙地（displacement texture+TS）
透明迷宫（OIT）
海洋、池塘（SSR，FFT，OIT）

### Code
统一shader参数传递标准

### 具体的任务
- [ ] 整体渲染管线与重构
- [ ] 草场的精细化调试
- [ ] 天空盒技术
- [ ] 最后一个pass的颜色处理
- [ ] 整体地图的搭建
- [ ] 模型加载
- [ ] 状态打印 
- [ ] 物体运动
- [ ] 云
- [ ] 调试OIT渲染中的参数（比如说，视角与水平面贴近的时候反射多透射少）
- [ ] 实现草地、雪地交互