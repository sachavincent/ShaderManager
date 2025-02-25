uniform int a;

flat in int o;
out float oo;

void main() {
	if (o < 0)
		oo = 0.1;
	else
		oo = -0.1;
}