struct Test {
    uint a;
    int b;
	float c;
};
layout(std430, binding = $(TEST)) readonly buffer TestBuffer {
    Test test[];
};
