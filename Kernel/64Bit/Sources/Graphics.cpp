#include <Graphics.h>
#include <WindowSystem.h>
#include <MemoryManagement.h>

void Graphics::DrawPixel(WORD *WindowBuffer , int X , int Y , WORD Color) {
	VBEMODEINFOBLOCK *Block = (VBEMODEINFOBLOCK*)VBEMODEINFOBLOCK_STARTADDRESS;
    if((X < 0)||(Y < 0)||(X >= Block->Width)||(Y >= Block->Height)) {
    	return;
    }

   	WindowBuffer[(Y*Block->Width)+X] = Color;
}

void Graphics::DrawRectangle(WORD *WindowBuffer , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	int X;
	int Y;
	for(Y = Y1; Y <= Y2; Y++) {
		for(X = X1; X <= X2; X++) {
			Graphics::DrawPixel(WindowBuffer , X , Y , Color);
		}
	}
}

void Graphics::DrawEmptyRectangle(WORD *WindowBuffer , int X1 , int Y1 , int X2 , int Y2 , WORD Color , int Padding) {
	DrawRectangle(WindowBuffer , X1 , Y1 , X1+Padding , Y2 , Color);
	DrawRectangle(WindowBuffer , X1 , Y1 , X2 , Y1+Padding , Color);
	DrawRectangle(WindowBuffer , X2 , Y2 , X1 , Y2+2 , Color);
	//future
}

void Graphics::DrawLine(WORD *WindowBuffer , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	int i;
	int X = 0;
	int Y = 0;
	int DX = X2-X1;
	int DY = Y2-Y1;
	int IX = (DX < 0) ? -DX : DX;
	int IY = (DY < 0) ? -DY : DY;
	int Delta = (IX > IY) ? IX : IY;
	int PointX = X1;
	int PointY = Y1;
	for(i = 0; i <= Delta; i++) {
		X += IX;
		Y += IY;
		if(X >= Delta) {
			X -= Delta;
			if(DX > 0) {
				PointX += 1;
			}
			else {
				if(DX != 0) {
					PointX += -1;
				}
			}
		}
		if(Y >= Delta) {
			Y -= Delta;
			if(DY > 0) {
				PointY += 1;
			}
			else {
				if(DY != 0) {
					PointY += -1;
				}
			}
		}
		DrawPixel(WindowBuffer , PointX , PointY , Color);
	}
}

void Graphics::DrawCursor(WORD *WindowBuffer , int X , int Y) {
	int i;
	const int LastX = X;
	const int LastY = Y;
    BYTE MouseCursor[33][33] = {
		"######                          " , 
		"##########                      " , 
		"##@@@@########                  " , 
		"##@$$$@@@@########              " , 
		"##@$%%$$$$@@@@########          " , 
		"##@$%%%%%%$$$$@@@@########      " , 
		" ##@$%0000%%%%$$$$@@@@########  " , 
		" ##@$%00000000%%%%$$$$@@@@######" , 
		" ##@$%000000000000%%%%$$$$@@@@##" , 
		" ##@$%0000000000%%$$$$@@@@######" , 
		"  ##@$%00000000%$$@@@@########  " , 
		"  ##@$%0000000%$@@########      " , 
		"  ##@$%0000000$@######          " , 
		"  ##@$%0000000%$@##             " , 
		"   ##@$%000%$%0%$@##            " , 
		"   ##@$%00%$@$%0%$@##           " , 
		"   ##@$%0%$@#@$%0%$@##          " , 
		"   ##@$%0%$@##@$%0%$@##         " , 
		"    ##@$%$@####@$%0%$@##        " , 
		"    ##@$%$@## ##@$%0%$@##       " , 
		"    ##@$%$@##  ##@$%0%$@##      " , 
		"    ##@$%$@##   ##@$%0%$@##     " , 
		"     ##@$@##     ##@$%0%$@##    " , 
		"     ##@$@##      ##@$%0%$@##   " , 
		"     ##@$@##       ##@$%0%$@##  " , 
		"     ##@$@##        ##@$%$@##   " , 
		"      ##@##          ##@$@##    " , 
		"      ##@##           ##@##     " , 
		"      ##@##            ###      " , 
		"      ##@##             #       " , 
		"       ###                      " , 
		"       ###                      " , 
	};
	for(i = 0; i < 32*32; i++) {
		switch(MouseCursor[Y-LastY][X-LastX]) {
			case '0':
				DrawPixel(WindowBuffer , X , Y , RGB(255 , 255 , 255));
				break;
			case '%':
				DrawPixel(WindowBuffer , X , Y , RGB(185 , 185 , 185));
				break;
			case '$':
				DrawPixel(WindowBuffer , X , Y , RGB(138 , 138 , 138));
				break;
			case '@':
				DrawPixel(WindowBuffer , X , Y , RGB(81 , 81 , 81));
				break;
			case '#':
				DrawPixel(WindowBuffer , X , Y , RGB(0 , 0 , 0));
				break;
			default:
				break;
		}
		X++;
		if(X-LastX >= 32) {
			X = LastX;
			Y++;
		}
	}
}

#define XBUTTON_COLOR 0

void Graphics::DrawWindow(WORD *WindowBuffer , int X , int Y , int Width , int Height , const char *Title) {
	int i;
	int j;
	const int Padding = 2;
	const int TaskbarSize = 14;
	int XButtonX = X+Width-(Padding*2)-TaskbarSize;
	int XButtonY = Y+Padding+3;

	const WORD BackgroundColor = WINDOW_DEFAULTBACKGROUND;
	const WORD TaskbarColor = WINDOW_TITLECOLOR;

	const char XButton[13][14] = {
		"#############" , 
		"#%%%%%%%%%%%#" , 
		"#%         @#" , 
		"#% XX   XX @#" , 
		"#%  XX XX  @#" , 
		"#%   XXX   @#" , 
		"#%   XXX   @#" , 
		"#%  XX XX  @#" , 
		"#% XX   XX @#" , 
		"#%         @#" , 
		"#%@@@@@@@@@@#" , 
		"#############" , 
	};
	Graphics::DrawRectangle(WindowBuffer , X , Y , X+Width , Y+Height , 0x00);
	Graphics::DrawRectangle(WindowBuffer , X+Padding , Y+Padding , X+Width-(Padding) , Y+Height-(Padding) , RGB(77 , 77 , 77));
	Graphics::DrawRectangle(WindowBuffer , X+Padding , Y+Padding , X+Width-(Padding)-1 , Y+Height-(Padding)-1 , BackgroundColor);

	Graphics::DrawRectangle(WindowBuffer , X+Padding+1 , Y+Padding+1 , X+Width-(Padding*2) , Y+Padding+TaskbarSize+2 , TaskbarColor);
	Graphics::DrawLine(WindowBuffer , X+Padding+1 , Y+Padding+1 , X+Width-(Padding*2)-1 , Y+Padding+1 , 0x00);
	Graphics::DrawLine(WindowBuffer , X+Padding+1 , Y+Padding , X+Padding+1 , Y+Padding+TaskbarSize+1 , 0x00);
	Graphics::DrawLine(WindowBuffer , X+Width-(Padding*2) , Y+Padding , X+Width-(Padding*2) , Y+Padding+TaskbarSize+1 , 0x00);
	Graphics::DrawLine(WindowBuffer , X+Padding+1 , Y+Padding+TaskbarSize+2 , X+Width-(Padding*2)-1 , Y+Padding+TaskbarSize+2 , 0x00);
	for(j = 0; j < 12; j++) {
		for(i = 0; i < 13; i++) {
			switch(XButton[j][i]) {
				case '#':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(0 , 0 , 0));
					}
					else {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(0 , 0 , 0));
					}
					break;
				case ' ':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(255 , 0 , 0));
					}
					else {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(101 , 101 , 101));
					}
					break;
				case '%':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(255 , 121 , 121));
					}
					else {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(174 , 174 , 174));
					}
					break;
				case '@':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(161 , 0 , 0));
					}
					else {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(67 , 67 , 67));
					}
					break;
				case 'X':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(255 , 255 , 255));
					}
					else {
						Graphics::DrawPixel(WindowBuffer , XButtonX+i , XButtonY+j , RGB(0 , 0 , 0));
					}
					break;
			}
		}
	}
}