#include <fstream>
#include <vector>
#include <stack>
#include "utils.h"
#include <stdio.h>

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

const unsigned short MODULO = 32768;

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
	if(type == NumberType::INVALID) {
		printf("number invalid!\n");
		exit(1);
	}
	if(type == NumberType::LITERAL) return val;
	return registers[getRegisterIdx(val)];
}

std::string inputBuffer;

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

		// 0 HALT
		//   stop execution and terminate the program
		if(instr == Opcode::HALT) {
			printf("HALT, pc %d\n", instr);
			return 0;
		}

		// 1 SET
		//   set register <a> to the value of <b>
		else if(instr == Opcode::SET) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);

			registers[a] = b;
			pc += 3;
		}

		// 2 PUSH
		//   push <a> onto the stack
		else if(instr == Opcode::PUSH) {
			unsigned short a = getValue(memory[pc + 1]);
			stack.push(a);
			pc += 2;
		}

		// 3 POP
		//   remove the top element from the stack and write it into <a>; empty stack = error
		else if(instr == Opcode::POP) {
			if(stack.size() == 0) {
				printf("POP ERROR: empty stack\n");
				return 1;
			}
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			registers[a] = stack.top();
			stack.pop();
			pc += 2;
		}

		// 4 EQ
		//   set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
		else if(instr == Opcode::EQ) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);
			if(b == c) registers[a] = 1;
			else registers[a] = 0;
			pc += 4;
		}

		// 5 GT
		//   set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
		else if(instr == Opcode::GT) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);
			if(b > c) registers[a] = 1;
			else registers[a] = 0;
			pc += 4;
		}

		// 6 JMP
		//   jump to <a>
		else if(instr == Opcode::JMP) {
			unsigned short a = getValue(memory[pc + 1]);
			pc = getValue(a);
		}

		// 7 JT
		//   if <a> is nonzero, jump to <b>
		else if(instr == Opcode::JT) {
			unsigned short a = getValue(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			if(a != 0) pc = b;
			else pc += 3;
		}

		// 8 JF
		//   if <a> is zero, jump to <b>
		else if(instr == Opcode::JF) {
			unsigned short a = getValue(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			if(a == 0) pc = b;
			else pc += 3;
		}

		// 9 ADD
		//   assign into <a> the sum of <b> and <c> (modulo 32768)
		else if(instr == Opcode::ADD) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);

			unsigned short result = (b + c) % MODULO;
			registers[a] = result;
			pc += 4;
		}

		// 10 MULT
		//   store into <a> the product of <b> and <c> (modulo 32768)
		else if(instr == Opcode::MULT) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);

			unsigned short result = (b * c) % MODULO;
			registers[a] = result;
			pc += 4;
		}

		// 11 MOD
		//   store into <a> the remainder of <b> divided by <c>
		else if(instr == Opcode::MOD) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);

			unsigned short result = b % c;
			registers[a] = result;
			pc += 4;
		}

		// 12 AND
		//   stores into <a> the bitwise and of <b> and <c>
		else if(instr == Opcode::AND) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);

			unsigned short result = b & c;
			registers[a] = result;
			pc += 4;
		}

		// 13 OR
		//   stores into <a> the bitwise or of <b> and <c>
		else if(instr == Opcode::OR) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);
			unsigned short c = getValue(memory[pc + 3]);

			unsigned short result = b | c;
			registers[a] = result;
			pc += 4;
		}

		// 14 NOT
		//   stores 15-bit bitwise inverse of <b> in <a>
		else if(instr == Opcode::NOT) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);

			unsigned short saved = b >> 16;
			b = ~b;

			// this bit setting sucks, but well
			if(saved == 0)
				b &= 0b0111111111111111;
			else
				b |= 0b1000000000000000;

			registers[a] = b;
			pc += 3;
		}

		// 15 RMEM
		//   read memory at address <b> and write it to <a>
		//   TODO with memory they mean actual program memory?
		else if(instr == Opcode::RMEM) {
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);

			registers[a] = memory[b];
			pc += 3;
		}

		// 16 WMEM
		//   write the value from <b> into memory at address <a>
		//   TODO with memory they mean actual program memory?
		else if(instr == Opcode::WMEM) {
			unsigned short a = getValue(memory[pc + 1]);
			unsigned short b = getValue(memory[pc + 2]);

			memory[a] = b;
			pc += 3;
		}

		// 17 CALL
		//   write the address of the next instruction to the stack and jump to <a>
		else if(instr == Opcode::CALL) {
			unsigned short a = getValue(memory[pc + 1]);
			stack.push(pc + 2);
			pc = a;
		}

		// 18 RET
		//   remove the top element from the stack and jump to it; empty stack = halt
		else if(instr == Opcode::RET) {
			if(stack.size() == 0) {
				printf("HALTING, returning at empty stack\n");
				return 0;
			}
			pc = getValue(stack.top());
			stack.pop();
		}

		// 19 OUT
		//   write the character represented by ascii code <a> to the terminal
		else if(instr == Opcode::OUT) {
			unsigned short a = getValue(memory[pc + 1]);
			printf("%c", a);
			pc += 2;
		}

		// 20 IN
		//   read a character from the terminal and write its ascii code to <a>; it can be assumed that once input starts, it will continue until a newline is encountered; this means that you can safely read whole lines from the keyboard and trust that they will be fully read
		else if(instr == Opcode::IN) {
			if(inputBuffer.size() == 0) {
				char buff[64];
				fgets(buff, 64, stdin);
				inputBuffer = std::string(buff);
			}
			unsigned short a = getRegisterIdx(memory[pc + 1]);
			unsigned short current = static_cast<unsigned short>(inputBuffer[0]);
			registers[a] = current;
			inputBuffer = inputBuffer.substr(1);
			pc += 2;
		}

		// 21 NOOP
		//   no operation
		else if(instr == Opcode::NOOP) {
			pc += 1;
		}

		// ERROR
		else {

			printf("ERROR, unknown opcode: %d\nPC: %d", instr, pc);
			return 1;
		}
	}


	return 0;
}

