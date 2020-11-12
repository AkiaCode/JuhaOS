#include <Layer.h>
#include <MemoryManagement.h>

/* Used Haribote OS source
 * thank you for Kawahi Hidemi
 * Book that I referenced:https://bit.ly/32vuYHA
 */

static LAYERMANAGER *LayerManager;

void UpdateLayerSub(int VX1 , int VY1 , int VX2 , int VY2 , int Height) {
	int i;
	int BX;
	int BY;
	int VX;
	int VY;
	int BX1;
	int BY1;
	int BX2;
	int BY2;
	WORD *Buffer;
	WORD TempBuffer;
	WORD *VideoMemory = LayerManager->VideoMemory;
	LAYER *Layer;
	if(VX1 < 0) {
		VX1 = 0;
	}
	if(VY1 < 0) {
		VY1 = 0;
	}
	if(VX2 > LayerManager->Width) {
		VX2 = LayerManager->Width;
	}
	if(VY2 > LayerManager->Height) {
		VY2 = LayerManager->Height;
	}
	for(i = Height; i <= LayerManager->Top; i++) {
		Layer = LayerManager->Layers[i];
		Buffer = Layer->Buffer;
		BX1 = VX1-Layer->VX1;
		BY1 = VY1-Layer->VY1;
		BX2 = VX2-Layer->VX1;
		BY2 = VY2-Layer->VY1;
		if(BX1 < 0) {
			BX1 = 0;
		}
		if(BY1 < 0) {
			BY1 = 0;
		}
		if(BX2 > Layer->BXSize) {
			BX2 = Layer->BXSize;
		}
		if(BY2 > Layer->BYSize) {
			BY2 = Layer->BYSize;
		}
		for(BY = BY1; BY < BY2; BY++) {
			VY = Layer->VY1+BY;
			for(BX = BX1; BX < BX2; BX++) {
				VX = Layer->VX1+BX;
				if(Buffer[BY*Layer->BXSize+BX] != Layer->InvisibleColor) {
					LayerManager->VideoMemory[(VY*LayerManager->Width)+VX] = Buffer[(BY*Layer->BXSize)+BX];
				}
			}
		}
	}
}

void InitLayerSystem(WORD *VideoMemory , int Width , int Height) {
	int i;
	LayerManager = (LAYERMANAGER*)Memory::malloc(sizeof(LAYERMANAGER));
	LayerManager->VideoMemory = VideoMemory;
	LayerManager->Width = Width;
	LayerManager->Height = Height;
	LayerManager->Top = -1;
	for(i = 0; i < LAYER_MAXCOUNT; i++) {
		LayerManager->Layers0[i].Flags = 0;
	}
}

LAYER *CreateLayer(WORD *Buffer , int Width , int Height , int InvisibleColor) {
	int i;
	LAYER *Layer;
	for(i = 0; i < LAYER_MAXCOUNT; i++) {
		if(LayerManager->Layers0[i].Flags == 0) {
			Layer = &(LayerManager->Layers0[i]);
			Layer->Flags = LAYER_FLAGS_USING;
			Layer->Height = -1;
			Layer->Buffer = Buffer;
			Layer->BXSize = Width;
			Layer->BYSize = Height;
			Layer->InvisibleColor = InvisibleColor;
			return Layer;
		}
	}
	return NULL;
}

void ChangeLayerHeight(LAYER *Layer , int Height) {
	int i;
	int OldHeight = Layer->Height;
	if(Height > LayerManager->Top+1) {
		Height = LayerManager->Top+1;
	}
	if(Height < -1) {
		Height = -1;
	}
	Layer->Height = Height;
	if(OldHeight > Height) {
		if(Height >= 0) {
			for(i = OldHeight; i > Height; i--) {
				LayerManager->Layers[i] = LayerManager->Layers[i-1];
				LayerManager->Layers[i]->Height = i;
			}
			LayerManager->Layers[Height] = Layer;
		}
		else {
			if(LayerManager->Top > OldHeight) {
				for(i = OldHeight; i < LayerManager->Top; i++) {
					LayerManager->Layers[i] = LayerManager->Layers[i+1];
					LayerManager->Layers[i]->Height = i;
				}
			}
			LayerManager->Top--;
		}
		UpdateLayer(Layer , Layer->VX1 , Layer->VY1 , Layer->VX1+Layer->BXSize , Layer->VY1+Layer->BYSize);
	}
	else if(OldHeight < Height) {
		if(OldHeight >= 0) {
			for(i = OldHeight; i < Height; i++) {
				LayerManager->Layers[i] = LayerManager->Layers[i+1];
				LayerManager->Layers[i]->Height = i;
			}
			LayerManager->Layers[i] = Layer;
		}
		else {
			for(i = LayerManager->Top; i >= Height; i--) {
				LayerManager->Layers[i+1] = LayerManager->Layers[i];
				LayerManager->Layers[i+1]->Height = i+1;
			}
			LayerManager->Layers[Height] = Layer;
			LayerManager->Top++;
		}
		UpdateLayer(Layer , Layer->VX1 , Layer->VY1 , Layer->VX1+Layer->BXSize , Layer->VY1+Layer->BYSize);
	}
}

void UpdateLayer(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2) {
	if(Layer->Height >= 0) {
		UpdateLayerSub(Layer->VX1+X1 , Layer->VY1+Y1 , Layer->VX1+X2 , Layer->VY1+Y2 , Layer->Height);
	}
}

void MoveLayer(LAYER *Layer , int X , int Y) {
	int OldVX1 = Layer->VX1;
	int OldVY1 = Layer->VY1;
	Layer->VX1 = X;
	Layer->VY1 = Y;
	if(Layer->Height >= 0) {
		UpdateLayerSub(OldVX1 , OldVY1 , OldVX1+Layer->BXSize , OldVY1+Layer->BYSize , 0);
		UpdateLayerSub(X , Y , X+Layer->BXSize , Y+Layer->BYSize , Layer->Height);
	}
}