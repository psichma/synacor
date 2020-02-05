#include <fstream>
#include <vector>
#include <stack>
#include "utils.h"
#include <stdio.h>


// short -> 2 bytes
// char  -> 1 byte

// 16 bit -> 2 bytes -> short
// memory 15 bit address space
// memory 16 bit values

enum NumberType {
	LITERAL,
	REGISTER,
	INVALID
};

enum Opcode {
	// normally numbered
	HALT,
	SET,
	PUSH,
	POP,
	EQ,
	GT,
	JMP,
	JT,
	JF,
	ADD,
	MULT,
	MOD,
	AND,
	OR,
	NOT,
	RMEM,
	WMEM,
	CALL,
	RET,
	OUT,
	IN,
	NOOP
};

std::vector<unsigned short> memory;
unsigned short registers[8];
std::stack<unsigned short> stack;

NumberType getNumberType(unsigned short val) {
	if (val >= 0 && val <= 32767) return NumberType::LITERAL;
	if (val >= 32768 && val <= 32775) return NumberType::REGISTER;
	return NumberType::INVALID;
}

unsigned short getRegisterIdx(unsigned short val) {
	NumberType type = getNumberType(val);
	if(type == NumberType::LITERAL && val < 8) return val;
	else if(type == NumberType::REGISTER) return val - 32768;
	return 8;
}

unsigned short getValue(unsigned short val) {
	NumberType type = getNumberType(val);
	if(type == NumberType::LITERAL) return val;
	return registers[getRegisterIdx(val)];
}



int main(int argc, char* argv[]) {
	std::string filename = "challenge.bin"; // could later be an argument
	std::string stringData = readFile(filename);
	memory = parseData(stringData);
	if(memory.size() == 0) {
		printf("data is empty\n");
		return 1;
	}

	printf("running program\n");

	// program counter aka pointer
	unsigned int pc = 0;

	while(pc < memory.size()) {
		unsigned short instr = memory[pc];

		/*
		// 0 HALT
		if(instr == Opcode::HALT) {
			printf("HALT, pc %d", instr);
			return 0;
		}

		// 1 SET
		else if(instr == Opcode::SET) {
			unsigned short param0 = getRegisterIdx(memory[pc + 1]);
			unsigned short param1 = getValue(memory[pc + 2]);
			if(param0 > 7) {
				printf("register param > 7! param: %d", param0);
				return 1;
			}
			registers[param0] = param1;
			pc += 3;
		}
		
		// 2 PUSH
		else if(instr == Opcode::PUSH) {
			unsigned short param0 = getValue(memory[pc + 1]);
			stack.push(param0);
			pc += 2;
		}

		// 3 POP
		else if(instr == Opcode::POP) {
			unsigned short param0 = getValue(memory[pc + 1]);
			stack.push(param0);
			pc += 2;
		}
*/	
		//if(instr == Opcode::POP) {
		//	printf("pop onto %d\n", memory[pc + 1]);
		//}
		//
		//
		
		pc++;
	}


	return 0;
}

