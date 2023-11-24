# MusicPlayer-in-Qt-Plus

**基于Qt的音乐播放器的设计与实现**

- 本项目旨在用Qt实现一个高颜值、高体验的音乐播放器
- 目前使用的音源API来源于NeteaseCloudMusicApi的网易云音乐API，并部署在自己的服务器上(已挂，可以换上自己的音源地址)，后续考虑添加其他音源。

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
- [x] 搜索提示列表
- [x] 音乐播放控制
- [x] 歌词显示及滚动
- [x] 我喜欢的音乐
- [x] 本地数据存储
- [x] 创建歌单功能
- [ ] 云端数据存储

## 界面展示

### 搜索结果页面
![搜索结果页面](https://raw.githubusercontent.com/hhhyxy/MusicPlayer-in-Qt-Plus/main/static/searchResult_Page.png)

##### 功能介绍
- 实现**圆角阴影**边框、拖动顶部可移动界面，双击可最大化，实现基本界面切换
- 实现整体UI界面，并优化动态交互效果，实现界面切换、加载动画
- 实现音乐搜索功能，搜索结果最大80条，使用**多线程**和**懒加载**方式提升页面加载速度
- 实现**搜索提示列表**功能，并且实现列表展开/收回动画
- 双击列表播放选中音乐，并可以在底部栏控制音乐切换、播放暂停、播放模式、音量
- 点击底部栏空白处可切换到歌词界面，并且有上拉**动画**

### 歌词页面
![歌词页面](https://raw.githubusercontent.com/hhhyxy/MusicPlayer-in-Qt-Plus/main/static/lrc_Page_gaussionBackground.png)

##### 功能介绍
- 实现歌词实时滚动，可使用鼠标上下**拖动**歌词
- 点击歌词可实时更新音乐播放进度
- 实现歌词页面**模糊背景**，采用高斯模糊的方式模糊专辑图片
- 点击左上角下拉按钮回到主界面，并且有下拉**动画**

### 我喜欢的音乐界面

- 界面样式与搜索结果页面一致
- 可以添加歌曲到我喜欢的音乐

### 右键菜单
![右键菜单](https://raw.githubusercontent.com/hhhyxy/MusicPlayer-in-Qt-Plus/main/static/menu_buttonRight.png)

##### 功能介绍
- 鼠标右键菜单可执行额外的操作

### 创建歌单功能

##### 功能介绍
- 可以创建自己的歌单并向其中添加歌曲

## 灵感来源
[YesPlayMusic](https://github.com/qier222/YesPlayMusic)

[lx-music-desktop](https://github.com/lyswhut/lx-music-desktop)

## API来源
[NeteaseCloudMusicApi](https://github.com/Binaryify/NeteaseCloudMusicApi)

## License
[The MIT License (MIT)](https://github.com/hhhyxy/MusicPlayer-in-Qt/edit/main/LICENSE)
