# MusicPlayer-in-Qt-Plus

**基于Qt的音乐播放器的设计与实现**

- 本项目旨在用Qt实现一个高颜值、高体验的音乐播放器
- 目前使用的音源API来源于NeteaseCloudMusicApi的网易云音乐API，并部署在自己的服务器上，后续考虑添加其他音源。
- 本项目正在并持续更新和维护。

## 项目运行

### 运行环境
win10

### 编辑工具
Qt Creator 9.0.1

### 编译器
MinGW 8.1.0

## 功能和画饼

- [x] 基本的UI框架
- [x] 在线音乐搜索
- [x] 音乐播放
- [x] 歌词显示及滚动
- [ ] 本地音乐播放
- [ ] 我喜欢的音乐和歌单
- [ ] 本地数据存储
- [ ] 云端数据存储

## 界面展示

### 搜索结果页面
![搜索结果页面](https://raw.githubusercontent.com/hhhyxy/MusicPlayer-in-Qt-Plus/main/static/searchResult_Page.png)

#### 功能介绍
- 实现圆角阴影边框、拖动顶部可移动界面，双击可最大化，实现基本界面切换
- 实现整体UI界面，并优化动态交互效果
- 实现音乐搜索功能，搜索结果最大80条，使用**多线程**和**懒加载**方式提升页面加载速度
- 双击列表播放选中音乐，并可以在底部栏控制音乐切换、播放暂停、播放模式、音量
- 点击底部栏空白处可切换到歌词界面

### 歌词页面
![歌词页面](https://raw.githubusercontent.com/hhhyxy/MusicPlayer-in-Qt-Plus/main/static/lrc_Page_gaussionBackground.png)

#### 功能介绍
- 实现歌词实时滚动，可使用鼠标上下拖动歌词
- 点击歌词可更新音乐播放进度
- 实现歌词页面模糊背景，采用高斯模糊的方式模糊专辑图片

### 右键菜单
![右键菜单](https://raw.githubusercontent.com/hhhyxy/MusicPlayer-in-Qt-Plus/main/static/menu_buttonRight.png)

#### 功能介绍
- 鼠标右键菜单可执行额外的操作

## 灵感来源
[YesPlayMusic](https://github.com/qier222/YesPlayMusic)

[lx-music-desktop](https://github.com/lyswhut/lx-music-desktop)

## API来源
[NeteaseCloudMusicApi](https://github.com/Binaryify/NeteaseCloudMusicApi)

## License
[The MIT License (MIT)](https://github.com/hhhyxy/MusicPlayer-in-Qt/edit/main/LICENSE)
