#include <Math.h>

DWORD pow(DWORD X , DWORD Y) {
	if(Y == 0) {
		return 1;
	}
	return X*pow(X , Y-1);
}
