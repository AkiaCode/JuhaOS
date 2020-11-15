#include <Graphics.h>
#include <WindowSystem.h>
#include <MemoryManagement.h>
#include <Font.h>
#include <string.h>

void Layer::SetLayer(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD *Buffer) {
	Layer->X1 = X1;
	Layer->Y1 = Y1;
	Layer->X2 = X2;
	Layer->Y2 = Y2;
	Layer->Buffer = Buffer;
}

BOOL Layer::GetOverlappedArea(LAYER *Layer1 , LAYER *Layer2 , LAYER *OverlappedArea) {
	int X1;
	int X2;
	int Y1;
	int Y2;

	X1 = MAX(Layer1->X1 , Layer2->X1);
	X2 = MIN(Layer1->X2 , Layer2->X2);

	if(X2 < X1) {
		return FALSE;
	}

	Y1 = MAX(Layer1->Y1 , Layer1->Y1);
	Y2 = MIN(Layer2->Y2 , Layer2->Y2);
	if(Y2 < Y1) {
		return FALSE;
	}
	OverlappedArea->X1 = X1;
	OverlappedArea->Y1 = Y1;
	OverlappedArea->X2 = X2;
	OverlappedArea->Y2 = Y2;
	return TRUE;
}

void Graphics::DrawPixel(LAYER *Layer , int X , int Y , WORD Color) {
    if((X < 0)||(Y < 0)||(X >= Layer->X2-Layer->X1)||(Y >= Layer->Y2-Layer->Y1)) {
    	return;
    }
   	Layer->Buffer[(Y*(Layer->X2-Layer->X1))+X] = Color;
}

void Graphics::DrawRectangle(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
	int X;
	int Y;
	for(Y = Y1; Y < Y2; Y++) {
		for(X = X1; X < X2; X++) {
			Graphics::DrawPixel(Layer , X , Y , Color);
		}
	}
}

void Graphics::DrawText(LAYER *Layer , const char *Text , int X , int Y , WORD Color) {
	int i;
	int j;
	int k;
	int CurrentX;
	int CurrentY;
	BYTE BitMask;
	int BitMaskStartAddress;
	WORD *VideoMemory = Layer->Buffer;
	const int Width = 8;
	const int Height = 16;
	int HeightBackup;
	CurrentX = X;
	for(k = 0; k < strlen(Text); k++) {
		CurrentY = Y;
		BitMaskStartAddress = Text[k]*Height;
		for(j = 0; j < Height; j++) {
			BitMask = Consolas_8x16_Bold[BitMaskStartAddress++];
			for(i = 0; i < Width; i++) {
				if(BitMask & (0x01 << (Width-i-1))) {
					DrawPixel(Layer , CurrentX+i , CurrentY , Color);
				}
			}
			CurrentY += 1;
		}
		CurrentX += Width;
	}
}

void Graphics::DrawEmptyRectangle(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color , int Padding) {
	
}

void Graphics::DrawLine(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2 , WORD Color) {
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
		DrawPixel(Layer , PointX , PointY , Color);
	}
}

void Graphics::DrawCursor(LAYER *Layer , int X , int Y) {
	int i;
	const int LastX = X;
	const int LastY = Y;
	BYTE MouseInvisibleArea[GRAPHICS_MOUSE_WIDTH*GRAPHICS_MOUSE_HEIGHT];
    BYTE MouseCursor[33][33] = {
    	"###           " , 
    	"#@@###        " , 
    	"#@@@@@####    " , 
    	" #@$$$@@@@### " , 
    	" #@$00$$$@@#  " , 
    	" #@$000$@##   " , 
    	"  #@$0$@#     " , 
    	"  #@$$@$@#    " , 
    	"  #@$@#@$@#   " , 
    	"  #@@# #@$@#  " , 
    	"   #@#  #@#   " , 
    	"   ##    #    " , 
    	"   #          " , 
    	"              " , 
	};
	memset(MouseInvisibleArea , 0 , sizeof(MouseInvisibleArea));
	for(i = 0; i < GRAPHICS_MOUSE_WIDTH*GRAPHICS_MOUSE_HEIGHT; i++) {
		switch(MouseCursor[Y-LastY][X-LastX]) {
			case '0':
				DrawPixel(Layer , X , Y , RGB(255 , 255 , 255));
				break;
			case '%':
				DrawPixel(Layer , X , Y , RGB(185 , 185 , 185));
				break;
			case '$':
				DrawPixel(Layer , X , Y , RGB(138 , 138 , 138));
				break;
			case '@':
				DrawPixel(Layer , X , Y , RGB(81 , 81 , 81));
				break;
			case '#':
				DrawPixel(Layer , X , Y , RGB(0 , 0 , 0));
				break;
			case ' ':
				DrawPixel(Layer , X , Y , GRAPHICS_MOUSE_INVISIBLECOLOR);
				break;
			default:
				break;
		}
		X++;
		if(X-LastX >= Layer->X2-Layer->X1) {
			X = LastX;
			Y++;
		}
	}
}

#define XBUTTON_COLOR 0

void Graphics::DrawWindow(LAYER *Layer , int X , int Y , int Width , int Height , const char *Title , WORD BackgroundColor) {
	int i;
	int j;
	const int Padding = 2;
	const int TaskbarSize = GRAPHICS_WINDOW_TITLEBARSIZE;
	int XButtonX = X+Width-(Padding*2)-TaskbarSize;
	int XButtonY = Y+Padding+3;

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
	Graphics::DrawRectangle(Layer , X , Y , X+Width , Y+Height , 0x00);
	Graphics::DrawRectangle(Layer , X+Padding , Y+Padding , X+Width-(Padding) , Y+Height-(Padding) , RGB(77 , 77 , 77));
	Graphics::DrawRectangle(Layer , X+Padding , Y+Padding , X+Width-(Padding)-1 , Y+Height-(Padding)-1 , BackgroundColor);

	Graphics::DrawRectangle(Layer , X+Padding+1 , Y+Padding+1 , X+Width-(Padding*2) , Y+Padding+TaskbarSize+2 , TaskbarColor);
	Graphics::DrawLine(Layer , X+Padding+1 , Y+Padding+1 , X+Width-(Padding*2)-1 , Y+Padding+1 , 0x00);
	Graphics::DrawLine(Layer , X+Padding+1 , Y+Padding , X+Padding+1 , Y+Padding+TaskbarSize+1 , 0x00);
	Graphics::DrawLine(Layer , X+Width-(Padding*2) , Y+Padding , X+Width-(Padding*2) , Y+Padding+TaskbarSize+1 , 0x00);
	Graphics::DrawLine(Layer , X+Padding+1 , Y+Padding+TaskbarSize+2 , X+Width-(Padding*2)-1 , Y+Padding+TaskbarSize+2 , 0x00);
	for(j = 0; j < 12; j++) {
		for(i = 0; i < 13; i++) {
			switch(XButton[j][i]) {
				case '#':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(0 , 0 , 0));
					}
					else {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(0 , 0 , 0));
					}
					break;
				case ' ':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(255 , 0 , 0));
					}
					else {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(101 , 101 , 101));
					}
					break;
				case '%':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(255 , 121 , 121));
					}
					else {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(174 , 174 , 174));
					}
					break;
				case '@':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(161 , 0 , 0));
					}
					else {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(67 , 67 , 67));
					}
					break;
				case 'X':
					if(XBUTTON_COLOR == 0) {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(255 , 255 , 255));
					}
					else {
						Graphics::DrawPixel(Layer , XButtonX+i , XButtonY+j , RGB(0 , 0 , 0));
					}
					break;
			}
		}
	}
	Graphics::DrawText(Layer , Title , X+6 , Y+4 , RGB(255 , 255 , 255));
}