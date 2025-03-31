#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

#define PIN     23 // LED矩阵的DIN引脚连接到GPIO23
#define ROW    16 // 矩阵的行数
#define COL     16 // 矩阵的列数

int counter = 0 ;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, PIN,
 NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,NEO_GRB + NEO_KHZ800);
void ledMatrix_init() {
  Serial.begin(115200);
  matrix.begin();  // 初始化LED矩阵
  matrix.setBrightness(10);  // 设置亮度为50
  matrix.setTextColor(matrix.Color(0, 255, 255));  // 设置文本颜色为白色
  matrix.setTextWrap(true);  // 设置文本换行
  matrix.setTextSize(1);  // 设置文本大小为1
}

void shoutime(int a,int b)
{
  matrix.clear();  // 清除之前的内容
  matrix.setCursor(2, 0);  // 设置文本起始位置
  if(a/10 == 0)
  {
    matrix.print("0");
    matrix.print(a,10);  // 显示文本
  }
  else
  {
    matrix.print(a,10);  // 显示文本
  }
  matrix.setCursor(2, 9);  // 设置文本起始位置
  if(b/10 == 0)
  {
    matrix.print("0");
    matrix.print(b,10);  // 显示分钟
  }
  else
  {
    matrix.print(b,10);  // 显示分钟
  }


  if(counter >= 50 && counter <= 100)
  {
    matrix.drawPixel(5, 8,matrix.Color(255, 0, 0));
    matrix.drawPixel(5, 7,matrix.Color(255, 0, 0));
    matrix.drawPixel(6, 8,matrix.Color(255, 0, 0));
    matrix.drawPixel(6, 7,matrix.Color(255, 0, 0));
    matrix.drawPixel(9, 8,matrix.Color(255, 0, 0));
    matrix.drawPixel(9, 7,matrix.Color(255, 0, 0));
    matrix.drawPixel(10, 8,matrix.Color(255, 0, 0));
    matrix.drawPixel(10, 7,matrix.Color(255, 0, 0));
    if(counter == 100)
    {
      counter = 0;
    }
  }
  else
  {
    matrix.drawPixel(5, 8,matrix.Color(0, 0, 0));
    matrix.drawPixel(5, 7,matrix.Color(0, 0, 0));
    matrix.drawPixel(6, 8,matrix.Color(0, 0, 0));
    matrix.drawPixel(6, 7,matrix.Color(0, 0, 0));
    matrix.drawPixel(9, 8,matrix.Color(0, 0, 0));
    matrix.drawPixel(9, 7,matrix.Color(0, 0, 0));
    matrix.drawPixel(10, 8,matrix.Color(0, 0, 0));
    matrix.drawPixel(10, 7,matrix.Color(0, 0, 0));
  }

  
  matrix.show();  // 显示更新LED矩阵
  delay(10); // 延迟0.01秒
  counter ++;
}
const uint16_t SANDMAN[16][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1},
  {0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},
  {1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const uint32_t colors_SANDMAN[] = {
  matrix.Color(0, 0, 0),     // 0: 黑色
  matrix.Color (255, 215, 0)     // 1:金色
};
void startpage()
{
  //matrix.clear();  // 清除之前的内容
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // 根据像素图案设置颜色
      matrix.drawPixel(x, y, colors_SANDMAN[SANDMAN[y][x]]);
    }
  }
  matrix.show();  // 更新显示
  //delay(200);
}
//苦力怕
const uint16_t creeperFace[16][16] = {
  {0, 0, 2, 2, 1, 1, 0, 0, 0, 0, 2, 2, 1, 1, 2, 2},
  {0, 0, 2, 2, 1, 1, 0, 0, 0, 0, 2, 2, 1, 1, 2, 2},
  {2, 2, 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2, 0, 0},
  {2, 2, 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2, 0, 0},
  {0, 0, 3, 3, 3, 3, 2, 2, 0, 0, 3, 3, 3, 3, 1, 1},
  {0, 0, 3, 3, 3, 3, 2, 2, 0, 0, 3, 3, 3, 3, 1, 1},
  {1, 1, 3, 3, 3, 3, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2},
  {1, 1, 3, 3, 3, 3, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2},
  {2, 2, 0, 0, 1, 1, 3, 3, 3, 3, 2, 2, 0, 0, 1, 1},
  {2, 2, 0, 0, 1, 1, 3, 3, 3, 3, 2, 2, 0, 0, 1, 1},
  {1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0},
  {1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 0, 0},
  {2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 2, 2},
  {2, 2, 0, 0, 3, 3, 0, 0, 2, 2, 3, 3, 1, 1, 2, 2},
  {0, 0, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 0, 0},
  {0, 0, 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2, 0, 0}
};

//史蒂夫
const uint16_t stive  [16][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 3, 3, 2, 2, 1, 1, 1, 1, 2, 2, 3, 3, 1, 1},
  {1, 1, 3, 3, 2, 2, 1, 1, 1, 1, 2, 2, 3, 3, 1, 1},
  {1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

//spirit
const uint16_t spirit  [16][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0},
  {0, 0, 1, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0},
  {0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0},
  {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
  {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
  {0, 1, 1, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 0},
  {0, 1, 1, 1, 1, 0, 0, 1, 2, 2, 2, 2, 2, 1, 1, 0},
  {0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 3, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 3, 1, 0, 0},
  {0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 3, 3, 3, 3, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const uint32_t colors_spirit[] = {
  matrix.Color(150, 150, 150),       // 0:白（背景）
  matrix.Color(0, 0, 0),     // 1: 黑（苦力怕身体）
  matrix.Color(200, 0, 0),     // 2: 红（苦力怕阴影）
  matrix.Color(90, 90, 90),      // 3:灰 （苦力怕眼睛）
  
};

const uint32_t colors_stive[] = {
  matrix.Color(50, 15, 15),       // 0: 棕色（背景）
  matrix.Color(205, 133, 63),     // 1: 绿色（苦力怕身体）
  matrix.Color(32, 0, 32),     // 2: 紫色（苦力怕阴影）
  matrix.Color(150, 150, 150),      // 3:白色 （苦力怕眼睛）
  
  matrix.Color(100, 30, 30)      // 3:白色 （苦力怕眼睛）
};


const uint32_t colors[] = {
  matrix.Color(0, 200, 90),       // 0: 浅绿（背景）
  matrix.Color(50, 255, 50),     // 1: 绿色（苦力怕身体）
  matrix.Color(0, 128, 0),     // 2: 深绿色（苦力怕阴影）
  matrix.Color(0, 0, 0)      // 3: 红色（苦力怕眼睛）
};
void creeper()
{
  //matrix.clear();  // 清除之前的内容
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // 根据像素图案设置颜色
      matrix.drawPixel(x, y, colors[creeperFace[y][x]]);
    }
  }
  matrix.show();  // 更新显示
  //delay(200);
}

void stive1()
{
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // 根据像素图案设置颜色
      matrix.drawPixel(x, y, colors_stive[stive[y][x]]);
    }
  }
  matrix.show();  // 更新显示
}


void colors_spirit1()
{
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      // 根据像素图案设置颜色
      matrix.drawPixel(x, y, colors_spirit[spirit[y][x]]);
    }
  }
  matrix.show();  // 更新显示
  //delay(200);
}