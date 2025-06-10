#include "display_ssd1963.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void Lcd_Write_Cmd(uint16_t Cmd)
{
   *(uint16_t *) (LCD_REG) = Cmd;
}
////////////////////////
void Lcd_Write_Data(uint16_t data)
{
   *(uint16_t *) (LCD_DATA)= data;
}


void LCD_Write_DATA(uint16_t data)
{
	 *(uint16_t *) (LCD_DATA)= data;

}

void LCD_Write_COM(uint16_t data)
{
	*(uint16_t *) (LCD_REG) = data;
}
///////////////////
void Lcd_SetArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey)
{
Lcd_Write_Cmd(SSD1963_SET_COLUMN_ADDRESS);	
Lcd_Write_Data((sx >> 8) & 0xFF);
Lcd_Write_Data((sx >> 0) & 0xFF);
Lcd_Write_Data((ex >> 8) & 0xFF);
Lcd_Write_Data((ex >> 0) & 0xFF);

Lcd_Write_Cmd(SSD1963_SET_PAGE_ADDRESS);	
Lcd_Write_Data((sy >> 8) & 0xFF);
Lcd_Write_Data((sy >> 0) & 0xFF);
Lcd_Write_Data((ey >> 8) & 0xFF);
Lcd_Write_Data((ey >> 0) & 0xFF);
}
//=============================================================================
// Fill area of specified color
//=============================================================================
void Lcd_FillArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, int16_t color)
{
uint16_t i;
Lcd_SetArea(sx, ex, sy, ey);
Lcd_Write_Cmd(SSD1963_WRITE_MEMORY_START);
for(i = 0; i < ((ex-sx+1)*(ey-sy+1)); i++)
	{
	Lcd_Write_Data(color);
	}
}

//=============================================================================
// Fills whole screen specified color
//=============================================================================
void Lcd_ClearScreen(int16_t color)
{
unsigned int x,y;
Lcd_SetArea(0, TFT_WIDTH-1 , 0, TFT_HEIGHT-1);
Lcd_Write_Cmd(0x2c);
for(x=0;x<TFT_WIDTH;x++)
	{
        for(y= 0;y<TFT_HEIGHT;y++)
                {
                Lcd_Write_Data(color);
                }
	}
}

void Lcd_SetPixel(int16_t x, int16_t y, int16_t color)
{
Lcd_SetArea(x, x, y, y);
Lcd_Write_Cmd(0x2c);
Lcd_Write_Data(color);
Lcd_Write_Cmd(0x0);
}


void Init_SSD1963(void)
{

//
//Lcd_Write_Cmd(0xe0); //START PLL
//Lcd_Write_Data(0x01);  //Set bit Enable PLL
//HAL_Delay(50);  //Wait  to let the PLL stable
//
//Lcd_Write_Cmd(0xe0); //START PLL
//Lcd_Write_Data(0x03);  //(0 - Use reference clock as system clock   1 -  Use PLL output as system clock)
//HAL_Delay(5);
//
//Lcd_Write_Cmd(0x01); // Software reset
//HAL_Delay(10);
//
//Lcd_Write_Cmd(0xe2);//Set the PLL
//Lcd_Write_Data(0x1d);
//Lcd_Write_Data(0x02);
//Lcd_Write_Data(0x54);
//
//Lcd_Write_Cmd(0xe6);//Set the LSHIFT (pixel clock) frequency
//Lcd_Write_Data(0x04);
//Lcd_Write_Data(0x6f);
//Lcd_Write_Data(0x47);
//
//Lcd_Write_Cmd(0xf0);//SET pixel data I/F format=8bit
//Lcd_Write_Data(0x03); //pixel data format, 0x03 is 16bit(565 format);0x00 is for 8-bit
////Set the MN of PLL
//
////////cput(0xe2);   //SET PLL freq=113.33MHz ;
////////dput(0x1d); // values set are diff in app note (1D)
////////dput(0x02);   // values set are diff in app note (02)
////////dput(0x54); // values set are diff in app note (54)
////////
////////cput(0xe6);   //SET PCLK freq=33.26MHz ; pixel clock frequency
////////dput(0x04); //01 values set are diff in app note 0x04
////////dput(0x6f); //30 values set are diff in app note 0x6f
////////dput(0x47); //16 values set are diff in app note 0x47
//
//Lcd_Write_Cmd(0xb0);	//LCD SPECIFICATION   SET LCD MODE SET TFT 18Bits MODE
//Lcd_Write_Data(0x20);  //dput(0x08); //SET TFT MODE & hsync+Vsync+DEN MODE
//Lcd_Write_Data(0x80);  //
//Lcd_Write_Data(0x03);//SET horizontal size=800-1 HightByte
//Lcd_Write_Data(0x1f);//1f   //SET horizontal size=800-1 LowByte
//Lcd_Write_Data(0x01);//SET vertical size=480-1 HightByte
//Lcd_Write_Data(0xdf);//df   //SET vertical size=480-1 LowByte
//Lcd_Write_Data(0x00);//   dput(0x2d); //SET even/odd line RGB seq.=RGB
//
////Set front porch and back porch
//Lcd_Write_Cmd(0xb4);// SET HBP
//Lcd_Write_Data(0x04); // SET HSYNC Total = // 0x04 SET HSYNC Total = 1056  // hsync of  value in app note is 8367 & not 1056
//Lcd_Write_Data(0x20);
//Lcd_Write_Data(0x00); // SET HBP = //SET HBP = 256 // HBP of value in app note is 163 & not 256
//Lcd_Write_Data(0x2e);  //SET VBP 0 //  earlier value of VBP = 8 , write data ( 0x07)
//Lcd_Write_Data(0xd2);// SET VBP
//Lcd_Write_Data(0x00);
//Lcd_Write_Data(0x00);// SET Hsync pulse start position
//Lcd_Write_Data(0x00);// SET Hsync pulse subpixel start position
//
//Lcd_Write_Cmd(0xb6);    //SET VBP,
//Lcd_Write_Data(0x02); //0x02 SET Vsync total 526 = 525 + 1  // vsync of  value in app note is 496  & not 525
//Lcd_Write_Data(0x0d);
//Lcd_Write_Data(0x00); //SET VBP = 45                // HBP of value in app note is 4 & not 45
//Lcd_Write_Data(0x17);// 0x2d
//Lcd_Write_Data(0x16);// 0x00 SET Vsync pulse 0     // SET Vsync pulse in app note is 2
//Lcd_Write_Data(0x00); //SET Vsync pulse start position
//Lcd_Write_Data(0x00);
//
//Lcd_Write_Cmd(0x2a);    //SET column address
//Lcd_Write_Data(0x00); //SET start column address=0
//Lcd_Write_Data(0x00);
//Lcd_Write_Data(0x03); //SET end column address=799
//Lcd_Write_Data(0x1f);
//
//Lcd_Write_Cmd(0x2b);;     //SET page address
//Lcd_Write_Data(0x00);//SET start page address=0
//Lcd_Write_Data(0x00);
//Lcd_Write_Data(0x01);//SET end page address=479
//Lcd_Write_Data(0xdf);  //1f
//
//Lcd_Write_Cmd(0x036);  //SET RGB BGR
//Lcd_Write_Data(0x08);  //1 BGR  0  RGB
////Lcd_Write_Cmd(0x36);;     //SET address mode to rotate mode
////Lcd_Write_Data(0x60);
////Lcd_Write_Cmd(0x3a);  //Reserved command ???????????
////Lcd_Write_Data(0x60);
//
//
//////////===========================================
////////Lcd_Write_Cmd(0x33);
////////Lcd_Write_Data(0x00); //SET top fixed area=0
////////Lcd_Write_Data(0xB4);
////////Lcd_Write_Data(0x00); //SET vertical scrolling area=479
////////Lcd_Write_Data(0x94);
////////Lcd_Write_Data(0x00); //SET bottom fixed area=0
////////Lcd_Write_Data(0x98);
//////////===============================================
//
//Lcd_Write_Cmd(0xb8);
//Lcd_Write_Data(0x0f);
//Lcd_Write_Data(0x01);
//
//Lcd_Write_Cmd(0xba);
//Lcd_Write_Data(0x01);
//
//Lcd_Write_Cmd(0x29);// SET display on
//
//Lcd_Write_Cmd(0x2c);



LCD_Write_COM(0xE2);		//PLL multiplier, set PLL clock to 120M
LCD_Write_DATA(0x23);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
LCD_Write_DATA(0x02);
LCD_Write_DATA(0x04);
LCD_Write_COM(0xE0);		// PLL enable
LCD_Write_DATA(0x01);
HAL_Delay(10);
LCD_Write_COM(0xE0);
LCD_Write_DATA(0x03);
HAL_Delay(10);
LCD_Write_COM(0x01);		// software reset
HAL_Delay(100);
LCD_Write_COM(0xE6);		//PLL setting for PCLK, depends on resolution
LCD_Write_DATA(0x04);
LCD_Write_DATA(0x93);
LCD_Write_DATA(0xE0);

LCD_Write_COM(0xB0);		//LCD SPECIFICATION
LCD_Write_DATA(0x00);	// 0x24
LCD_Write_DATA(0x00);
LCD_Write_DATA(0x03);		//Set HDP	799
LCD_Write_DATA(0x1F);
LCD_Write_DATA(0x01);		//Set VDP	479
LCD_Write_DATA(0xDF);
LCD_Write_DATA(0x00);

LCD_Write_COM(0xB4);		//HSYNC
LCD_Write_DATA(0x03);		//Set HT	928
LCD_Write_DATA(0xA0);
LCD_Write_DATA(0x00);		//Set HPS	46
LCD_Write_DATA(0x2E);
LCD_Write_DATA(0x30);		//Set HPW	48
LCD_Write_DATA(0x00);		//Set LPS	15
LCD_Write_DATA(0x0F);
LCD_Write_DATA(0x00);

LCD_Write_COM(0xB6);		//VSYNC
LCD_Write_DATA(0x02);		//Set VT	525
LCD_Write_DATA(0x0D);
LCD_Write_DATA(0x00);		//Set VPS	16
LCD_Write_DATA(0x10);
LCD_Write_DATA(0x10);		//Set VPW	16
LCD_Write_DATA(0x00);		//Set FPS	8
LCD_Write_DATA(0x08);

LCD_Write_COM(0xBA);
LCD_Write_DATA(0x05);		//GPIO[3:0] out 1

LCD_Write_COM(0xB8);
LCD_Write_DATA(0x07);	    //GPIO3=input, GPIO[2:0]=output
LCD_Write_DATA(0x01);		//GPIO0 normal

//LCD_Write_COM(0x36);		//rotation
//LCD_Write_DATA(0x21);		// -- Set to 0x22 to rotate 180 degrees 0x21

LCD_Write_COM(0xba);		//rotation
LCD_Write_DATA(0x01);		// -- Set to 0x22 to rotate 180 degrees 0x21


LCD_Write_COM(0xF0);		//pixel data interface
LCD_Write_DATA(0x03);


HAL_Delay(10);

//Lcd_SetArea(0, 0, 799, 479);

//Lcd_Write_Cmd(0x2a);    //SET column address
//Lcd_Write_Data(0x00); //SET start column address=0
//Lcd_Write_Data(0x00);
//Lcd_Write_Data(0x03); //SET end column address=799
//Lcd_Write_Data(0x1f);
//
//Lcd_Write_Cmd(0x2b);;     //SET page address
//Lcd_Write_Data(0x00);//SET start page address=0
//Lcd_Write_Data(0x00);
//Lcd_Write_Data(0x01);//SET end page address=479
//Lcd_Write_Data(0xdf);  //1f

LCD_Write_COM(0x29);		//display on

LCD_Write_COM(0xBE);		//set PWM for B/L
LCD_Write_DATA(0x06);
LCD_Write_DATA(0xF0);
LCD_Write_DATA(0x01);
LCD_Write_DATA(0xF0);
LCD_Write_DATA(0x00);
LCD_Write_DATA(0x00);

LCD_Write_COM(0xD0);
LCD_Write_DATA(0x0D);

LCD_Write_COM(0x2C);


}
//===========================================================================
//===========================================================================
//===========================================================================
//�-��� ��� �������� ������
//static __inline  - not work before define of function 
void 	TFT_Send_Cmd(uint8_t index)
{
	*(uint8_t *) (LCD_REG) = index;	  
}

//�-��� ��� �������� ������
void TFT_Write_Data(uint16_t data)
{   
    *(uint16_t *) (LCD_DATA) = data; 	
}
//�-��� ������������� ������� ������� �� X
void TFT_Set_X(uint16_t start_x,uint16_t end_x)
{
	TFT_Send_Cmd(0x002A);
	TFT_Write_Data(start_x>>8);
	TFT_Write_Data(start_x&0x00ff);
	
	TFT_Write_Data(end_x>>8);
	TFT_Write_Data(end_x&0x00ff);
}

//�-��� ������������� ������� ������� �� Y
void TFT_Set_Y(uint16_t start_y,uint16_t end_y)
{
	TFT_Send_Cmd(0x002B);
	TFT_Write_Data(start_y>>8);
	TFT_Write_Data(start_y&0x00ff);
	
	TFT_Write_Data(end_y>>8);
	TFT_Write_Data(end_y&0x00ff);
}

//�-��� ������ ���������� ����� �� �������
void TFT_Set_XY(uint16_t x, uint16_t y)
{
	TFT_Set_X(x, x);
	TFT_Set_Y(y, y);
}


//�-���  ������ ���������� ������� �������
void TFT_Set_Work_Area(uint16_t x, uint16_t y, uint16_t length, uint16_t width)
{
	TFT_Set_X(x, x+length-1);
	TFT_Set_Y(y, y+width-1);
	TFT_Send_Cmd(0x2C);
}


void TFT_Clear_Screen(uint16_t color)
{
	uint32_t i=0;
	TFT_Set_Work_Area(0,0,800,480);
	for(i=0; i < 384000; i++)
	{
		TFT_Write_Data(color);	//������� ��������� �����
	}
}


//�-��� ������ ������ ������� �������, �����, �� ��������� ����, �� ��������� �������(��� �� ������ ���� ������� ������������ ������ ������ �� �������� 8�8)
void TFT_Draw_Char(uint16_t x, uint16_t y, uint16_t color, uint16_t phone,const uint8_t *table, uint8_t ascii, uint8_t size)
{
	uint8_t i,f = 0;
	
	
	for (i = 0; i < 8; i++)
	{
		for(f = 0; f < 8; f++)
		{
			if((*(table + 8*(ascii-0x20)+i)>>(7-f))&0x01)
			{
				 TFT_Draw_Fill_Rectangle(x+f*size, y+i*size, size, size, color);
			}
			else
			{	
				 TFT_Draw_Fill_Rectangle(x+f*size, y+i*size, size, size, phone);
			}
		}
	}
}

//�-��� ������ ������, ��������� �� ��������� �������
void TFT_Draw_String(uint16_t x, uint16_t y, uint16_t color,uint16_t phone, const uint8_t *table, char *string, uint8_t size)
{
   //���������� ����� ������ ����� ������ ���� �����, ��� ��� ������ ������������� ����
	while(*string)
	{      
    //��������� �� ������� �� �� �� ������� ������ ��� ��������� ���������� �������,
    // ���� ��, �� ��������� �� ��������� �������
		if((x + 8) > (TFT_WIDTH-1))
		{
			x = 1;
			y = y + 8*size;
		}
		TFT_Draw_Char(x, y, color, phone, table, *string, size);//������������ ������
		x += 8*size;     //�������� ���������� ��� ��������� ���������� �������
		*string++;           //����������� �������� ���������, ����� �� �������� �� ��������� ������
	}
}

//�-��� ������ ����� ��������� ����� � �������
void TFT_Draw_Line (uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint8_t size,uint16_t color)
{
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	int error2 = 0;
	
	for (;;)
	{
		TFT_Draw_Fill_Rectangle(x1,y1,size,size,color);
		
		if(x1 == x2 && y1 == y2)
		break;
		
		error2 = error * 2;
		
		if(error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		
		if(error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

//�-��� ������ �������������� �����, ��������� �����, ������� � �����
void TFT_Draw_HLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color)
{
	uint16_t i=0;
	
	TFT_Set_Work_Area(x,y,length,size);
	for(i=0; i<(length*size); i++)
	TFT_Write_Data(color);
}

//�-��� ������ ������������ �����, ��������� �����, ������� � �����
void TFT_Draw_VLine(uint16_t x, uint16_t y, uint16_t length, uint16_t size, uint16_t color)
{
	uint16_t i=0;
	
	TFT_Set_Work_Area(x,y,size,length);
	for(i=0; i<(length*size); i++)
	TFT_Write_Data(color);
}

//�-��� ������ �������������, ��������� �����, ������, ������� ����� � �����
void TFT_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t size, uint16_t color)
{
	TFT_Draw_HLine(x, y, length, size, color);
	TFT_Draw_HLine(x, y + width, length, size, color);
	TFT_Draw_VLine(x, y, width, size, color);
	TFT_Draw_VLine(x + length - size, y, width, size, color);
}

//�-��� ������ ����������� �������������, ��������� �����, ������, �����
void TFT_Draw_Fill_Rectangle(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t color)
{
	uint32_t i=0;
	
	TFT_Set_Work_Area(x,y,length, width);
	for(i=0; i < length*width; i++)
	{
		TFT_Write_Data(color);	//������� ��������� �����
	}
}

// Desenha um triângulo apontando para baixo (base em cima, ápice embaixo)
void TFT_Draw_Inverted_Triangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint16_t color)
{
    uint16_t x1 = x;                // canto esquerdo da base
    uint16_t y1 = y;
    uint16_t x2 = x + width;        // canto direito da base
    uint16_t y2 = y;
    uint16_t x3 = x + width / 2;    // ápice
    uint16_t y3 = y + height;

    // reutiliza TFT_Draw_Line para conectar os três pontos
    TFT_Draw_Line(x1, y1, x2, y2, size, color);
    TFT_Draw_Line(x2, y2, x3, y3, size, color);
    TFT_Draw_Line(x3, y3, x1, y1, size, color);
}


void TFT_Draw_Fill_Triangle_Simple(uint16_t x, uint16_t y,
                                   uint16_t width, uint16_t height,
                                   uint8_t size, uint16_t color)
{
    for (uint16_t i = 0; i <= height; i++)
    {
        // para cada linha i, a largura cresce de 0 até width
        uint16_t w = (uint32_t)width * i / height;
        // desloca de forma a manter o triângulo centrado na caixa
        uint16_t x0 = x + (width - w) / 2;
        TFT_Draw_HLine(x0, y + i, w, size, color);
    }
}

// Preenche um triângulo isósceles apontando para baixo
// mesma convenção de parâmetros, mas base no topo
void TFT_Draw_Fill_Triangle_Inverted_Simple(uint16_t x, uint16_t y,
                                            uint16_t width, uint16_t height,
                                            uint8_t size, uint16_t color)
{
    for (uint16_t i = 0; i <= height; i++)
    {
        // aqui a largura decresce de width até 0
        uint16_t w = (uint32_t)width * (height - i) / height;
        uint16_t x0 = x + (width - w) / 2;
        TFT_Draw_HLine(x0, y + i, w, size, color);
    }
}

//�-��� ������ ����������� �� ������ � ��������� �������� ����� � ��������� ������
void TFT_Draw_Triangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint16_t color)
{
	
	TFT_Draw_Line( x1, y1, x2, y2, color, size);
	TFT_Draw_Line( x2, y2, x3, y3, color, size);
	TFT_Draw_Line( x3, y3, x1, y1, color, size);
}

//�-��� ������ ���������� ������� �������, ������ �������� ������� � ��������� ������, ����� �������� 
//������ ���������� ������ ������ ��� ����� ���������� �������� fill ������ �������, ����� ����
void TFT_Draw_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint16_t color)
{
	int a_,b_,P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_)
	{
		if(fill == 1)
		{
			TFT_Draw_Fill_Rectangle(x-a_,y-b_,2*a_+1,2*b_+1,color);
			TFT_Draw_Fill_Rectangle(x-b_,y-a_,2*b_+1,2*a_+1,color);
		}
		else
		{
			TFT_Draw_Fill_Rectangle(a_+x, b_+y, size, size, color);
			TFT_Draw_Fill_Rectangle(b_+x, a_+y, size, size, color);
			TFT_Draw_Fill_Rectangle(x-a_, b_+y, size, size, color);
			TFT_Draw_Fill_Rectangle(x-b_, a_+y, size, size, color);
			TFT_Draw_Fill_Rectangle(b_+x, y-a_, size, size, color);
			TFT_Draw_Fill_Rectangle(a_+x, y-b_, size, size, color);
			TFT_Draw_Fill_Rectangle(x-a_, y-b_, size, size, color);
			TFT_Draw_Fill_Rectangle(x-b_, y-a_, size, size, color);
		}
		if (P < 0 )
		{
			P = (P + 3) + (2* a_);
			a_ ++;
		}
		else
		{
			P = (P + 5) + (2* (a_ - b_));
			a_ ++;
			b_ --;
		}
	}
}

//��������������� �-��� ��� ����������� ���� ��������������
void TFT_Draw_Circle_Helper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint16_t color)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      TFT_Draw_Fill_Rectangle(x0 + x, y0 + y, size, size, color);
      TFT_Draw_Fill_Rectangle(x0 + y, y0 + x, size, size, color);
    }
    if (cornername & 0x2) {
      TFT_Draw_Fill_Rectangle(x0 + x, y0 - y, size, size, color);
      TFT_Draw_Fill_Rectangle(x0 + y, y0 - x, size, size, color);
    }
    if (cornername & 0x8) {
      TFT_Draw_Fill_Rectangle(x0 - y, y0 + x, size, size, color);
      TFT_Draw_Fill_Rectangle(x0 - x, y0 + y, size, size, color);
    }
    if (cornername & 0x1) {
      TFT_Draw_Fill_Rectangle(x0 - y, y0 - x, size, size, color);
      TFT_Draw_Fill_Rectangle(x0 - x, y0 - y, size, size, color);
    }
  }
}

//�-��� ������ ������������� �������� �����, ������, �������� ����������� ����, ������� ����� � ��������� ������
void TFT_Draw_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint16_t color)
{

  TFT_Draw_HLine(x+r  , y    , length-2*r, size, color); // Top
  TFT_Draw_HLine(x+r  , y+width-1, length-2*r, size, color); // Bottom
  TFT_Draw_VLine(x    , y+r  , width-2*r, size, color); // Left
  TFT_Draw_VLine(x+length-1, y+r  , width-2*r, size, color); // Right

  TFT_Draw_Circle_Helper(x+r    , y+r    , r, 		 			1, size, color);
  TFT_Draw_Circle_Helper(x+length-r-1, y+r    , r, 			2, size, color);
  TFT_Draw_Circle_Helper(x+length-r-1, y+width-r-1, r,  4, size, color);
  TFT_Draw_Circle_Helper(x+r    , y+width-r-1, r, 			8, size, color);
}

//��������������� �-��� ��� ����������� ���� ������������ ��������������
void TFT_Draw_Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) 
{

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      TFT_Draw_VLine(x0+x, y0-y, 2*y+1+delta, 1, color);
      TFT_Draw_VLine(x0+y, y0-x, 2*x+1+delta, 1, color);
    }
    if (cornername & 0x2) {
     TFT_Draw_VLine(x0-x, y0-y, 2*y+1+delta, 1, color);
     TFT_Draw_VLine(x0-y, y0-x, 2*x+1+delta, 1, color);
    }
  }
}

//�-��� ������ ����������� ������������� �������� �����, ������, �������� ����������� ����� � ��������� ������
void TFT_Draw_Fill_Round_Rect(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint16_t color)
{
  TFT_Draw_Fill_Rectangle(x+r, y, length-2*r, width, color);

  TFT_Draw_Fill_Circle_Helper(x+length-r-1, y+r, r, 1, width-2*r-1, color);
  TFT_Draw_Fill_Circle_Helper(x+r    , y+r, r, 2, width-2*r-1, color);
}

static void LCD_Char(int16_t x, int16_t y, const GFXglyph *glyph, const GFXfont *font, uint8_t size, uint32_t color24)
{
	uint8_t  *bitmap = font -> bitmap;
	uint16_t bo = glyph -> bitmapOffset;
	uint8_t bits = 0, bit = 0;
	uint16_t set_pixels = 0;
	uint8_t  cur_x, cur_y;
	for(cur_y = 0; cur_y < glyph -> height; cur_y++)
	{
		for(cur_x = 0; cur_x < glyph -> width; cur_x++)
		{
			if(bit == 0)
			{
				bits = (*(const unsigned char *)(&bitmap[bo++]));
				bit  = 0x80;
			}
			if(bits & bit)
			{
				set_pixels++;
			}
			else if (set_pixels > 0)
			{
				TFT_Draw_Fill_Rectangle(x + (glyph -> xOffset + cur_x - set_pixels) * size, y + (glyph -> yOffset + cur_y) * size, size * set_pixels, size, color24);
				set_pixels = 0;
			}
			bit >>= 1;
		}
		if (set_pixels > 0)
		{
			TFT_Draw_Fill_Rectangle(x + (glyph -> xOffset + cur_x-set_pixels) * size, y + (glyph -> yOffset + cur_y) * size, size * set_pixels, size, color24);
			set_pixels = 0;
		}
	}
}
void LCD_Font(uint16_t x, uint16_t y, char *text, const GFXfont *p_font, uint8_t size, uint32_t color24)
{
	int16_t cursor_x = x;
	int16_t cursor_y = y;
	GFXfont font;
	memcpy((&font), (p_font), (sizeof(GFXfont)));
	for(uint16_t text_pos = 0; text_pos < strlen(text); text_pos++)
	{
		char c = text[text_pos];
		if(c == '\n')
		{
			cursor_x = x;
			cursor_y += font.yAdvance * size;
		}
		else if(c >= font.first && c <= font.last && c != '\r')
		{
			GFXglyph glyph;
			memcpy((&glyph), (&font.glyph[c - font.first]), (sizeof(GFXglyph)));
			LCD_Char(cursor_x, cursor_y, &glyph, &font, size, color24);
			cursor_x += glyph.xAdvance * size;
		}
	}
}

void LCD_Font_BG(uint16_t x, uint16_t y, char *text,
                 const GFXfont *font, uint8_t size,
                 uint32_t fg_color, uint32_t bg_color)
{
    int16_t cursor_x = x;
    int16_t cursor_y = y;

    for (size_t i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        if (c == '\n')
        {
            cursor_x = x;
            cursor_y += font->yAdvance * size;
            continue;
        }
        if (c < font->first || c > font->last || c == '\r')
            continue;

        GFXglyph glyph = font->glyph[c - font->first];
        const uint8_t *bitmap = font->bitmap + glyph.bitmapOffset;
        uint16_t w = glyph.width;
        uint16_t h = glyph.height;
        uint8_t  xo = glyph.xOffset;
        uint8_t  yo = glyph.yOffset;

        // Para cada pixel do glyph:
        for (uint16_t row = 0; row < h; row++)
        {
            for (uint16_t col = 0; col < w; col++)
            {
                uint16_t byteIndex = row * ((w + 7) / 8) + (col / 8);
                uint8_t  bitMask   = 0x80 >> (col & 7);
                bool     pixelOn   = (bitmap[byteIndex] & bitMask) != 0;
                uint32_t color     = pixelOn ? fg_color : bg_color;

                // desenha o pixel escalonado
                TFT_Draw_Fill_Rectangle(
                    cursor_x + (xo + col) * size,
                    cursor_y + (yo + row) * size,
                    size, size,
                    color
                );
            }
        }
        // avança cursor
        cursor_x += glyph.xAdvance * size;
    }
}
