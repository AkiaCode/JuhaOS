#ifndef _LAYER_H_
#define _LAYER_H_

#include <Types.h>

/* Used Haribote OS source
 * thank you for Kawahi Hidemi
 * Book that I referenced:https://bit.ly/32vuYHA
 */

#define LAYER_MAXCOUNT 256
#define LAYER_FLAGS_USING 1

#pragma pack(push , 1)

typedef struct {
	WORD *Buffer;
	int BXSize;
	int BYSize;
	int VX1;
	int VY1;
	int Height;
	int Flags;
	WORD InvisibleColor;
}LAYER;

typedef struct {
	WORD *VideoMemory;
	int Width;
	int Height;
	int Top;

	LAYER Layers0[LAYER_MAXCOUNT];
	LAYER *Layers[LAYER_MAXCOUNT];
}LAYERMANAGER;

#pragma pack(pop)

void InitLayerSystem(WORD *WORD , int Width , int Height);
LAYER *CreateLayer(WORD *Buffer , BOOL InvisibleAreaUsing , int Width , int Height , int InvisibleColor);
void ChangeLayerHeight(LAYER *Layer , int Height);
void UpdateLayer(LAYER *Layer , int X1 , int Y1 , int X2 , int Y2);
void MoveLayer(LAYER *Layer , int X , int Y);
void RemoveLayer(LAYER *Layer);

#endif