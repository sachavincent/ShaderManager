#pragma once

#include <glslang/Include/intermediate.h>
#include <glslang/MachineIndependent/localintermediate.h>

struct Symbol {
	std::string name;
	glslang::TBasicType type;

	Symbol(::glslang::TIntermSymbol* symbol) {
		name = symbol->getName();
		type = symbol->getType().getBasicType();
	}
};