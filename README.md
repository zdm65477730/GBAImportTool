# GBAImportTool
游戏ROM，封面，简介，帮助说明文字和图片添加工具。

## 描述 
可以批量根据当前添加的titles里游戏目录来生成对应的数据文件和语言文件。  
简单说，就是把游戏Rom，图片，配置按游戏编号格式放入应用程序所在文件夹里的titles文件夹里，然后执行应用程序。应用程序就会自动生成所有titles下面所有游戏的数据文件以及bootapp里的语言文件。
剩下的就是把bootapp和titles文件夹放到NS: atmosphere/contents/<APP_ID>/romfs/下就行了。

## 编译方法
- Linux版导入工具编译：

```bash
g++ -std=c++23 -O2 -Wall -pedantic -pthread GBAImportTool.cpp -o GBAImportTool
```

- Windows版导入工具编译：
直接用VS Studio导入工程，然后编译。

## 具体操作步骤：
1. 把需要添加的所有游戏资源（ROM和封面，简介图片，联机教程图片）按照如下要求放入titles文件夹新建游戏ROM文件夹里，规则为：

···
gbaTools
├── GBAImportTool.exe #Windows主程序
├── Readme.md         #说明文件
├── gb                #gb/gbc工作目录，参考模板目录结构
│   └── romfs
│       ├── bootapp
│       │   └── resources
│       │       ├── scenes
│       │       │   └── sys
│       │       │       └── sysmain.scn
│       │       └── sprites
│       │           └── colorhalftone.png
│       └── titles    #gb/gbc工作目录，拷贝模板C-1314_j.json，重命名并复制到这个目录：<ROM_ID>/<ROM_ID>.json
├── gba               #gba工作目录，参考模板目录结构
│   └── romfs
│       ├── bootapp
│       │   └── resources
│       │       ├── scenes
│       │       │   └── sys
│       │       │       └── sysmain.scn
│       │       └── sprites
│       │           └── colorhalftone.png
│       └── titles #gba工作目录，拷贝模板A-1314_j.json，重命名并复制到这个目录：<ROM_ID>/<ROM_ID>.json
├── GBAImportTool     #Linux主程序
└── temple            #模板，勿删，内容作为参考
    ├── gb
    │   └── romfs
    │       ├── bootapp
    │       │   └── resources
    │       │       ├── scenes
    │       │       │   └── sys
    │       │       │       └── sysmain.scn
    │       │       ├── sprites
    │       │       │   └── colorhalftone.png
    │       │       └── strings
    │       │           ├── ja
    │       │           │   └── strings.lng
    │       │           ├── zh-hans
    │       │           │   └── strings.lng
    │       │           └── zh-hant
    │       │               └── strings.lng
    │       └── titles  #gb/gbc ROM模板处理目录
    │           └── C-1314_j
    │               ├── C-1314_j-details.png
    │               ├── C-1314_j.gbc
    │               ├── C-1314_j.json
    │               ├── C-1314_j.png
    │               ├── CONNECT_GUIDE_01_C-1314_j.png
    │               ├── CONNECT_GUIDE_02_C-1314_j.png
    │               └── CONNECT_GUIDE_03_C-1314_j.png
    └── gba
        └── romfs
            ├── bootapp
            │   └── resources
            │       ├── scenes
            │       │   └── sys
            │       │       └── sysmain.scn #屏幕面板配置
            │       ├── sprites
            │       │   └── colorhalftone.png #屏幕面板配置
            │       └── strings #必须模板语言文件，勿删除
            │           ├── ja
            │           │   └── strings.lng
            │           ├── zh-hans
            │           │   └── strings.lng
            │           └── zh-hant
            │               └── strings.lng
            └── titles #gba ROM模板处理目录
                └── A-1314_j #游戏ROM文件夹：<分组ID>-<游戏ID>_<区域>
                    ├── A-1314_j-details.png #简介封面：<分组ID>-<游戏ID>_<区域>-details.png
                    ├── A-1314_j.gba #Rom，<分组ID>-<游戏ID>_<区域>.gba
                    ├── A-1314_j.json #配置模板，基于这个模板修改
                    ├── A-1314_j.png #封面：<分组ID>-<游戏ID>_<区域>.png
                    ├── CONNECT_GUIDE_01_A-1314_j.png #联机图片教程：CONNECT_GUIDE_<编号>_<分组ID>-<游戏ID>_<区域>.png
                    ├── CONNECT_GUIDE_02_A-1314_j.png
                    ├── CONNECT_GUIDE_03_A-1314_j.png
                    ├── CONNECT_GUIDE_04_A-1314_j.png
                    ├── ONECARTRIDGE_GUIDE_01_A-1314_j.png #联机等待图片教程：ONECARTRIDGE_GUIDE_<编号>_<分组ID>-<游戏ID>_<区域>.png
                    ├── ONECARTRIDGE_GUIDE_02_A-1314_j.png
                    ├── ONECARTRIDGE_GUIDE_03_A-1314_j.png
                    └── ONECARTRIDGE_GUIDE_04_A-1314_j.png
···

游戏ID可以参考：[GBA官方游戏全集](https://www.oldmanemu.net/%e6%8e%8c%e6%9c%ba%e6%b8%b8%e6%88%8f/gba/gba%e5%ae%98%e6%96%b9%e6%b8%b8%e6%88%8f%e5%85%a8%e9%9b%86)
GBA游戏信息，包括封面，简介等文字和图片信息参考：[GBA Game DB](https://indienova.com/gamedb/platform/gba/p/1)

2. 修改游戏ROM文件夹里游戏json文件的游戏信息，所有需要添加的游戏都这么操作。
GBA模板json文件说明：

```
{
        "titles": {
                "copyright": "©1992,2001 INTELLIGENT SYSTEMS.", #版权信息
                "lcla6_release_date": "2023-02-01",             #录入日期
                "players_count": 4,                             #游戏人数
                "publisher": "任天堂",                           #发行商
                "release_date": "2001-07-21",                   #发布日期
                "rewind_interval": 1.5,                              
                "save_count": 1,                                #存档数
                "simultaneous": false,                          #同步开启  
                "sort_publisher": "任天堂",                      #发行商
                "sort_title": "任天堂游戏",                      #排序用标题      
                "title": "任天堂ゲーム",                         #游戏标题
                "title_ko": "닌텐도 게임",                       #游戏标题韩语
                "title_zhHans": "任天堂游戏",                    #游戏标题简中
                "title_zhHant": "任天堂遊戲",                    #游戏标题繁中
                "display_version":"v1.0.0",                     #游戏封面右下加显示文字
                "volume": 100                                   #游戏分卷号
        },
        "strings": {
                "META_TITLE_COMMENT_JA": "ゲーム紹介。\n任天堂游戏",   #日文游戏简介，如需换行，注意使用换行符\n
                "META_TITLE_COMMENT_SC": "游戏简介。",                #简体中文游戏简介
                "META_TITLE_COMMENT_TC": "遊戲簡介。",                #繁体中文游戏简介
                "META_TITLE_CONNECT_GUIDE_01_JA": "ステップ1",        #联机步骤1日文，配合联机指引图片
                "META_TITLE_CONNECT_GUIDE_01_SC": "步骤1",            #联机步骤1简体中文
                "META_TITLE_CONNECT_GUIDE_01_TC": "步驟1",            #联机步骤1繁体中文
                "META_TITLE_ONECARTRIDGE_GUIDE_01_JA": "ステップ1",   #等待联机步骤1日文，同上
                "META_TITLE_ONECARTRIDGE_GUIDE_01_SC": "步骤1",       #等待联机步骤1简体中文
                "META_TITLE_ONECARTRIDGE_GUIDE_01_TC": "步驟1"        #等待联机步骤1繁体中文
        }
}
```

GB/GBC模板json文件说明，大部分参考GBA模板json说明：

```
{
        "titles": {
                "CGB-Default": "None",       #GBC默认
                "CGB-Nostalgic": "None",     #GBC怀旧
                "DMG_MGB-Default": "None",   #GB/GBP/GBL默认
                "DMG_MGB-Nostalgic": "High", #GB/GBP/GBL怀旧
                "emu_option": "sh3"          #模拟器选项，部分游戏有指定
        },
        "strings": {
        }
}
```

3. Windows下双击执行: GBAImportTool.exe，或WSL/Linux下执行：./GBAImportTool
   
4. 把gb或者gba目录下romfs目录放到对应NS: atmosphere/contents/<APP_ID>/下即可。
