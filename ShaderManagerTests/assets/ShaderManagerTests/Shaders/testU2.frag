uniform float a;

flat in int o;
out float oo;

void main() {
	if (o < 0)
		oo = a;
	else
		oo = -a;
}