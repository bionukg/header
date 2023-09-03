#pragma once
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#ifndef bionukg_metafunction_h
#define bionukg_metafunction_h

template <class POINTER>
void rw_valloc(SIZE_T counts, POINTER &ptr)
{
	ptr = (POINTER)VirtualAlloc(0, counts * sizeof(*ptr), MEM_COMMIT, PAGE_READWRITE);
}
template <class POINTER>
void rwe_valloc(SIZE_T counts, POINTER &ptr)
{
	ptr = (POINTER)VirtualAlloc(0, counts * sizeof(*ptr), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
};
BOOL vfree(void *ptr)
{
	return VirtualFree(ptr, 0, MEM_RELEASE);
};

union funcptr
{
	void (*v_vf)(void);
	void (*v_vpf)(void *);
	void (*v_vp_vpf)(void *, void *);
	void *(*vp_vp_vpf)(void *, void *);
};
class metafunction
{
private:
	funcptr fnp;
	uint16_t size = 0;
	char name[256] = {0};
	uint8_t *funcmem = 0;
	int readBYTE(FILE *fp, _In_ LPVOID vbuf, int offset)
	{
		uint8_t *buf = (uint8_t *)vbuf + offset;
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
		funcmem = (uint8_t *)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
	metafunction(const char *dlc)
	{
		FILE *fp = fopen(dlc, "rb");
		for (size_t i = 0; i < 256; i++)
		{
			fread(&name[i], 1, 1, fp);
			if (name[i] == '\n' || name[i] == '\r')
				break;
		}
		funcmem = (uint8_t *)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (funcmem == 0)
		{
			throw "oom";
		}
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
	void getfunc(void *funcp)
	{
		char c[4] = {0xc3, 0xcc, 0xcc, 0xcc};
		getfunc(funcp, c);
	}
	void getfunc(void *funcp, const char endBYTE[4])
	{
		for (size = 0; size < 0x1000; size++)
		{
			funcmem[size] = ((uint8_t *)funcp)[size];
			if (*(uint32_t *)&((uint8_t *)funcp)[size] == *(uint32_t *)endBYTE)
			{
				size++;
				funcmem[size] = ((uint8_t *)funcp)[size];
				size++;
				funcmem[size] = ((uint8_t *)funcp)[size];
				size++;
				funcmem[size] = ((uint8_t *)funcp)[size];
				size++;
				break;
			}
		}
	}
	void savefunc(const char *path)
	{
		FILE *fp = fopen(path, "wb+");
		for (size_t i = 0; name[i] != 0 && i < 256; i++)
		{
			fwrite(name, 1, 1, fp);
		}
		fwrite("\n", 1, 1, fp);
		for (size_t i = 0; i < size; i++)
		{
			uint8_t buf[4] = {0};
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
				if (0 <= buf[i] && buf[i] <= 9)
				{
					buf[i] += '0';
				}
				else if (10 <= buf[i] && buf[i] <= 15)
				{
					buf[i] += 'A' - 10;
				}
			}
			fwrite(buf, 1, 3, fp);
		}
		fclose(fp);
	}

private:
};
// ref:https://www.intel.cn/content/www/cn/zh/architecture-and-technology/64-ia-32-architectures-software-developer-vol-1-manual.html
// Accumulator for operands and results data
union reg0
{
	uint32_t eax;
	uint16_t ax;
	struct u8
	{
		uint8_t al;
		uint8_t ah;
	};
};
// ?? Pointer to data in the DS segment
union reg1
{
	uint32_t ebx;
	uint16_t bx;
	struct u8
	{
		uint8_t bl;
		uint8_t bh;
	};
};
// ?? Counter for string and loop operations
union reg2
{
	uint32_t ecx;
	uint16_t cx;
	struct u8
	{
		uint8_t cl;
		uint8_t ch;
	};
};
// ?? I/O pointer
union reg3
{
	uint32_t edx;
	uint16_t dx;
	struct u8
	{
		uint8_t dl;
		uint8_t dh;
	};
};
// Pointer to data in the segment pointed to by the DS register; source pointer for string operations
union reg4
{
	uint32_t ebp;
	uint16_t bp;
};
// Pointer to data (or destination) in the segment pointed to by the ES register;destination pointer for string operations
union reg5
{
	uint32_t esi;
	uint16_t si;
};
// Stack pointer (in the SS segment)
union reg6
{
	uint32_t edi;
	uint16_t di;
};
// Pointer to data on the stack (in the SS segment)
union reg7
{
	uint32_t esp;
	uint16_t sp;
};
union reg8
{
	uint32_t eip;
};
struct bit32
{
	uint8_t b00 : 1; // low bit
	uint8_t b01 : 1;
	uint8_t b02 : 1;
	uint8_t b03 : 1;
	uint8_t b04 : 1;
	uint8_t b05 : 1;
	uint8_t b06 : 1;
	uint8_t b07 : 1;
	uint8_t b08 : 1;
	uint8_t b09 : 1;
	uint8_t b10 : 1;
	uint8_t b11 : 1;
	uint8_t b12 : 1;
	uint8_t b13 : 1;
	uint8_t b14 : 1;
	uint8_t b15 : 1;
	uint8_t b16 : 1;
	uint8_t b17 : 1;
	uint8_t b18 : 1;
	uint8_t b19 : 1;
	uint8_t b20 : 1;
	uint8_t b21 : 1;
	uint8_t b22 : 1;
	uint8_t b23 : 1;
	uint8_t b24 : 1;
	uint8_t b25 : 1;
	uint8_t b26 : 1;
	uint8_t b27 : 1;
	uint8_t b28 : 1;
	uint8_t b29 : 1;
	uint8_t b30 : 1;
	uint8_t b31 : 1; // high bit
};
enum eflags
{
	CF = 0, // Carry Flag
	/*1*/
	PF = 2, // Parity Flag
	/*0*/
	AF = 4, // Auxiliary Carry Flag
	/*0*/
	ZF = 6, // Zero Flag
	SF,		// Sign Flag
	TF,		// Trap Flag
	IF,		// Interrupt Enable Flag
	DF,		// Direction Flag
	OF,		// Overflow Flag
	IOPL0,	// I/O Privilege Level
	IOPL1,
	NT, // Nested Task
	/*0*/
	RF = 16, // Resume Flag
	VM,		 // Virtual-8086 Mode
	AC,		 // Alignment Check / Access Control
	VIF,	 // Virtual Interrupt Flag
	VIP,	 // Virtual Interrupt Pending
	ID,		 // ID Flag
	/*0*/
};
struct EFL32
{
	bool CF : 1; // low bit
	bool b01 : 1;
	bool PF : 1;
	bool b03 : 1;
	bool AF : 1;
	bool b05 : 1;
	bool ZF : 1;
	bool SF : 1;
	bool TF : 1;
	bool IF : 1;
	bool DF : 1;
	bool OF : 1;
	bool IOPL0 : 1;
	bool IOPL1 : 1;
	bool NT : 1;
	bool b15 : 1;
	bool RF : 1;
	bool VM : 1;
	bool AC : 1;
	bool VIF : 1;
	bool VIP : 1;
	bool ID : 1;
	bool b22 : 1;
	bool b23 : 1;
	bool b24 : 1;
	bool b25 : 1;
	bool b26 : 1;
	bool b27 : 1;
	bool b28 : 1;
	bool b29 : 1;
	bool b30 : 1;
	bool b31 : 1; // high bit
};

union reg9
{
	uint32_t efl;
	struct bit32 bits;
	struct EFL32 FL;
};
class disasm_x86
{
	uint8_t *codemem;
	char txt[256] = {0};

public:
	void dasm(uint8_t *mem)
	{
		switch ((mem[0] >> 4) & 0x0f)
		{
		case 0x0:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // add	8,8
				break;
			case 0x1: // add	32,32
				break;
			case 0x2: // add	8,8(swap)
				break;
			case 0x3: // add	32,32(swap)
				break;
			case 0x4: // add	8,imm8
				break;
			case 0x5: // add	32,imm32
				break;
			case 0x6: /// push es
				break;
			case 0x7: /// pop es
				break;
			case 0x8: // or	8,8
				break;
			case 0x9: // or	32,32
				break;
			case 0xa: // or	8,8(swap)
				break;
			case 0xb: // or	32,32(swap)
				break;
			case 0xc: // or	8,imm8
				break;
			case 0xd: // or	32,imm32
				break;
			case 0xe: /// push cs
				break;
			case 0xf: // bswap 32
				break;
			}
			break;
		case 0x1:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // adc	8,8
				break;
			case 0x1: // adc	32,32
				break;
			case 0x2: // adc	8,8(swap)
				break;
			case 0x3: // adc	32,32(swap)
				break;
			case 0x4: // adc	8,imm8
				break;
			case 0x5: // adc	32,imm32
				break;
			case 0x6: /// push ss
				break;
			case 0x7: /// pop ss
				break;
			case 0x8: // sbb	8,8
				break;
			case 0x9: // sbb	32,32
				break;
			case 0xa: // sbb	8,8(swap)
				break;
			case 0xb: // sbb	32,32(swap)
				break;
			case 0xc: // sbb	8,imm8
				break;
			case 0xd: // sbb	32,imm32
				break;
			case 0xe: /// push ds
				break;
			case 0xf: // pop ds
				break;
			}
			break;
		case 0x2:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // and 8,8
				break;
			case 0x1: // and 32,32
				break;
			case 0x2: // and 8,8(swap)
				break;
			case 0x3: // and 32,32(swap)
				break;
			case 0x4: // and 8,imm8
				break;
			case 0x5: // and 32,imm32
				break;
			case 0x6: //?
				break;
			case 0x7: // daa
				break;
			case 0x8: // sub 8,8
				break;
			case 0x9: // sub 32,32
				break;
			case 0xa: // sub 8,8(swap)
				break;
			case 0xb: // sub 32,32(swap)
				break;
			case 0xc: // sub 8,imm8
				break;
			case 0xd: // sub 32,imm32
				break;
			case 0xe: //?
				break;
			case 0xf: // das
				break;
			}
			break;
		case 0x3:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // xor 8,8
				break;
			case 0x1: // xor 32,32
				break;
			case 0x2: // xor 8,8(swap)
				break;
			case 0x3: // xor 32,32(swap)
				break;
			case 0x4: // xor 8,imm8
				break;
			case 0x5: // xor 32,imm32
				break;
			case 0x6: //?
				break;
			case 0x7: /// aaa
				break;
			case 0x8: // cmp 8,8
				break;
			case 0x9: // cmp 32,32
				break;
			case 0xa: // cmp 8,8(swap)
				break;
			case 0xb: // cmp 32,32(swap)
				break;
			case 0xc: // cmp 8,imm8
				break;
			case 0xd: // cmp 32,imm32
				break;
			case 0xe: //?
				break;
			case 0xf: /// aas
				break;
			}
			break;
		case 0x4:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // inc eax
				break;
			case 0x1: // inc ecx
				break;
			case 0x2: // inc edx
				break;
			case 0x3: // inc ebx
				break;
			case 0x4: // inc esp
				break;
			case 0x5: // inc ebp
				break;
			case 0x6: // inc esi
				break;
			case 0x7: // inc edi
				break;
			case 0x8: // dec eax
				break;
			case 0x9: // dec ecx
				break;
			case 0xa: // dec edx
				break;
			case 0xb: // dec ebx
				break;
			case 0xc: // dec esp
				break;
			case 0xd: // dec ebp
				break;
			case 0xe: // dec esi
				break;
			case 0xf: // dec edi
				break;
			}
			break;
		case 0x5:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // push eax
				break;
			case 0x1: // push ecx
				break;
			case 0x2: // push edx
				break;
			case 0x3: // push ebx
				break;
			case 0x4: // push esp
				break;
			case 0x5: // push ebp
				break;
			case 0x6: // push esi
				break;
			case 0x7: // push edi
				break;
			case 0x8: // pop eax
				break;
			case 0x9: // pop ecx
				break;
			case 0xa: // pop edx
				break;
			case 0xb: // pop ebx
				break;
			case 0xc: // pop esp
				break;
			case 0xd: // pop ebp
				break;
			case 0xe: // pop esi
				break;
			case 0xf: // pop edi
				break;
			}
			break;
		case 0x6:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // pushad
				break;
			case 0x1: // popad
				break;
			case 0x2: //?bound
				break;
			case 0x3: // arpl
				break;
			case 0x4:
				break;
			case 0x5:
				break;
			case 0x6: // 32-16reg prefix
				break;
			case 0x7:
				break;
			case 0x8: // push imm32
				break;
			case 0x9: // imul 32,32,imm32
				break;
			case 0xa: // push imm8
				break;
			case 0xb: // imul 32,32,imm8
				break;
			case 0xc: // ins m8 es:[edi],dx
				break;
			case 0xd: // ins m32 es:[edi],dx
				break;
			case 0xe: // outs dx,m8 [esi]
				break;
			case 0xf: // outs dx,m8 [esi]
				break;
			}
			break;
		case 0x7:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // jo rel8
				break;
			case 0x1: // jno rel8
				break;
			case 0x2: // jb/jnae rel8
				break;
			case 0x3: // jae/jnb rel8
				break;
			case 0x4: // je rel8
				break;
			case 0x5: // jne rel8
				break;
			case 0x6: // jbe/jna rel8
				break;
			case 0x7: // ja/jnbe rel8
				break;
			case 0x8: // js rel8
				break;
			case 0x9: // jns rel8
				break;
			case 0xa: // jp
				break;
			case 0xb: // jnp
				break;
			case 0xc: // jl
				break;
			case 0xd: // jge
				break;
			case 0xe: // jle
				break;
			case 0xf: // jg
				break;
			}

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
		auto funcmem = (uint8_t *)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (funcmem == 0)
		{
			system("pause");
			return;
		}
		for (size_t i = 0; i < 0xff; i++)
		{
			((uint32_t *)funcmem)[2 * i] = 0xccccccc84;
			((uint32_t *)funcmem)[2 * i + 1] = 0xcccccccc;

			funcmem[8 * i + 1] = i;
			// funcmem[8 * i + 2] = i;
		}

		system("pause");
		system("pause");
		VirtualFree(funcmem, 0, MEM_RELEASE);
	}
};
class asm_x86
{
	const char *reg8[8] = {"al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"};
	const char *reg16[8] = {"ax", "bx", "cx", "dx", "bp", "si", "di", "sp"};
	const char *opcode2[12] = {"add", "adc", "sub", "sbb", "and", "or", "xor", "cmp", "test", "xchg", "mov", "arpl"};
	const char *opcode1[8] = {"push", "pop", "bswap", "inc", "dec", "neg"};
	const char *opcode0[12] = {"daa", "das", "aaa", "pushad", "popad", "cwde", "cdq", "wait", "pushfd", "popfd", "sahf", "lahf"};
};

class asm_x86_min
{
public:
	const char *reg[8] = {"eax", "ebx", "ecx", "edx", "ebp", "esi", "edi", "esp"};
	const char *operand2[16] = {
		"add", "sub", "and", "or ", "xor",
		"cmp", "test",
		"mov", 0};
	const char *operaand1[16] = {"neg", "not"};
	FILE *fopen_rb(_In_ const char *path)
	{
		FILE *fp;
		fopen_s(&fp, path, "rb+");
		if (fp == 0)
			throw "fopen failure";
		return fp;
	}
	void *calljmp(void *p)
	{
		if (*(uint8_t *)p == 0xe9)
		{
			return (uint8_t *)p + *((uint32_t *)&(((uint8_t *)p)[1])) + 5;
		}
		return p;
	}
	void to_asm_text(const char *path)
	{
		FILE *fp;
		fp = fopen_rb(path);
		for (size_t i = 0; operand2[i] != 0; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				for (size_t k = 0; k < 8; k++)
				{
					fprintf(fp, "%s %s,%s\n", operand2[i], reg[j], reg[k]);
					// fprintf(fp, "int 3\nint 3\nint 3\nint 3\n");
					fprintf(fp, "%s [%s],%s\n", operand2[i], reg[j], reg[k]);
					// fprintf(fp, "int 3\nint 3\nint 3\nint 3\n");
					fprintf(fp, "%s %s,[%s]\n", operand2[i], reg[j], reg[k]);
					// fprintf(fp, "int 3\nint 3\nint 3\nint 3\n");
				}
			}
		}
		fclose(fp);
	}
	void to_disasm_text(const char *path, void *funcmem)
	{
		FILE *fp;
		fp = fopen_rb(path);
		uint8_t *fnbyte = (uint8_t *)calljmp(funcmem);
		size_t c = 0;
		size_t x = 0;
		while ((*(uint32_t *)&fnbyte[x] != 0xcccccccc))
		{
			x++;
		}
		for (;;)
		{
			if (*(uint32_t *)&fnbyte[x] == 0xcccccccc)
			{
				x += 4;
				c++;
				while ((*(uint32_t *)&fnbyte[x] != 0xcccccccc))
				{
					fprintf(fp, "%X%X ", fnbyte[x] >> 4, fnbyte[x] & 0xf);
					x++;
				}
				fprintf(fp, "\n");
			}
			else
			{
				system("pause");
			}
			if (c > 3 * 8 * 8 * 8)
			{
				goto to_disasm_text_end;
			}
		}
	to_disasm_text_end:
		fclose(fp);
	}

#define str32(a, b, c, d) ((uint32_t)(((uint8_t)d) << 24) | (((uint8_t)c) << 16) | (((uint8_t)b) << 8) | (((uint8_t)a) << 0))
	enum opcodestr
	{
		add_ = str32('a', 'd', 'd', ' '),
		addmr_ = str32('a', 'd', 'd', 'm'),
		addrm_ = str32('a', 'd', 'd', 'r'),
		sub_ = str32('s', 'u', 'b', ' '),
		submr_ = str32('s', 'u', 'b', 'm'),
		subrm_ = str32('s', 'u', 'b', 'r'),
		and_ = str32('a', 'n', 'd', ' '),
		andmr_ = str32('a', 'n', 'd', 'm'),
		andrm_ = str32('a', 'n', 'd', 'r'),
		or_ = str32('o', 'r', ' ', ' '),
		ormr_ = str32('o', 'r', ' ', 'm'),
		orrm_ = str32('o', 'r', ' ', 'r'),
		xor_ = str32('x', 'o', 'r', ' '),
		xormr_ = str32('x', 'o', 'r', 'm'),
		xorrm_ = str32('x', 'o', 'r', 'r'),
		cmp_ = str32('c', 'm', 'p', ' '),
		cmpmr_ = str32('c', 'm', 'p', 'm'),
		cmprm_ = str32('c', 'm', 'p', 'r'),
		test_ = str32('t', 'e', 's', ' '),
		testmr_ = str32('t', 'e', 's', 'm'),
		testrm_ = str32('t', 'e', 's', 'r'),
		mov_ = str32('m', 'o', 'v', ' '),
		movmr_ = str32('m', 'o', 'v', 'm'),
		movrm_ = str32('m', 'o', 'v', 'r'),
		jne_ = str32('j', 'n', 'e', ' ')
	};

	void opcodestr_producer(const char *path)
	{
		FILE *fp;
		fp = fopen_rb(path);

		for (size_t i = 0; operand2[i] != 0; i++)
		{
			fprintf(fp, "%s_ = str32('%c', '%c', '%c', ' '),\n", operand2[i], operand2[i][0], operand2[i][1], operand2[i][2]);
			fprintf(fp, "%smr_ = str32('%c', '%c', '%c', 'm'),\n", operand2[i], operand2[i][0], operand2[i][1], operand2[i][2]);
			fprintf(fp, "%srm_ = str32('%c', '%c', '%c', 'r'),\n", operand2[i], operand2[i][0], operand2[i][1], operand2[i][2]);
		}

		fclose(fp);
	}
	int asm_line(_In_ const char *asmcode_txt, _Out_writes_bytes_(32) char buffer[32])
	{
		size_t i = 0;
		memset(buffer, 0, 32);
		switch (opcodestr(*(uint32_t *)asmcode_txt))
		{
		case asm_x86_min::add_:
		case asm_x86_min::addmr_:
			buffer[i] = 0x01;
			break;
		case asm_x86_min::addrm_:
			buffer[i] = 0x03;
			break;
		case asm_x86_min::sub_:
		case asm_x86_min::submr_:
			buffer[i] = 0x29;
			break;
		case asm_x86_min::subrm_:
			buffer[i] = 0x2B;
			break;
		case asm_x86_min::and_:
		case asm_x86_min::andmr_:
			buffer[i] = 0x21;
			break;
		case asm_x86_min::andrm_:
			buffer[i] = 0x23;
			break;
		case asm_x86_min::or_:
		case asm_x86_min::ormr_:
			break;
		case asm_x86_min::orrm_:
			break;
		case asm_x86_min::xor_:
		case asm_x86_min::xormr_:
			break;
		case asm_x86_min::xorrm_:
			break;
		case asm_x86_min::cmp_:
		case asm_x86_min::cmpmr_:
			break;
		case asm_x86_min::cmprm_:
			break;
		case asm_x86_min::test_:
		case asm_x86_min::testmr_:
			break;
		case asm_x86_min::testrm_:
			break;
		case asm_x86_min::mov_:
		case asm_x86_min::movmr_:
			break;
		case asm_x86_min::movrm_:
			break;
		case asm_x86_min::jne_:
			buffer[i] = 0x75;
			break;

		default:
			break;
		}
	}
	int machine_code_write(uint8_t machinecode, void *mem)
	{
		*(uint8_t *)mem = machinecode;
		return 1;
	}
	int machine_code_writeBYTE(char *byte, void *mem)
	{
	}
#if defined(_bionukg_timer_h) && defined(_M_IX86) 
	int benchmark(const char *inst, int8_t len)
	{
		if (len < 0)
			return -1;
		uint8_t *p;
		size_t offset = 0;
		rwe_valloc(1024, p);

		memcpy(&p[offset], inst, len);
		offset += len;

		timer t0;

		__asm call p;

		auto r = t0.us_form_last();
		vfree(p);
	}
#endif
};

#ifdef bionukg_string_h
#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	class x86_asm_to_machine_code
	{
		string_<char> x86_asm(const char *asmcode_txt)
		{
			string_view_<char> asmcode(asmcode_txt);

			return asmcode;
		}

		void asm_preprocess(const char *asmcode_txt)
		{
			//remove repeat space, tab
			//delete comment, which start with ';', '#', '//';end with '\n'
		}
	};

#ifdef namespace_bionukg
};
#endif // namespace_bionukg



#endif // bionukg_string_h
// 私有协议，基于TCP
namespace protocol
{
#if defined(_WIN32) && defined(_M_IX86)
#define r0_ eax_
#define r1_ ebx_
#define r2_ ecx_
#define r3_ edx_
#elif defined(_WIN32) && defined(_M_X64)
#define r0_ rax_
#define r1_ rbx_
#define r2_ rcx_
#define r3_ rdx_
#endif
    const uint8_t exampleasm_add[] =
    {
        // 32位x86汇编
        // 传入参数, eax ,ebx
        // 返回值, eax
        // add eax,ebx
        0x01, 0xd8,
        // ret
        0xc3 };

    template <int maxlen_>
    struct machine_code
    {
    public:
        constexpr static const int maxlen = maxlen_;
        constexpr static const int size = maxlen_;
        uint8_t code[maxlen_] = { 0 };

        template <int thatlen_>
        machine_code<maxlen_ + thatlen_> operator+(const machine_code<thatlen_>& mc)
        {
            machine_code<maxlen_ + thatlen_> ret;
            memcpy(ret.code, this->code, maxlen_);
            memcpy(ret.code + maxlen_, mc.code, thatlen_);
            return ret;
        }

        machine_code(const uint8_t(&thatcode)[maxlen_])
        {
            memcpy(code, thatcode, maxlen_);
        }

        machine_code() = default;

        void print()
        {
            for (int i = 0; i < maxlen; i++)
            {
                printf("%02x ", code[i]);
            }
            printf("\n");
        }
    };

    namespace x86_assembler
    {

        enum ins_2op_5bit : uint8_t
        {
            add_ = 0x00 >> 3,
            加 = 0x00 >> 3,
            or_ = 0x08 >> 3,
            或 = 0x08 >> 3,
            adc_ = 0x10 >> 3,
            进位加 = 0x10 >> 3,
            sbb_ = 0x18 >> 3,
            借位减 = 0x18 >> 3,
            and_ = 0x20 >> 3,
            与 = 0x20 >> 3,
            sub_ = 0x28 >> 3,
            减 = 0x28 >> 3,
            xor_ = 0x30 >> 3,
            异或 = 0x30 >> 3,
            cmp_ = 0x38 >> 3,
            比较 = 0x38 >> 3,
            mov_ = 0x88 >> 3,
            置值 = 0x88 >> 3,
        };
        using 指令二作五 = ins_2op_5bit;
        enum mod_2op_3bit : uint8_t
        {
            // width_ = BYTE , right to left
            BLR_ = 0x00,
            BYTE_left_to_right = 0x00,
            字节左到右 = 0x00,
            // width_ = DWORD , right to left
            DLR_ = 0x01,
            DWORD_left_to_right = 0x01,
            双字左到右 = 0x01,
            // width_ = BYTE , left to right
            BRL_ = 0x02,
            BYTE_right_to_left = 0x02,
            字节右到左 = 0x02,
            // width_ = DWORD , left to right
            DRL_ = 0x03,
            DWORD_right_to_left = 0x03,
            双字右到左 = 0x03,
            // width_ = BYTE , imm8 to reg
            BIR_ = 0x04,
            BYTE_imm8_to_al = 0x04,
            字节立即数到甲 = 0x04,
            // width_ = DWORD , imm32 to reg
            DIR_ = 0x05,
            DWORD_imm32_to_eax = 0x05,
            双字立即数到甲 = 0x05,
        };
        using 修饰二作三 = mod_2op_3bit;
        enum type_2op_2bit : uint8_t
        {
            // p for pointer(m for memory), r for register, i for immediate added on pointer
            r_p_ = 0x00,
            reg_ptr = 0x00,
            寄存器和指针 = 0x00,
            r_m_ = 0x00,
            reg_mem = 0x00,
            寄存器和内存 = 0x00,
            r_i8 = 0x01,
            reg_imm8 = 0x01,
            寄存器和八位立即数 = 0x01,
            r_i32 = 0x02,
            reg_imm32 = 0x02,
            寄存器和三十二位立即数 = 0x02,
            r_r_ = 0x03,
            reg_reg = 0x03,
            寄存器和寄存器 = 0x03,
        };
        using 类型二作二 = type_2op_2bit;
        enum reg_order_3bit : uint8_t
        {
            reg0_ = 0x00,
            al_ = 0x00,
            eax_ = 0x00,
            rax_ = 0x00,
            甲 = 0x00,
            寄存器甲 = 0x00,

            reg1_ = 0x01,
            cl_ = 0x01,
            ecx_ = 0x01,
            rcx_ = 0x01,
            乙 = 0x01,
            寄存器乙 = 0x01,

            reg2_ = 0x02,
            dl_ = 0x02,
            edx_ = 0x02,
            rdx_ = 0x02,
            丙 = 0x02,
            寄存器丙 = 0x02,

            reg3_ = 0x03,
            bl_ = 0x03,
            ebx_ = 0x03,
            rbx_ = 0x03,
            丁 = 0x03,
            寄存器丁 = 0x03,

            reg4_ = 0x04,
            ah_ = 0x04,
            esp_ = 0x04,
            rsp_ = 0x04,
            戊 = 0x04,
            寄存器戊 = 0x04,
            栈指针 = 0x04,

            reg5_ = 0x05,
            ch_ = 0x05,
            ebp_ = 0x05,
            rbp_ = 0x05,
            己 = 0x05,
            寄存器己 = 0x05,
            基指针 = 0x05,

            reg6_ = 0x06,
            dh_ = 0x06,
            esi_ = 0x06,
            rsi_ = 0x06,
            庚 = 0x06,
            寄存器庚 = 0x06,
            源索引 = 0x06,

            reg7_ = 0x07,
            bh_ = 0x07,
            edi_ = 0x07,
            rdi_ = 0x07,
            辛 = 0x07,
            寄存器辛 = 0x07,
            终索引 = 0x07,
        };
        using 寄存器 = reg_order_3bit;
        enum ins_1byte_8bit : uint8_t
        {
            push_es_ = 0x06,
            pop_es_ = 0x07,
            push_cs_ = 0x0e,
            // pop_cs_ = 0x0f,//
            push_ss_ = 0x16,
            pop_ss_ = 0x17,
            push_ds_ = 0x1e,
            pop_ds_ = 0x1f,
            daa_ = 0x27, // decimal adjust al after add
            加法后十进制调整 = 0x27,
            das_ = 0x2f, // decimal adjust al after sub
            减法后十进制调整 = 0x2f,
            aaa_ = 0x37, // ascii adjust al after add
            加法后字符调整 = 0x37,
            aas_ = 0x3f, // ascii adjust al after sub
            减法后字符调整 = 0x3f,
            inc_eax_ = 0x40,
            三十二位甲自增 = 0x40,
            inc_ecx_ = 0x41,
            三十二位乙自增 = 0x41,
            inc_edx_ = 0x42,
            三十二位丙自增 = 0x42,
            inc_ebx_ = 0x43,
            三十二位丁自增 = 0x43,
            inc_esp_ = 0x44,
            三十二位戊自增 = 0x44,
            inc_ebp_ = 0x45,
            三十二位己自增 = 0x45,
            inc_esi_ = 0x46,
            三十二位庚自增 = 0x46,
            inc_edi_ = 0x47,
            三十二位辛自增 = 0x47,
            dec_eax_ = 0x48,
            三十二位甲自减 = 0x48,
            dec_ecx_ = 0x49,
            三十二位乙自减 = 0x49,
            dec_edx_ = 0x4a,
            三十二位丙自减 = 0x4a,
            dec_ebx_ = 0x4b,
            三十二位丁自减 = 0x4b,
            dec_esp_ = 0x4c,
            三十二位戊自减 = 0x4c,
            dec_ebp_ = 0x4d,
            三十二位己自减 = 0x4d,
            dec_esi_ = 0x4e,
            三十二位庚自减 = 0x4e,
            dec_edi_ = 0x4f,
            三十二位辛自减 = 0x4f,
            push_eax_ = 0x50,
            三十二位甲入栈 = 0x50,
            push_ecx_ = 0x51,
            三十二位乙入栈 = 0x51,
            push_edx_ = 0x52,
            三十二位丙入栈 = 0x52,
            push_ebx_ = 0x53,
            三十二位丁入栈 = 0x53,
            push_esp_ = 0x54, // sp is stack pointer
            三十二位戊入栈 = 0x54,
            栈指针入栈 = 0x54,
            push_ebp_ = 0x55, // bp is base pointer
            三十二位己入栈 = 0x55,
            基指针入栈 = 0x55,
            push_esi_ = 0x56, // si is source index
            三十二位庚入栈 = 0x56,
            源索引入栈 = 0x56,
            push_edi_ = 0x57, // di is destination index
            三十二位辛入栈 = 0x57,
            终索引入栈 = 0x57,
            pop_eax_ = 0x58,
            三十二位甲出栈 = 0x58,
            pop_ecx_ = 0x59,
            三十二位乙出栈 = 0x59,
            pop_edx_ = 0x5a,
            三十二位丙出栈 = 0x5a,
            pop_ebx_ = 0x5b,
            三十二位丁出栈 = 0x5b,
            pop_esp_ = 0x5c,
            三十二位戊出栈 = 0x5c,
            栈指针出栈 = 0x5c,
            pop_ebp_ = 0x5d,
            三十二位己出栈 = 0x5d,
            基指针出栈 = 0x5d,
            pop_esi_ = 0x5e,
            三十二位庚出栈 = 0x5e,
            源索引出栈 = 0x5e,
            pop_edi_ = 0x5f,
            三十二位辛出栈 = 0x5f,
            终索引出栈 = 0x5f,
            pushad_ = 0x60, // push eax,ecx,edx,ebx,esp,ebp,esi,edi
            三十二位通用寄存器入栈 = 0x60,
            popad_ = 0x61,
            三十二位通用寄存器出栈 = 0x61,
            arpl_ = 0x63, // adjust rpl of segment selector
            调整段选择子特权级 = 0x63,
            WORD_prefix = 0x66, // change 32bit operand to 16bit
            字前缀 = 0x66,
            nop_ = 0x90,
            no_operation = 0x90,
            无操作 = 0x90,
            xchg_eax_eax_ = 0x90, // exchange self is no effect
            xchg_eax_ecx_ = 0x91,
            交换三十二位甲乙 = 0x91,
            xchg_eax_edx_ = 0x92,
            交换三十二位甲丙 = 0x92,
            xchg_eax_ebx_ = 0x93,
            交换三十二位甲丁 = 0x93,
            xchg_eax_esp_ = 0x94,
            交换三十二位甲戊 = 0x94,
            xchg_eax_ebp_ = 0x95,
            交换三十二位甲己 = 0x95,
            xchg_eax_esi_ = 0x96,
            交换三十二位甲庚 = 0x96,
            xchg_eax_edi_ = 0x97,
            交换三十二位甲辛 = 0x97,
            cwde_ = 0x98, // mov ax to eax, sign extend
            // convert word to double word
            符号拓展字到双字 = 0x98,
            cdq_ = 0x99, // mov eax to edx:eax, sign extend
            // convert double word to quad word
            符号拓展双字到四字 = 0x99,
            wait_ = 0x9b, // wait for coprocessor
            等待协处理器 = 0x9b,
            pushfd_ = 0x9c, // push flags
            标志寄存器入栈 = 0x9c,
            popfd_ = 0x9d, // pop flags
            标志寄存器出栈 = 0x9d,
            sahf_ = 0x9e, // store ah to flags
            甲从标志寄存器存入 = 0x9e,
            lahf_ = 0x9f, // load flags to ah
            甲从标志寄存器取出 = 0x9f,
            movsb_ = 0xa4, // mov byte from ds:[esi] to es:[edi]
            复制源字节到目的字节 = 0xa4,
            movsd_ = 0xa5, // mov dword from ds:[esi] to es:[edi]
            复制源双字到目的双字 = 0xa5,
            cmpsb_ = 0xa6, // cmp byte from ds:[esi] to es:[edi]
            比较源字节与目的字节 = 0xa6,
            cmpsd_ = 0xa7, // cmp dword from ds:[esi] to es:[edi]
            比较源双字与目的双字 = 0xa7,
            ret_ = 0xc3, // return// pop eip
            返回 = 0xc3,
            retf_ = 0xcb, // return far// pop eip, pop cs
            远返回 = 0xcb,
            leave_ = 0xc9, // mov ebp to esp, pop ebp
            离开 = 0xc9,
            int3_ = 0xcc, // interrupt 3
            三号中断 = 0xcc,
            into_ = 0xce, // interrupt 4 if overflow
            溢出中断 = 0xce,
            iretd_ = 0xcf, // interrupt return
            中断返回 = 0xcf,
            xlat_ = 0xd7, // al = [ds:ebx+al]
            字节查表 = 0xd7,
            xlatb_ = 0xd7,      // same as xlat_
            lock_prefix = 0xf0, // lock the next instruction, make it atomic
            锁定前缀 = 0xf0,
            repne_prefix = 0xf2, // whlie(!ZF&&ecx!=0){ecx--;instruction;}
            重复非零前缀 = 0xf2,
            rep_prefix = 0xf3, // while(ecx!=0){ecx--;instruction;}
            重复前缀 = 0xf3,
            hlt_ = 0xf4, // halt
            暂停 = 0xf4,
            cmc_ = 0xf5, // CF = !CF
            取反进位标志 = 0xf5,
            clc_ = 0xf8, // CF = 0
            清零进位标志 = 0xf8,
            stc_ = 0xf9, // CF = 1
            置一进位标志 = 0xf9,
            cli_ = 0xfa, // IF = 0
            清零中断标志 = 0xfa,
            sti_ = 0xfb, // IF = 1
            置一中断标志 = 0xfb,
            cld_ = 0xfc, // DF = 0
            清零方向标志 = 0xfc,
            std_ = 0xfd, // DF = 1
            置一方向标志 = 0xfd,
        };
        using 指令单八 = ins_1byte_8bit;

        // instruction with a 8bit immediate or relative address
        enum ins_imm8_8bit : uint8_t
        {
            push_imm8_ = 0x6a,
            八位立即数入栈 = 0x6a,
            jo_ = 0x70, // jump if overflow(OF=1)
            溢出跳转 = 0x70,
            jno_ = 0x71,
            未溢出跳转 = 0x71,
            jb_ = 0x72, // jump if below(CF=1)
            无符号数小于跳转 = 0x72,
            jnb_ = 0x73,
            无符号数大于等于跳转 = 0x73,
            jz_ = 0x74, // jump if zero(ZF=1)
            为零跳转 = 0x74,
            je_ = 0x74,
            相等跳转 = 0x74,
            jnz_ = 0x75,
            非零跳转 = 0x75,
            jne_ = 0x75,
            不等跳转 = 0x75,
            jbe_ = 0x76, // jump if below or equal(CF=1 or ZF=1)
            无符号数小于等于跳转 = 0x76,
            jna_ = 0x76,
            ja_ = 0x77,
            无符号数大于跳转 = 0x77,
            jnbe_ = 0x77,
            js_ = 0x78, // jump if sign(SF=1)
            有符号数小于零跳转 = 0x78,
            jns_ = 0x79,
            有符号数大于等于零跳转 = 0x79,
            jp_ = 0x7a, // jump if parity(PF=1)
            奇偶校验为一跳转 = 0x7a,
            jnp_ = 0x7b,
            奇偶校验为零跳转 = 0x7b,
            jl_ = 0x7c, // jump if less(SF!=OF)
            有符号数小于跳转 = 0x7c,
            jnge_ = 0x7c,
            jge_ = 0x7d,
            有符号数大于等于跳转 = 0x7d,
            jnl_ = 0x7d,
            jle_ = 0x7e, // jump if less or equal(ZF=1 or SF!=OF)
            有符号数小于等于跳转 = 0x7e,
            jng_ = 0x7e,
            jg_ = 0x7f,
            有符号数大于跳转 = 0x7f,
            jnle_ = 0x7f,
            test_al_imm8_ = 0xa8,
            试甲与八位立即数 = 0xa8,
            mov_al_imm8_ = 0xb0,
            置甲以八位立即数 = 0xb0,
            mov_cl_imm8_ = 0xb1,
            置乙以八位立即数 = 0xb1,
            mov_dl_imm8_ = 0xb2,
            置丙以八位立即数 = 0xb2,
            mov_bl_imm8_ = 0xb3,
            置丁以八位立即数 = 0xb3,
            mov_ah_imm8_ = 0xb4,
            mov_ch_imm8_ = 0xb5,
            mov_dh_imm8_ = 0xb6,
            mov_bh_imm8_ = 0xb7,
            int_imm8_ = 0xcd,
            八位立即数中断 = 0xcd,
            aam_ = 0xd4, //{ah,al}={al/imm8,al%imm8}
            // ascii adjust ax after multiply
            乘法后字符调整 = 0xd4,
            aamb_ = 0xd4, // same as aam_
            aad_ = 0xd5,  //{ah,al}={0,ah*imm8+al}
            // ascii adjust ax after divide
            除法后字符调整 = 0xd5,
            aadb_ = 0xd5,   // same as aad_
            loopne_ = 0xe0, // if(ecx!=0&&ZF==0){ecx--;eip+=imm8}
            // loop not equal
            不等时循环 = 0xe0,
            loopnz_ = 0xe0, // same as loopne_
            // loop not zero
            非零时循环 = 0xe0,
            loope_ = 0xe1, // if(ecx!=0&&ZF==1){ecx--;eip+=imm8}
            // loop equal
            相等时循环 = 0xe1,
            loopz_ = 0xe1, // same as loope_
            // loop zero
            为零时循环 = 0xe1,
            loop_ = 0xe2, // if(ecx!=0){ecx--;eip+=imm8}
            循环 = 0xe2,
            jecxz_ = 0xe3, // if(ecx==0){eip+=imm8}
            乙为零跳转 = 0xe3,
            in_al_imm8_ = 0xe4,
            从八位立即数端口读入甲 = 0xe4,
            out_imm8_al_ = 0xe6,
            将甲写入八位立即数端口 = 0xe6,
            jmp_rel8_ = 0xeb, // eip+=imm8
            跳转到八位相对地址 = 0xeb,
        };
        using 指令立即字节 = ins_imm8_8bit;
        // 1BYTE instruction with a 32bit immediate or relative address or memory address
        enum ins_imm32_32bit : uint8_t
        {
            push_imm32_ = 0x68,
            mov_al_mem32_ = 0xa0,
            movb_al_mem32_ = 0xa0,
            mov_eax_mem32_ = 0xa1,
            movd_eax_mem32_ = 0xa1,
            mov_mem32_al_ = 0xa2,
            movb_mem32_al_ = 0xa2,
            mov_mem32_eax_ = 0xa3,
            movd_mem32_eax_ = 0xa3,
            test_eax_imm32_ = 0xa9,
            mov_eax_imm32_ = 0xb8,
            mov_ecx_imm32_ = 0xb9,
            mov_edx_imm32_ = 0xba,
            mov_ebx_imm32_ = 0xbb,
            mov_esp_imm32_ = 0xbc,
            mov_ebp_imm32_ = 0xbd,
            mov_esi_imm32_ = 0xbe,
            mov_edi_imm32_ = 0xbf,
            call_rel32_ = 0xe8, // push eip, eip+=imm32
            jmp_rel32_ = 0xe9,  // eip+=imm32

        };
        using 指令立即双字 = ins_imm32_32bit;

        typedef struct instruction_2oprand_2BYTE
        {
            uint8_t ins : 5;
            uint8_t mod : 3;

            uint8_t type : 2;
            uint8_t op1 : 3;
            uint8_t op2 : 3;

            instruction_2oprand_2BYTE(ins_2op_5bit ins_, mod_2op_3bit mod_, type_2op_2bit type_, reg_order_3bit op1_, reg_order_3bit op2_)
            {
                ins = ins_;
                mod = mod_;
                type = type_;
                op1 = op1_;
                op2 = op2_;
            };

            machine_code<2> to_machine_code()
            {
                machine_code<2> ret;
                ret.code[0] = ins << 3 | mod;
                ret.code[1] = type << 6 | op1 << 3 | op2;
                return ret;
            }
            machine_code<2> mcoded()
            {
                return to_machine_code();
            }

        } ins2op2b;

        typedef struct instruction_reg0_imm8
        {
            uint8_t ins : 5;
            uint8_t mod : 3;
            uint8_t imm8 : 8;
            instruction_reg0_imm8(ins_2op_5bit ins_, mod_2op_3bit mod_, uint8_t imm8_)
            {
                ins = ins_;
                mod = mod_;
                imm8 = imm8_;
            };
            machine_code<2> to_machine_code()
            {
                machine_code<2> ret;
                ret.code[0] = ins << 3 | mod;
                ret.code[1] = imm8;
                return ret;
            }
            machine_code<2> mcoded()
            {
                return to_machine_code();
            }
        } insreg0imm8;

        typedef struct instruction_reg0_imm32
        {
            uint8_t ins : 5;
            uint8_t mod : 3;
            uint32_t imm32 : 32;
            instruction_reg0_imm32(ins_2op_5bit ins_, mod_2op_3bit mod_, uint32_t imm32_)
            {
                ins = ins_;
                mod = mod_;
                imm32 = imm32_;
            };
            machine_code<5> to_machine_code()
            {
                machine_code<5> ret;
                ret.code[0] = ins << 3 | mod;
                *(uint32_t*)(ret.code + 1) = imm32;
                return ret;
            }
            machine_code<5> mcoded()
            {
                return to_machine_code();
            }
        } insreg0imm32;

        machine_code<2> x86asm(ins_2op_5bit ins_, mod_2op_3bit mod_, type_2op_2bit type_, reg_order_3bit op1_, reg_order_3bit op2_)
        {
            return machine_code<2>({ uint8_t(ins_ << 3 | mod_), uint8_t(type_ << 6 | op1_ << 3 | op2_) });
        }

        machine_code<1> x86asm(ins_1byte_8bit ins_)
        {
            return machine_code<1>({ uint8_t(ins_) });
        }

        machine_code<2> x86asm(ins_imm8_8bit ins_, uint8_t imm8_)
        {
            return machine_code<2>({ uint8_t(ins_), uint8_t(imm8_) });
        }

        machine_code<5> x86asm(ins_imm32_32bit ins_, uint32_t imm32_)
        {
            return machine_code<5>({ uint8_t(ins_), uint8_t(imm32_ >> 24), uint8_t(imm32_ >> 16), uint8_t(imm32_ >> 8), uint8_t(imm32_) });
        }

        machine_code<5> x86asm(ins_2op_5bit ins_, mod_2op_3bit mod_, uint32_t imm32_)
        {
            return machine_code<5>({ uint8_t(ins_ << 3 | mod_), uint8_t(imm32_), uint8_t(imm32_ >> 8), uint8_t(imm32_ >> 16), uint8_t(imm32_ >> 24) });
        }
#define 使用叉八六 using namespace protocol::x86_assembler;
#define 叉八六汇编 x86asm

        enum asm_token
        {
            token_void = 0xffff,
            opcode_1oprand = 0,
            opcode_2oprand = 1,
            mod_BYTE = 2,
            mod_DWORD = 3,
            oprand_mem = 4,
            oprand_reg = 5,
            oprand_imm8 = 6,
            oprand_imm32 = 7,
        };

        struct token_
        { // 32 BYTE
            int32_t type;
            char name[32 - sizeof(type)];
            token_(int32_t type_, const char* name_)
            {
                type = type_;
                for (int i = 0; i < sizeof(name); i++)
                {
                    name[i] = name_[i];
                    if (name_[i] == 0)
                        break;
                }
            }
        };

        uint64_t b64_10c_(const char str[10])
        {
            uint64_t ret = 0;
            for (int i = 0; i < 10; i++)
            {
                ret <<= 6;
                if (str[i] >= '0' && str[i] <= '9')
                    ret |= str[i] - '0';
                else if (str[i] >= 'a' && str[i] <= 'z')
                    ret |= str[i] - 'a' + 10;
                else if (str[i] >= 'A' && str[i] <= 'Z')
                    ret |= str[i] - 'A' + 36;
                else if (str[i] == '_')
                    ret |= 62;
                else if (str[i] == ',')
                    ret |= 63;
                else if (str[i] == '\0')
                    break;
                else
                    return ~((uint64_t)0);
            }
            return ret;
        };
        uint64_t operator""_b64_10c_(const char* str, size_t len)
        {
            return b64_10c_(str);
        }

        void asm_lexer(const char* asmcode, int len)
        {
            char buf[10] = {};

            auto getlineend = [](const char* str, int len, int start = 0) -> int
            {
                for (int i = start; i < len; i++)
                {
                    if (str[i] == '\n')
                        return i;
                }
                return len;
            };

            auto getword = [](const char* str, int len, char wordbuf[10], int start = 0) -> int
            {
                int i = start;
                for (; i < len; i++)
                {
                    if (str[i] == ' ' || str[i] == '\t')
                        continue;
                    else
                        break;
                }
                int j = 0;
                for (; i < len; i++)
                {
                    if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == ',' || str[i] == 0 || j >= 16)
                        break;
                    else
                        wordbuf[j] = str[i];
                    j++;
                }
                wordbuf[j] = 0;
                return i;
            };

            auto checktoken = [](char wordbuf[10]) -> int
            {
                switch (b64_10c_(wordbuf))
                {
                    // case "add"_b64_10c_:

                default:
                    break;
                }
            };
        };
    };

    namespace jit_run
    {
        class executable_memory
        {
        protected:
            void* ptr = NULL;
            size_t size = 0;
#ifdef _WIN32
            const size_t min_size = 4096;
#elif defined(__linux__)
            const size_t min_size = 4096;
#endif
        public:
            executable_memory() = default;
            executable_memory(void* code, size_t size_)
            {
                reset(code, size_);
            }
            void* get_ptr()
            {
                return ptr;
            }
            size_t get_size()
            {
                return size;
            }
            void free()
            {
                executable_free(ptr, size);
                ptr = NULL;
                size = 0;
            }
            void alloc(size_t size_)
            {
                free();
                // size_ should be multiple of min_size
                const size_t mask = min_size - 1;
                size_ = (size_ + mask) & (~mask);
                size = size_;
                ptr = executable_malloc(size_);
            }
            void reset(const void* code, size_t size_)
            {
                free();
                alloc(size_);
                memcpy(ptr, code, size_);
            }

            template <typename obj_t>
            void reset(obj_t& obj)
            {
                reset(obj.get_code(), obj.get_size());
            }

            ~executable_memory()
            {
                free();
            }
            void* executable_malloc(size_t size)
            {
#if defined(_WIN32)
                return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#elif defined(__linux__)
                return mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#else
#error "not supported"
                return NULL;
#endif
            }
            void executable_free(void* ptr, size_t size)
            {
#if defined(_WIN32)
                VirtualFree(ptr, 0, MEM_RELEASE);
#elif defined(__linux__)
                munmap(ptr, size);
#else
#error "not supported"
#endif
            }

            // set the memory to be read-executable only
            void lock()
            {
#if defined(_WIN32)
                DWORD oldProtect;
                VirtualProtect(ptr, size, PAGE_EXECUTE_READ, &oldProtect);
#elif defined(__linux__)
                mprotect(ptr, size, PROT_READ | PROT_EXEC);
#else
#error "not supported"
#endif
            };

            void unlock()
            {
#if defined(_WIN32)
                DWORD oldProtect;
                VirtualProtect(ptr, size, PAGE_EXECUTE_READWRITE, &oldProtect);
#elif defined(__linux__)
                mprotect(ptr, size, PROT_READ | PROT_WRITE | PROT_EXEC);
#else
#error "not supported"
#endif
            }
        };

        class jit_interface
        {
        public:
            enum func_call_enum
            {
                fc_cdecl = 0,
                fc_stdcall = 1,
                fc_fastcall = 2,
                fc_thiscall = 3,
                fc_axcall = 4,
                fc_cxcall = 5,
                fc_staticall = 6,
            };
            func_call_enum func_call = fc_cxcall;
            executable_memory coder;
            executable_memory caller;
            constexpr static const uint8_t axcall[] = {
#if (defined(_WIN32) && defined(_M_IX86)) || (defined(__linux__) && defined(__i386__))
                // the args are on stack, and the return value is in eax
                // the first 4 args are in eax,ebx,ecx,edx
                // the first arg is for function call
                // mov eax,[esp+4]
                0x8b,
                0x44,
                0x24,
                0x04,
                // mov ebx,[esp+8]
                0x8b,
                0x5c,
                0x24,
                0x08,
                // mov ecx,[esp+12]
                0x8b,
                0x4c,
                0x24,
                0x0c,
                // mov edx,[esp+16]
                0x8b,
                0x54,
                0x24,
                0x10,
                // jmp eax
                0xff,
                0xe0,
#elif (defined(_WIN32) && defined(_M_X64)) || (defined(__linux__) && defined(__x86_64__))
                // in x64, the first 4 args are in rcx,rdx,r8,r9
                // mov them to rax,rbx,rcx,rdx
                // mov rax,r8
                0x49,
                0x8b,
                0xc0,
                // xchg rax,rcx
                0x48,
                0x91,
                // mov rbx,rdx
                0x48,
                0x8b,
                0xda,
                // mov rdx,r9
                0x49,
                0x8b,
                0xd1,
                // jmp rax
                0xff,
                0xe0,
#endif
            };
            constexpr static const uint8_t cxcall[] = {
#if (defined(_WIN32) && defined(_M_IX86)) || (defined(__linux__) && defined(__i386__))
                // the args are on stack, mov them to ecx,edx,eax,ebx
                // mov ecx,[esp+4]
                0x8b,
                0x4c,
                0x24,
                0x04,
                // mov edx,[esp+8]
                0x8b,
                0x54,
                0x24,
                0x08,
                // mov eax,[esp+12]
                0x8b,
                0x44,
                0x24,
                0x0c,
                // mov ebx,[esp+16]
                0x8b,
                0x5c,
                0x24,
                0x10,
                // jmp ecx
                0xff,
                0xe1,
#elif (defined(_WIN32) && defined(_M_X64)) || (defined(__linux__) && defined(__x86_64__))
                // in x64, the first 4 args are in rcx,rdx,r8,r9
                // mov them to rcx,rdx,rax,rbx
                // mov rcx,rcx//do nothing
                // mov rdx,rdx//do nothing
                // mov rax,r8
                0x49,
                0x8b,
                0xc0,
                // mov rbx,r9
                0x49,
                0x8b,
                0xd9,
                // jmp rcx
                0xff,
                0xe1,
#endif
            };
            constexpr static const uint8_t staticall[] = {
#if (defined(_WIN32) && defined(_M_IX86)) || (defined(__linux__) && defined(__i386__))
                // the args are on stack, and the return value is in eax
                // the first 4 args are in eax,ebx,ecx,edx
                // the first arg is for function call
                // mov eax,[esp+4]
                0x8b,
                0x44,
                0x24,
                0x04,
                // mov ebx,[esp+8]
                0x8b,
                0x5c,
                0x24,
                0x08,
                // mov ecx,[esp+12]
                0x8b,
                0x4c,
                0x24,
                0x0c,
                // mov edx,[esp+16]
                0x8b,
                0x54,
                0x24,
                0x10,
                // jmp offset 0
                0xe9,
                0x00,
                0x00,
                0x00,
                0x00
#elif (defined(_WIN32) && defined(_M_X64)) || (defined(__linux__) && defined(__x86_64__))
                // in x64, the first 4 args are in rcx,rdx,r8,r9
                // mov them to rax,rbx,rcx,rdx
                // mov rax,r8
                0x49,
                0x8b,
                0xc0,
                // xchg rax,rcx
                0x48,
                0x91,
                // mov rbx,rdx
                0x48,
                0x8b,
                0xda,
                // mov rdx,r9
                0x49,
                0x8b,
                0xd1,
                // jmp offset 0 (64bit)
                0xff,
                0x25,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,
                0x00,
                0x00,
                0x00,
                0x00
#endif
            };

            class jit_code
            {
            public:
                size_t size = 0;
                uint8_t* code = NULL;
                // a lambda function, accept this, argc, argv
                // and return void
                typedef void (*modify_func_t)(jit_code*, size_t, void*);
                modify_func_t modify_func = NULL;

                jit_code() {};
                jit_code(void* code_, size_t size_, modify_func_t modify_func_ = NULL)
                {
                    init(code_, size_, modify_func_);
                }

                void init(void* code_, size_t size_, modify_func_t modify_func_ = NULL)
                {
                    code = (uint8_t*)code_;
                    size = size_;
                    modify_func = modify_func_;
                }
                const void* get_code()
                {
                    return code;
                }
                size_t get_size()
                {
                    return size;
                }
                void modify(size_t argc = 0, void* argv = 0)
                {
                    if (modify_func != NULL)
                        modify_func(this, argc, argv);
                }
                ~jit_code()
                {
                }
            };

            jit_code ccall[7] = {
                jit_code(),                               // fc_cdecl
                jit_code(),                               // fc_stdcall
                jit_code(),                               // fc_fastcall
                jit_code(),                               // fc_thiscall
                jit_code((void*)axcall, sizeof(axcall)), // fc_axcall
                jit_code((void*)cxcall, sizeof(cxcall)), // fc_cxcall
                jit_code((void*)staticall, sizeof(staticall),
                         [](jit_code* obj, size_t argc, void* argv)
                         {
                        // argc is the function address, argv is the addr to change
            #if (defined(_WIN32) && defined(_M_IX86)) || (defined(__linux__) && defined(__i386__))
                                         // the last 4 BYTEs of staticall is the relative address of the function
                                         static_assert(sizeof(argc) == sizeof(uint32_t), "argc should be 32bit");
                                         uint32_t* relative_immaddr = (uint32_t*)(uint32_t(argv) + obj->get_size() - 4);
                                         uint32_t relative_value = (uint32_t)argc - ((uint32_t)relative_immaddr + 4);
                                         *relative_immaddr = relative_value;
            #elif (defined(_WIN32) && defined(_M_X64)) || (defined(__linux__) && defined(__x86_64__))
                                         // the last 8 BYTEs of staticall is the absolute address of the function
                                         uint64_t* relative_immaddr = (uint64_t*)(uint64_t(argv) + obj->get_size() - 8);
                                         uint64_t relative_value = argc;
                                         *relative_immaddr = relative_value;
            #endif
                                     }) // fc_staticall

            };

            // const size_t ccall[][2] = {
            //     {0, 0},                           // fc_cdecl
            //     {0, 0},                           // fc_stdcall
            //     {0, 0},                           // fc_fastcall
            //     {0, 0},                           // fc_thiscall
            //     {(size_t)axcall, sizeof(axcall)}, // fc_axcall
            //     {(size_t)cxcall, sizeof(cxcall)}, // fc_cxcall
            // };

            jit_interface(const void* code_, size_t size_, func_call_enum func_call_ = fc_cxcall)
            {
                coder.reset(code_, size_);
                func_call = func_call_;
                caller.reset(ccall[func_call_]);
                ccall[func_call_].modify((size_t)coder.get_ptr(), caller.get_ptr());
            };

            size_t run(size_t arg1 = 0, size_t arg2 = 0, size_t arg3 = 0, size_t arg4 = 0)
            {

#if defined(_WIN32) // use cdecl
                typedef size_t(__cdecl* func_t)(size_t, size_t, size_t, size_t);
#elif defined(__linux__) // use cdecl
                typedef size_t(*func_t)(size_t, size_t, size_t, size_t);
#endif

                func_t caller_ = (func_t)caller.get_ptr();
                switch (func_call)
                {
                case fc_staticall:
                    return caller_(arg1, arg2, arg3, arg4);
                case fc_axcall:
                case fc_cxcall:
                    return caller_((size_t)coder.get_ptr(), arg1, arg2, arg3);
                default:
                    return -1;
                }
            };
        };
    };

};

#endif