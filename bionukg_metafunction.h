#pragma once
#include<stdio.h>
#include<stdint.h>
#include<windows.h>

union funcptr
{
	void(*v_vf)(void);
	void(*v_vpf)(void*);
	void(*v_vp_vpf)(void*, void*);
	void* (*vp_vp_vpf)(void*, void*);
};
class metafunction
{
private:
	funcptr fnp;
	uint16_t size = 0;
	char name[256] = { 0 };
	uint8_t* funcmem = 0;
	int readBYTE(FILE* fp,_In_ LPVOID vbuf,int offset)
	{
		uint8_t* buf = (uint8_t*)vbuf+offset;
		char temp;
		int rd, rcnt = 0;
		rd = fread(&temp, 1, 1, fp);

		while (rd == 1)
		{
			if (temp >= '0' && temp <= '9')
			{
				*buf <<= 4;
				*buf |= temp - '0';
				rcnt++;
			}
			else if (temp >= 'a' && temp <= 'f')
			{
				*buf <<= 4;
				*buf |= temp - 'a' + 10;
				rcnt++;
			}
			else if (temp >= 'A' && temp <= 'F')
			{
				*buf <<= 4;
				*buf |= temp - 'A' + 10;
				rcnt++;
			}
			if (rcnt == 2)
			{
				return 1;
			}
			rd = fread(&temp, 1, 1, fp);
		}
		return 0;

	};
public:
	metafunction()
	{
		funcmem = (uint8_t*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
	metafunction(const char* dlc)
	{
		FILE* fp = fopen(dlc, "rb");
		for (size_t i = 0; i < 256; i++)
		{
			fread(&name[i], 1, 1, fp);
			if (name[i] == '\n'|| name[i] == '\r')
				break;
		}
		funcmem = (uint8_t*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		for (size = 0; size < 0x1000; size++)
		{
			int ed = readBYTE(fp, funcmem, size);
			if (ed == 0)
				break;
		}
		fclose(fp);
	}
	~metafunction()
	{
		if (funcmem != 0)
			VirtualFree(funcmem, 0, MEM_RELEASE);
	}
	void getfunc(void* funcp)
	{
		char c[4] = { 0xc3,0xcc,0xcc,0xcc };
		getfunc(funcp, c);
	}
	void getfunc(void* funcp, const char endBYTE[4])
	{
		for (size = 0; size < 0x1000; size++)
		{
			funcmem[size] = ((uint8_t*)funcp)[size];
			if (*(uint32_t*)&((uint8_t*)funcp)[size] == *(uint32_t*)endBYTE)
			{
				size++;
				funcmem[size] = ((uint8_t*)funcp)[size];
				size++;
				funcmem[size] = ((uint8_t*)funcp)[size];
				size++;
				funcmem[size] = ((uint8_t*)funcp)[size];
				size++;
				break;
			}
		}
	}
	void savefunc(const char* path)
	{
		FILE* fp = fopen(path, "wb+");
		for (size_t i = 0; name[i]!=0&&i<256; i++)
		{
			fwrite(name, 1, 1, fp);
		}
		fwrite("\n", 1, 1, fp);
		for (size_t i = 0; i < size; i++)
		{
			uint8_t buf[4] = { 0 };
			if ((i & 0xf) == 0xf)
			{
				buf[2] = '\n';
			}
			else
			{
				buf[2] = ' ';
			}

			buf[0] = this->funcmem[i] >> 4;
			buf[1] = this->funcmem[i] & 0x0f;

			for (size_t i = 0; i < 2; i++)
			{
				if (0<=buf[i]&&buf[i] <= 9)
				{
					buf[i] += '0';
				}
				else if (10 <= buf[i] && buf[i] <= 15)
				{
					buf[i] += 'A'-10;
				}
			}
			fwrite(buf, 1, 3, fp);
		}
		fclose(fp);

	}
private:

};
//ref:https://www.intel.cn/content/www/cn/zh/architecture-and-technology/64-ia-32-architectures-software-developer-vol-1-manual.html
//Accumulator for operands and results data
union reg0 {
	uint32_t eax;
	uint16_t ax;
	struct u8 {
		uint8_t al;
		uint8_t ah;
	};
};
//¡ª Pointer to data in the DS segment
union reg1 {
	uint32_t ebx;
	uint16_t bx;
	struct u8 {
		uint8_t bl;
		uint8_t bh;
	};
}; 
//¡ª Counter for string and loop operations
union reg2 {
	uint32_t ecx;
	uint16_t cx;
	struct u8 {
		uint8_t cl;
		uint8_t ch;
	};
};
//¡ª I/O pointer
union reg3 {
	uint32_t edx;
	uint16_t dx;
	struct u8 {
		uint8_t dl;
		uint8_t dh;
	};
};
// Pointer to data in the segment pointed to by the DS register; source pointer for string operations
union reg4 {
	uint32_t ebp;
	uint16_t bp;
};
//Pointer to data (or destination) in the segment pointed to by the ES register; 
//destination pointer for string operations
union reg5 {
	uint32_t esi;
	uint16_t si;
};
//Stack pointer (in the SS segment)
union reg6 {
	uint32_t edi;
	uint16_t di;
};
//Pointer to data on the stack (in the SS segment)
union reg7 {
	uint32_t esp;
	uint16_t sp;
};
class disasm_x86
{
	uint8_t* codemem;
	char txt[256] = { 0 };
public:
	void dasm(uint8_t* mem)
	{
		switch ((mem[0]>>4)&0x0f)
		{
		case 0x0:
			switch (mem[0] & 0x0f)
			{
			case 0x0://add	8,8
				break;
			case 0x1://add	32,32
				break;
			case 0x2://add	8,8(swap)
				break;
			case 0x3://add	32,32(swap)
				break;
			case 0x4://add	8,imm8
				break;
			case 0x5://add	32,imm32
				break;
			case 0x6://?
				break;
			case 0x7://?
				break;
			case 0x8://or	8,8
				break;
			case 0x9://or	32,32
				break;
			case 0xa://or	8,8(swap)
				break;
			case 0xb://or	32,32(swap)
				break;
			case 0xc://or	8,imm8
				break;
			case 0xd://or	32,imm32
				break;
			case 0xe://?
				break;
			case 0xf://bswap 32
				break;
			}
			break;
		case 0x1:
			switch (mem[0] & 0x0f)
				{
				case 0x0://adc	8,8
					break;
				case 0x1://adc	32,32
					break;
				case 0x2://adc	8,8(swap)
					break;
				case 0x3://adc	32,32
					break;
				case 0x4://adc	8,imm8
					break;
				case 0x5://adc	32,imm32
					break;
				case 0x6://?
					break;
				case 0x7://?
					break;
				case 0x8://sbb	8,8
					break;
				case 0x9://sbb	32,32
					break;
				case 0xa://sbb	8,8(swap)
					break;
				case 0xb://sbb	32,32(swap)
					break;
				case 0xc://sbb	8,imm8
					break;
				case 0xd://sbb	32,imm32
					break;
				case 0xe://?
					break;
				case 0xf://?
					break;
				}			
			break;
		case 0x2:
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			break;
		case 0x6:
			break;
		case 0x7:
			break;
		case 0x8:
			break;
		case 0x9:
			break;
		case 0xa:
			break;
		case 0xb:
			break;
		case 0xc:
			break;
		case 0xd:
			break;
		case 0xe:
			break;
		case 0xf:
			switch (mem[0] & 0x0f)
			{
			case 0x0:
				break;
			case 0x1:
				break;
			case 0x2:
				break;
			case 0x3:
				break;
			case 0x4:
				break;
			case 0x5:
				break;
			case 0x6:
				break;
			case 0x7:
				break;
			case 0x8:
				break;
			case 0x9:
				break;
			case 0xa:
				break;
			case 0xb:
				break;
			case 0xc:
				break;
			case 0xd:
				break;
			case 0xe:
				break;
			case 0xf:
				break;
			}
			break;
		default:
			break;
		}
	}
	void tasm()
	{
		auto funcmem = (uint8_t*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		for (size_t i = 0; i < 0xff; i++)
		{
			((uint32_t*)funcmem)[2*i] = 0xcccccccc;
			((uint32_t*)funcmem)[2 * i+1] = 0xcccccccc;

			funcmem[8 * i] = i;
		}
		system("pause");
		system("pause");

		VirtualFree(funcmem, 0, MEM_RELEASE);
	}

};
class asm_x86 {
	const char* reg8[8] = { "al","ah","bl","bh","cl","ch","dl","dh" };
	const char* reg16[8] = { "ax","bx","cx","dx","bp","si","di","sp" };
	const char* opcode2[12] = { "add","adc","sub","sbb","and","or","xor","cmp","teat","xchg","mov","arpl"};
	const char* opcode1[8] = { "push","pop","bswap","inc","dec"};
	const char* opcode0[12] = {"daa","das","aaa","pushad","popad","cwde","cdq","wait","pushfd","popfd","sahf","lahf"};
};