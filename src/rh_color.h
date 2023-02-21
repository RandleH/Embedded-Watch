#ifndef _RH_COLOR_H
#define _RH_COLOR_H

#include "rh_common.h"
#include "rh_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RH_COLOR_RECORD_VERSION            "RH_COLOR - 08/18/2021"

#if ( RH_CFG_GRAPHIC_COLOR_TYPE == RH_CFG_GRAPHIC_COLOR_BIN    )
  #define REVERSE_COLOR( M_COLOR )         (((M_COLOR)==0)?(0xff):(0x00))
  
  #define COLOR_MASK_RED                   0xff
  #define COLOR_MASK_GREEN                 0xff
  #define COLOR_MASK_BLUE                  0xff
  
  #define DARKEN_COLOR_1Bit(C)             (uint8_t)((C)&0)
  #define DARKEN_COLOR_2Bit(C)             (uint8_t)((C)&0)

  #define RH_COLOR_MAKE(R_255,G_255,B_255)    (uint8_t)(((R_255+G_255+B_255)/3 > 128)?0xff:0x00)
  #define RH_COLOR_HEX(x)                  ((x)==0)?((uint8_t)(0x00)):((uint8_t)(0xff))
  
#elif ( RH_CFG_GRAPHIC_COLOR_TYPE == RH_CFG_GRAPHIC_COLOR_RGB565    )
  
  #define COLOR_MASK_RED                   0xF800
  #define COLOR_MASK_GREEN                 0x7E00
  #define COLOR_MASK_BLUE                  0x001F
  
  #define REVERSE_COLOR( M_COLOR )         (uint16_t)( (0xF800-((M_COLOR)&(0xF800))) | (0x7E00-((M_COLOR)&(0x7E00))) | (0x001F-((M_COLOR)&0x001F)) )
  
  #define DARKEN_COLOR_1Bit(C)             (uint16_t)( ((((C)&COLOR_MASK_RED)>>1)&(COLOR_MASK_RED))|((((C)&COLOR_MASK_GREEN)>>1)&(COLOR_MASK_GREEN))|((((C)&COLOR_MASK_BLUE)>>1)&(COLOR_MASK_BLUE)) )
  #define DARKEN_COLOR_2Bit(C)             (uint16_t)( ((((C)&COLOR_MASK_RED)>>2)&(COLOR_MASK_RED))|((((C)&COLOR_MASK_GREEN)>>2)&(COLOR_MASK_GREEN))|((((C)&COLOR_MASK_BLUE)>>2)&(COLOR_MASK_BLUE)) )

  #define RH_COLOR_MAKE(R_255,G_255,B_255)    (uint16_t)((((R_255)>>3)<<11)|(((G_255)>>2)<<5)|((B_255)>>3))
  #define RH_COLOR_HEX(x)                  (uint16_t)((0xf800&(((x)>>19)<<11)) | (0x07e0&(((x)>>10)<<5 )) | (0x001f&(((x)>>3 ))))

#elif ( RH_CFG_GRAPHIC_COLOR_TYPE == RH_CFG_GRAPHIC_COLOR_RGB888    )

  #define COLOR_MASK_RED                   0x00FF0000
  #define COLOR_MASK_GREEN                 0x0000FF00
  #define COLOR_MASK_BLUE                  0x000000FF
  
  #define REVERSE_COLOR( M_COLOR )         (uint32_t)( (0x00FF0000-((M_COLOR)&(0x00FF0000))) | (0x0000FF00-((M_COLOR)&(0x0000FF00))) | (0x000000FF-((M_COLOR)&0x000000FF)) )

  #define DARKEN_COLOR_1Bit(C)             (uint32_t)( ((((C)&COLOR_MASK_RED)>>1)&(COLOR_MASK_RED))|((((C)&COLOR_MASK_GREEN)>>1)&(COLOR_MASK_GREEN))|((((C)&COLOR_MASK_BLUE)>>1)&(COLOR_MASK_BLUE)) )
  #define DARKEN_COLOR_2Bit(C)             (uint32_t)( ((((C)&COLOR_MASK_RED)>>2)&(COLOR_MASK_RED))|((((C)&COLOR_MASK_GREEN)>>2)&(COLOR_MASK_GREEN))|((((C)&COLOR_MASK_BLUE)>>2)&(COLOR_MASK_BLUE)) )

  #define RH_COLOR_MAKE(R_255,G_255,B_255)    (uint32_t)((((R_255)&0xff)<<16)|(((G_255)&0xff)<<8)|((B_255)&0xff))
  #define RH_COLOR_HEX(x)                  (uint32_t)((x)&0x00ffffff)

#else
  #error "[RH_color]: Unknown color type."
#endif







// Standard
#define M_COLOR_WHITE                        (RH_COLOR_MAKE(255,255,255))  // 白色
#define M_COLOR_BLACK                        (RH_COLOR_MAKE(  0,  0,  0))  // 黑色
           
#define M_COLOR_BLUE                         (RH_COLOR_MAKE(  0,  0,255))  // 蓝色
#define M_COLOR_RED                          (RH_COLOR_MAKE(255,  0,  0))  // 红色
#define M_COLOR_GREEN                        (RH_COLOR_MAKE(  0,255,  0))  // 绿色
#define M_COLOR_YELLOW                       (RH_COLOR_MAKE(255,255,  0))  // 黄色
#define M_COLOR_CYAN                         (RH_COLOR_MAKE(  0,255,255))  // 青色
#define M_COLOR_MAGENTA                      (RH_COLOR_MAKE(255,  0,255))  // 洋紫
           
// Red-Blue Series           
#define M_COLOR_PINK                         (RH_COLOR_MAKE(255,192,203))  // 粉红
#define M_COLOR_CRIMSON                      (RH_COLOR_MAKE(220, 20, 60))  // 猩红
#define M_COLOR_LAVENDERBLUSH                (RH_COLOR_MAKE(255,240,245))  // 苍白紫罗兰红
#define M_COLOR_PALEVIOLATRED                (RH_COLOR_MAKE(219,112,147))  // 羞涩淡紫红
#define M_COLOR_HOTPINK                      (RH_COLOR_MAKE(255,105,180))  // 热情粉红
#define M_COLOR_MEDIUMVIOLATRED              (RH_COLOR_MAKE(199, 21,133))  // 适中紫罗兰
#define M_COLOR_ORCHID                       (RH_COLOR_MAKE(218,112,214))  // 兰花紫
#define M_COLOR_THISTLE                      (RH_COLOR_MAKE(216,191,216))  // 苍紫
#define M_COLOR_PLUM                         (RH_COLOR_MAKE(221,160,221))  // 轻紫
#define M_COLOR_VOILET                       (RH_COLOR_MAKE(218,112,214))  // 紫罗兰
#define M_COLOR_DARKVOILET                   (RH_COLOR_MAKE(255,  0,255))  // 紫红
#define M_COLOR_PURPLE                       (RH_COLOR_MAKE(128,  0,128))  // 紫
#define M_COLOR_MEDIUMORCHID                 (RH_COLOR_MAKE(255,  0,255))  // 适中兰花紫
#define M_COLOR_DARKVIOLET                   (RH_COLOR_MAKE(148,  0,211))  // 深紫罗兰
#define M_COLOR_INDIGO                       (RH_COLOR_MAKE( 75,  0,130))  // 靓青
#define M_COLOR_BLUEVIOLET                   (RH_COLOR_MAKE(138, 43,226))  // 蓝紫罗兰
#define M_COLOR_MEDIUMPURPLE                 (RH_COLOR_MAKE(147,112,219))  // 适中紫
#define M_COLOR_MEDIUMSLATEBLUE              (RH_COLOR_MAKE(123,104,238))  // 适中板岩蓝
#define M_COLOR_SLATEBLUE                    (RH_COLOR_MAKE(106, 90,205))  // 板岩蓝
#define M_COLOR_DARKSLATEBLUE                (RH_COLOR_MAKE( 72, 61,139))  // 深板岩蓝
#define M_COLOR_LAVENDER                     (RH_COLOR_MAKE(230,230,250))  // 薰衣草淡
#define M_COLOR_GHOSTWHITE                   (RH_COLOR_MAKE(248,248,255))  // 幽灵白
           
// Blue-Green Series           
#define M_COLOR_MEDIUMBLUE                   (RH_COLOR_MAKE(  0,  0,205))  // 适中蓝
#define M_COLOR_MIDNIGHTBLUE                 (RH_COLOR_MAKE( 25, 25,112))  // 午夜蓝
#define M_COLOR_DARKBLUE                     (RH_COLOR_MAKE(  0,  0,139))  // 深蓝
#define M_COLOR_NAVY                         (RH_COLOR_MAKE(  0,  0,128))  // 海军蓝
#define M_COLOR_ROYALBLUE                    (RH_COLOR_MAKE( 65,105,225))  // 皇家蓝
#define M_COLOR_CORNFLOWERBLUE               (RH_COLOR_MAKE(100,149,237))  // 矢车菊蓝
#define M_COLOR_LIGHTSTEELBLUE               (RH_COLOR_MAKE(176,196,222))  // 淡钢蓝
#define M_COLOR_LIGHTSLATEGRAY               (RH_COLOR_MAKE(119,136,153))  // 浅板岩灰
#define M_COLOR_SLATEGRAY                    (RH_COLOR_MAKE(112,128,144))  // 石板灰
#define M_COLOR_DODGERBLUE                   (RH_COLOR_MAKE( 30,114,255))  // 道奇蓝
#define M_COLOR_ALICEBLUE                    (RH_COLOR_MAKE(240,248,255))  // 爱丽丝蓝
#define M_COLOR_STEELBLUE                    (RH_COLOR_MAKE( 70,130,180))  // 钢蓝
#define M_COLOR_LIGHTSKYBLUE                 (RH_COLOR_MAKE(135,206,250))  // 淡天蓝
#define M_COLOR_SKYBLUE                      (RH_COLOR_MAKE(135,206,235))  // 天蓝
#define M_COLOR_DEEPSKYBLUE                  (RH_COLOR_MAKE(  0,191,255))  // 深天蓝
#define M_COLOR_LIGHTBLUE                    (RH_COLOR_MAKE(173,216,230))  // 淡蓝
#define M_COLOR_POWDERBLUE                   (RH_COLOR_MAKE(176,224,230))  // 火药蓝
#define M_COLOR_CADETBLUE                    (RH_COLOR_MAKE( 95,158,160))  // 军校蓝
#define M_COLOR_AZURE                        (RH_COLOR_MAKE(245,255,255))  // 蔚蓝
#define M_COLOR_LIGHTCYAN                    (RH_COLOR_MAKE(240,255,255))  // 淡青
#define M_COLOR_PALETURQUOISE                (RH_COLOR_MAKE(175,238,238))  // 苍白宝石绿
#define M_COLOR_AQUA                         (RH_COLOR_MAKE(  0,255,255))  // 水绿
#define M_COLOR_DARKTURQUOISE                (RH_COLOR_MAKE(  0,206,209))  // 深宝石绿
#define M_COLOR_DARKSLATEGRAY                (RH_COLOR_MAKE( 47, 79, 79))  // 深石板灰
#define M_COLOR_DARKCYAN                     (RH_COLOR_MAKE(  0,139,139))  // 深青色
#define M_COLOR_TEAL                         (RH_COLOR_MAKE(  0,128,128))  // 水鸭色
#define M_COLOR_MEDIUMTURQUOISE              (RH_COLOR_MAKE( 72,209,204))  // 适中宝石绿
#define M_COLOR_LIGHTSEEGREEN                (RH_COLOR_MAKE( 32,178,170))  // 浅海样绿
#define M_COLOR_TURQUOISE                    (RH_COLOR_MAKE( 64,224,208))  // 宝石绿
#define M_COLOR_AQUAMARINE                   (RH_COLOR_MAKE(127,255,212))  // 碧绿
#define M_COLOR_MEDIUMAQUAMARINE             (RH_COLOR_MAKE(102,205,170))  // 适中碧绿
#define M_COLOR_MEDIUMSPRINGGREEN            (RH_COLOR_MAKE(  0,250,154))  // 适中春天绿
#define M_COLOR_SPRINGGREEN                  (RH_COLOR_MAKE(  0,255,127))  // 春天绿
#define M_COLOR_MEDIUMSEEGREEN               (RH_COLOR_MAKE( 60,179,113))  // 适中海洋绿
#define M_COLOR_SEEGREEN                     (RH_COLOR_MAKE( 46,139, 87))  // 海洋绿
#define M_COLOR_LIGHTGREEN                   (RH_COLOR_MAKE(144,238,144))  // 浅绿
#define M_COLOR_PALEGREEN                    (RH_COLOR_MAKE(152,251,152))  // 苍白绿
#define M_COLOR_DARKSEEGREEN                 (RH_COLOR_MAKE(143,188,143))  // 深海洋绿
#define M_COLOR_LIME                         (RH_COLOR_MAKE( 50,205, 50))  // 莱姆色
#define M_COLOR_CHARTREUSE                   (RH_COLOR_MAKE(127,255,  0))  // 查特酒绿
           
// Green-RED Series           
#define M_COLOR_FORESTGREEN                  (RH_COLOR_MAKE( 34,139, 34))  // 森林绿
#define M_COLOR_LAWNGREEN                    (RH_COLOR_MAKE(124,252,  0))  // 草坪绿
#define M_COLOR_GREENYELLOW                  (RH_COLOR_MAKE(173,255, 47))  // 绿黄
#define M_COLOR_DARKOLIVEGREEN               (RH_COLOR_MAKE( 85,107, 47))  // 深橄榄绿
#define M_COLOR_YELLOWGREEN                  (RH_COLOR_MAKE(154,205, 50))  // 黄绿
#define M_COLOR_OLIVEDRAB                    (RH_COLOR_MAKE( 34,139, 34))  // 橄榄褐
#define M_COLOR_BEIGE                        (RH_COLOR_MAKE(245,245,220))  // 米色
#define M_COLOR_LIGHTRODYELLOW               (RH_COLOR_MAKE( 34,139, 34))  // 浅秋黄
#define M_COLOR_IVORY                        (RH_COLOR_MAKE(255,255,240))  // 象牙白
#define M_COLOR_OLIVE                        (RH_COLOR_MAKE(128,128,  0))  // 橄榄
#define M_COLOR_DARKKHAKI                    (RH_COLOR_MAKE(189,183,107))  // 深卡其布
#define M_COLOR_LEMONCHIFFON                 (RH_COLOR_MAKE(255,250,205))  // 柠檬沙
#define M_COLOR_PALEGOLDROD                  (RH_COLOR_MAKE(238,232,170))  // 灰秋
#define M_COLOR_KHAKI                        (RH_COLOR_MAKE(240,230,140))  // 卡其布
#define M_COLOR_GOLDEN                       (RH_COLOR_MAKE(255,215,  0))  // 金色
#define M_COLOR_CORNMILK                     (RH_COLOR_MAKE(255,248,220))  // 玉米
#define M_COLOR_GOLDROD                      (RH_COLOR_MAKE(218,165, 32))  // 秋天
#define M_COLOR_DARKGOLDROD                  (RH_COLOR_MAKE(184,134, 11))  // 深秋
#define M_COLOR_FLORALWHITE                  (RH_COLOR_MAKE(255,250,240))  // 白花
#define M_COLOR_OLDLACE                      (RH_COLOR_MAKE(253,245,230))  // 浅米色
#define M_COLOR_WHEAT                        (RH_COLOR_MAKE(245,222,179))  // 小麦
#define M_COLOR_MOCCASIN                     (RH_COLOR_MAKE(255,228,181))  // 鹿皮
#define M_COLOR_ORANGE                       (RH_COLOR_MAKE(255,165,  0))  // 橙色
#define M_COLOR_PAPAYAWHIP                   (RH_COLOR_MAKE(255,239,213))  // 木瓜
#define M_COLOR_BLANCHEDALMOND               (RH_COLOR_MAKE(255,235,205))  // 漂白的杏仁
#define M_COLOR_NAVAJOWHITE                  (RH_COLOR_MAKE(255,222,173))  // 耐而节白
#define M_COLOR_ANTIQUEWHITE                 (RH_COLOR_MAKE(250,235,215))  // 古白
#define M_COLOR_TAN                          (RH_COLOR_MAKE(210,180,140))  // 晒
#define M_COLOR_BURLYWOOD                    (RH_COLOR_MAKE(222,184,135))  // 树干
#define M_COLOR_BISQUE                       (RH_COLOR_MAKE(255,228,196))  // 乳脂
#define M_COLOR_DARKORANGE                   (RH_COLOR_MAKE(255,140,  0))  // 深橙色
#define M_COLOR_LINEN                        (RH_COLOR_MAKE(255,240,230))  // 亚麻
#define M_COLOR_PERU                         (RH_COLOR_MAKE(205,133, 63))  // 秘鲁
#define M_COLOR_SANDYBROWN                   (RH_COLOR_MAKE(244,164, 96))  // 沙棕
#define M_COLOR_CHOCOLATE                    (RH_COLOR_MAKE(210,105, 30))  // 巧克力
#define M_COLOR_SEASHELL                     (RH_COLOR_MAKE(255,245,238))  // 海贝
#define M_COLOR_SIENNA                       (RH_COLOR_MAKE(160, 82, 45))  // 土黄赭
#define M_COLOR_SALMON                       (RH_COLOR_MAKE(255,160,122))  // 三文鱼
#define M_COLOR_CORAL                        (RH_COLOR_MAKE(255,127, 80))  // 珊瑚红
#define M_COLOR_ORANGERED                    (RH_COLOR_MAKE(255, 69,  0))  // 橙红
#define M_COLOR_TOMATO                       (RH_COLOR_MAKE(255, 99, 71))  // 番茄
#define M_COLOR_MISTYROSE                    (RH_COLOR_MAKE(255,228,225))  // 迷雾玫瑰
#define M_COLOR_BLOODYMEAT                   (RH_COLOR_MAKE(250,128,114))  // 鲜肉
#define M_COLOR_LIGHTCORAL                   (RH_COLOR_MAKE(240,128,128))  // 浅珊瑚红
#define M_COLOR_ROSEBROWN                    (RH_COLOR_MAKE(188,143,143))  // 玫瑰棕
#define M_COLOR_INDIANRED                    (RH_COLOR_MAKE(205, 92, 92))  // 浅粉红
#define M_COLOR_BROWN                        (RH_COLOR_MAKE(165, 42, 42))  // 棕色
#define M_COLOR_FIREBRICK                    (RH_COLOR_MAKE(178, 34, 34))  // 火砖
#define M_COLOR_DARKRED                      (RH_COLOR_MAKE(139,  0,  0))  // 深红
#define M_COLOR_MAROON                       (RH_COLOR_MAKE(128,  0,  0))  // 栗色
           
// Neutral Series           
#define M_COLOR_WHITESMOKE                   (RH_COLOR_MAKE(245,245,245))  // 烟白
#define M_COLOR_GAINSBORO                    (RH_COLOR_MAKE(220,220,220))  // 赶死部落
#define M_COLOR_LIGHTGRAY                    (RH_COLOR_MAKE(211,211,211))  // 浅灰
#define M_COLOR_SILVER                       (RH_COLOR_MAKE(192,192,192))  // 银色
#define M_COLOR_DARKGRAY                     (RH_COLOR_MAKE( 73, 73, 73))  // 深灰
#define M_COLOR_DIMGRAY                      (RH_COLOR_MAKE( 54, 54, 54))  // 暗灰

#define M_COLOR_COAL                         (RH_COLOR_MAKE( 34, 35, 34))  // 煤炭黑


#ifdef __cplusplus
}
#endif

#endif



