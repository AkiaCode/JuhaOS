#ifndef _LAYER_H_
#define _LAYER_H_

#include <Types.h>
#include <Graphics.h>

/* Used Haribote OS source
 * thank you for Kawahi Hidemi
 * Book that I referenced:https://bit.ly/32vuYHA
 */

#define LAYER_MAXCOUNT 256

#pragma pack(push , 1)

typedef struct {
	
}LAYER;

typedef struct {
	WORD *VideoRAM;
	int Width;
	int Height;
	int Top;

	LAYER *Layers[LAYER_MAXCOUNT];
	LAYER Layers0[LAYER_MAXCOUNT];
}MANAGER;

#pragma pack(pop)

#endif