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
#define M_COLOR_WHITE                        (RH_COLOR_MAKE(255,255,255))  // ??????
#define M_COLOR_BLACK                        (RH_COLOR_MAKE(  0,  0,  0))  // ??????
           
#define M_COLOR_BLUE                         (RH_COLOR_MAKE(  0,  0,255))  // ??????
#define M_COLOR_RED                          (RH_COLOR_MAKE(255,  0,  0))  // ??????
#define M_COLOR_GREEN                        (RH_COLOR_MAKE(  0,255,  0))  // ??????
#define M_COLOR_YELLOW                       (RH_COLOR_MAKE(255,255,  0))  // ??????
#define M_COLOR_CYAN                         (RH_COLOR_MAKE(  0,255,255))  // ??????
#define M_COLOR_MAGENTA                      (RH_COLOR_MAKE(255,  0,255))  // ??????
           
// Red-Blue Series           
#define M_COLOR_PINK                         (RH_COLOR_MAKE(255,192,203))  // ??????
#define M_COLOR_CRIMSON                      (RH_COLOR_MAKE(220, 20, 60))  // ??????
#define M_COLOR_LAVENDERBLUSH                (RH_COLOR_MAKE(255,240,245))  // ??????????????????
#define M_COLOR_PALEVIOLATRED                (RH_COLOR_MAKE(219,112,147))  // ???????????????
#define M_COLOR_HOTPINK                      (RH_COLOR_MAKE(255,105,180))  // ????????????
#define M_COLOR_MEDIUMVIOLATRED              (RH_COLOR_MAKE(199, 21,133))  // ???????????????
#define M_COLOR_ORCHID                       (RH_COLOR_MAKE(218,112,214))  // ?????????
#define M_COLOR_THISTLE                      (RH_COLOR_MAKE(216,191,216))  // ??????
#define M_COLOR_PLUM                         (RH_COLOR_MAKE(221,160,221))  // ??????
#define M_COLOR_VOILET                       (RH_COLOR_MAKE(218,112,214))  // ?????????
#define M_COLOR_DARKVOILET                   (RH_COLOR_MAKE(255,  0,255))  // ??????
#define M_COLOR_PURPLE                       (RH_COLOR_MAKE(128,  0,128))  // ???
#define M_COLOR_MEDIUMORCHID                 (RH_COLOR_MAKE(255,  0,255))  // ???????????????
#define M_COLOR_DARKVIOLET                   (RH_COLOR_MAKE(148,  0,211))  // ????????????
#define M_COLOR_INDIGO                       (RH_COLOR_MAKE( 75,  0,130))  // ??????
#define M_COLOR_BLUEVIOLET                   (RH_COLOR_MAKE(138, 43,226))  // ????????????
#define M_COLOR_MEDIUMPURPLE                 (RH_COLOR_MAKE(147,112,219))  // ?????????
#define M_COLOR_MEDIUMSLATEBLUE              (RH_COLOR_MAKE(123,104,238))  // ???????????????
#define M_COLOR_SLATEBLUE                    (RH_COLOR_MAKE(106, 90,205))  // ?????????
#define M_COLOR_DARKSLATEBLUE                (RH_COLOR_MAKE( 72, 61,139))  // ????????????
#define M_COLOR_LAVENDER                     (RH_COLOR_MAKE(230,230,250))  // ????????????
#define M_COLOR_GHOSTWHITE                   (RH_COLOR_MAKE(248,248,255))  // ?????????
           
// Blue-Green Series           
#define M_COLOR_MEDIUMBLUE                   (RH_COLOR_MAKE(  0,  0,205))  // ?????????
#define M_COLOR_MIDNIGHTBLUE                 (RH_COLOR_MAKE( 25, 25,112))  // ?????????
#define M_COLOR_DARKBLUE                     (RH_COLOR_MAKE(  0,  0,139))  // ??????
#define M_COLOR_NAVY                         (RH_COLOR_MAKE(  0,  0,128))  // ?????????
#define M_COLOR_ROYALBLUE                    (RH_COLOR_MAKE( 65,105,225))  // ?????????
#define M_COLOR_CORNFLOWERBLUE               (RH_COLOR_MAKE(100,149,237))  // ????????????
#define M_COLOR_LIGHTSTEELBLUE               (RH_COLOR_MAKE(176,196,222))  // ?????????
#define M_COLOR_LIGHTSLATEGRAY               (RH_COLOR_MAKE(119,136,153))  // ????????????
#define M_COLOR_SLATEGRAY                    (RH_COLOR_MAKE(112,128,144))  // ?????????
#define M_COLOR_DODGERBLUE                   (RH_COLOR_MAKE( 30,114,255))  // ?????????
#define M_COLOR_ALICEBLUE                    (RH_COLOR_MAKE(240,248,255))  // ????????????
#define M_COLOR_STEELBLUE                    (RH_COLOR_MAKE( 70,130,180))  // ??????
#define M_COLOR_LIGHTSKYBLUE                 (RH_COLOR_MAKE(135,206,250))  // ?????????
#define M_COLOR_SKYBLUE                      (RH_COLOR_MAKE(135,206,235))  // ??????
#define M_COLOR_DEEPSKYBLUE                  (RH_COLOR_MAKE(  0,191,255))  // ?????????
#define M_COLOR_LIGHTBLUE                    (RH_COLOR_MAKE(173,216,230))  // ??????
#define M_COLOR_POWDERBLUE                   (RH_COLOR_MAKE(176,224,230))  // ?????????
#define M_COLOR_CADETBLUE                    (RH_COLOR_MAKE( 95,158,160))  // ?????????
#define M_COLOR_AZURE                        (RH_COLOR_MAKE(245,255,255))  // ??????
#define M_COLOR_LIGHTCYAN                    (RH_COLOR_MAKE(240,255,255))  // ??????
#define M_COLOR_PALETURQUOISE                (RH_COLOR_MAKE(175,238,238))  // ???????????????
#define M_COLOR_AQUA                         (RH_COLOR_MAKE(  0,255,255))  // ??????
#define M_COLOR_DARKTURQUOISE                (RH_COLOR_MAKE(  0,206,209))  // ????????????
#define M_COLOR_DARKSLATEGRAY                (RH_COLOR_MAKE( 47, 79, 79))  // ????????????
#define M_COLOR_DARKCYAN                     (RH_COLOR_MAKE(  0,139,139))  // ?????????
#define M_COLOR_TEAL                         (RH_COLOR_MAKE(  0,128,128))  // ?????????
#define M_COLOR_MEDIUMTURQUOISE              (RH_COLOR_MAKE( 72,209,204))  // ???????????????
#define M_COLOR_LIGHTSEEGREEN                (RH_COLOR_MAKE( 32,178,170))  // ????????????
#define M_COLOR_TURQUOISE                    (RH_COLOR_MAKE( 64,224,208))  // ?????????
#define M_COLOR_AQUAMARINE                   (RH_COLOR_MAKE(127,255,212))  // ??????
#define M_COLOR_MEDIUMAQUAMARINE             (RH_COLOR_MAKE(102,205,170))  // ????????????
#define M_COLOR_MEDIUMSPRINGGREEN            (RH_COLOR_MAKE(  0,250,154))  // ???????????????
#define M_COLOR_SPRINGGREEN                  (RH_COLOR_MAKE(  0,255,127))  // ?????????
#define M_COLOR_MEDIUMSEEGREEN               (RH_COLOR_MAKE( 60,179,113))  // ???????????????
#define M_COLOR_SEEGREEN                     (RH_COLOR_MAKE( 46,139, 87))  // ?????????
#define M_COLOR_LIGHTGREEN                   (RH_COLOR_MAKE(144,238,144))  // ??????
#define M_COLOR_PALEGREEN                    (RH_COLOR_MAKE(152,251,152))  // ?????????
#define M_COLOR_DARKSEEGREEN                 (RH_COLOR_MAKE(143,188,143))  // ????????????
#define M_COLOR_LIME                         (RH_COLOR_MAKE( 50,205, 50))  // ?????????
#define M_COLOR_CHARTREUSE                   (RH_COLOR_MAKE(127,255,  0))  // ????????????
           
// Green-RED Series           
#define M_COLOR_FORESTGREEN                  (RH_COLOR_MAKE( 34,139, 34))  // ?????????
#define M_COLOR_LAWNGREEN                    (RH_COLOR_MAKE(124,252,  0))  // ?????????
#define M_COLOR_GREENYELLOW                  (RH_COLOR_MAKE(173,255, 47))  // ??????
#define M_COLOR_DARKOLIVEGREEN               (RH_COLOR_MAKE( 85,107, 47))  // ????????????
#define M_COLOR_YELLOWGREEN                  (RH_COLOR_MAKE(154,205, 50))  // ??????
#define M_COLOR_OLIVEDRAB                    (RH_COLOR_MAKE( 34,139, 34))  // ?????????
#define M_COLOR_BEIGE                        (RH_COLOR_MAKE(245,245,220))  // ??????
#define M_COLOR_LIGHTRODYELLOW               (RH_COLOR_MAKE( 34,139, 34))  // ?????????
#define M_COLOR_IVORY                        (RH_COLOR_MAKE(255,255,240))  // ?????????
#define M_COLOR_OLIVE                        (RH_COLOR_MAKE(128,128,  0))  // ??????
#define M_COLOR_DARKKHAKI                    (RH_COLOR_MAKE(189,183,107))  // ????????????
#define M_COLOR_LEMONCHIFFON                 (RH_COLOR_MAKE(255,250,205))  // ?????????
#define M_COLOR_PALEGOLDROD                  (RH_COLOR_MAKE(238,232,170))  // ??????
#define M_COLOR_KHAKI                        (RH_COLOR_MAKE(240,230,140))  // ?????????
#define M_COLOR_GOLDEN                       (RH_COLOR_MAKE(255,215,  0))  // ??????
#define M_COLOR_CORNMILK                     (RH_COLOR_MAKE(255,248,220))  // ??????
#define M_COLOR_GOLDROD                      (RH_COLOR_MAKE(218,165, 32))  // ??????
#define M_COLOR_DARKGOLDROD                  (RH_COLOR_MAKE(184,134, 11))  // ??????
#define M_COLOR_FLORALWHITE                  (RH_COLOR_MAKE(255,250,240))  // ??????
#define M_COLOR_OLDLACE                      (RH_COLOR_MAKE(253,245,230))  // ?????????
#define M_COLOR_WHEAT                        (RH_COLOR_MAKE(245,222,179))  // ??????
#define M_COLOR_MOCCASIN                     (RH_COLOR_MAKE(255,228,181))  // ??????
#define M_COLOR_ORANGE                       (RH_COLOR_MAKE(255,165,  0))  // ??????
#define M_COLOR_PAPAYAWHIP                   (RH_COLOR_MAKE(255,239,213))  // ??????
#define M_COLOR_BLANCHEDALMOND               (RH_COLOR_MAKE(255,235,205))  // ???????????????
#define M_COLOR_NAVAJOWHITE                  (RH_COLOR_MAKE(255,222,173))  // ????????????
#define M_COLOR_ANTIQUEWHITE                 (RH_COLOR_MAKE(250,235,215))  // ??????
#define M_COLOR_TAN                          (RH_COLOR_MAKE(210,180,140))  // ???
#define M_COLOR_BURLYWOOD                    (RH_COLOR_MAKE(222,184,135))  // ??????
#define M_COLOR_BISQUE                       (RH_COLOR_MAKE(255,228,196))  // ??????
#define M_COLOR_DARKORANGE                   (RH_COLOR_MAKE(255,140,  0))  // ?????????
#define M_COLOR_LINEN                        (RH_COLOR_MAKE(255,240,230))  // ??????
#define M_COLOR_PERU                         (RH_COLOR_MAKE(205,133, 63))  // ??????
#define M_COLOR_SANDYBROWN                   (RH_COLOR_MAKE(244,164, 96))  // ??????
#define M_COLOR_CHOCOLATE                    (RH_COLOR_MAKE(210,105, 30))  // ?????????
#define M_COLOR_SEASHELL                     (RH_COLOR_MAKE(255,245,238))  // ??????
#define M_COLOR_SIENNA                       (RH_COLOR_MAKE(160, 82, 45))  // ?????????
#define M_COLOR_SALMON                       (RH_COLOR_MAKE(255,160,122))  // ?????????
#define M_COLOR_CORAL                        (RH_COLOR_MAKE(255,127, 80))  // ?????????
#define M_COLOR_ORANGERED                    (RH_COLOR_MAKE(255, 69,  0))  // ??????
#define M_COLOR_TOMATO                       (RH_COLOR_MAKE(255, 99, 71))  // ??????
#define M_COLOR_MISTYROSE                    (RH_COLOR_MAKE(255,228,225))  // ????????????
#define M_COLOR_BLOODYMEAT                   (RH_COLOR_MAKE(250,128,114))  // ??????
#define M_COLOR_LIGHTCORAL                   (RH_COLOR_MAKE(240,128,128))  // ????????????
#define M_COLOR_ROSEBROWN                    (RH_COLOR_MAKE(188,143,143))  // ?????????
#define M_COLOR_INDIANRED                    (RH_COLOR_MAKE(205, 92, 92))  // ?????????
#define M_COLOR_BROWN                        (RH_COLOR_MAKE(165, 42, 42))  // ??????
#define M_COLOR_FIREBRICK                    (RH_COLOR_MAKE(178, 34, 34))  // ??????
#define M_COLOR_DARKRED                      (RH_COLOR_MAKE(139,  0,  0))  // ??????
#define M_COLOR_MAROON                       (RH_COLOR_MAKE(128,  0,  0))  // ??????
           
// Neutral Series           
#define M_COLOR_WHITESMOKE                   (RH_COLOR_MAKE(245,245,245))  // ??????
#define M_COLOR_GAINSBORO                    (RH_COLOR_MAKE(220,220,220))  // ????????????
#define M_COLOR_LIGHTGRAY                    (RH_COLOR_MAKE(211,211,211))  // ??????
#define M_COLOR_SILVER                       (RH_COLOR_MAKE(192,192,192))  // ??????
#define M_COLOR_DARKGRAY                     (RH_COLOR_MAKE( 73, 73, 73))  // ??????
#define M_COLOR_DIMGRAY                      (RH_COLOR_MAKE( 54, 54, 54))  // ??????

#define M_COLOR_COAL                         (RH_COLOR_MAKE( 34, 35, 34))  // ?????????


#ifdef __cplusplus
}
#endif

#endif



