#include "testU4.glsl"

flat in int o;
out float oo;

void main() {
	if (o < 0)
		oo = test[0].c;
	else
		oo = -test[0].c;
}