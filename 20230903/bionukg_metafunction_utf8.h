#pragma once
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

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
// �� Pointer to data in the DS segment
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
// �� Counter for string and loop operations
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
// �� I/O pointer
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
			case 0x2: //?
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
			switch (mem[0] & 0x0f)
			{
			case 0x0: // or 8,imm8
				break;
			case 0x1: // or 32,imm32
				break;
			case 0x2: // or 8,imm8
				break;
			case 0x3: // or 32,imm32
				break;
			case 0x4: // test 8,8
				break;
			case 0x5: // test 32,32
				break;
			case 0x6: // xchg 8,8
				break;
			case 0x7: // xchg 32,32
				break;
			case 0x8: // mov 8,8
				break;
			case 0x9: // mov 32,32
				break;
			case 0xa: // mov 8,8(swap)
				break;
			case 0xb: // mov 32,32(swap)
				break;
			case 0xc: // mov 32,cs
				break;
			case 0xd: // ?
				break;
			case 0xe: // mov cs,32
				break;
			case 0xf: // ?
				break;
			}
			break;
		case 0x9:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // nop
				break;
			case 0x1: // xchg eax,ecx
				break;
			case 0x2: // xchg eax,edx
				break;
			case 0x3: // xchg eax,ebx
				break;
			case 0x4: // xchg eax,esp
				break;
			case 0x5: // xchg eax,ebp
				break;
			case 0x6: // xchg eax,esi
				break;
			case 0x7: // xchg eax,edi
				break;
			case 0x8: // cwde // mov eax,sign-extend of ax
				break;
			case 0x9: // cdq // mov {edx,eax},sign-extend of eax
				break;
			case 0xa: // call far imm32:imm16
				break;
			case 0xb: // wait
				break;
			case 0xc: // pushfd // push eflags
				break;
			case 0xd: // popfd // pop eflags
				break;
			case 0xe: // sahf // store ah into flags
				break;
			case 0xf: // lahf // load ah from flags
				break;
			}
			break;
		case 0xa:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // mov al, [imm32]
				break;
			case 0x1: // mov eax, [imm32]
				break;
			case 0x2: // mov [imm32], al
				break;
			case 0x3: // mov [imm32], eax
				break;
			case 0x4: // mov BYTE [edi], [esi]
				break;
			case 0x5: // mov DWORD [edi], [esi]
				break;
			case 0x6: // cmp BYTE [esi], [edi]
				break;
			case 0x7: // cmp DWORD [esi], [edi]
				break;
			case 0x8: // test al, imm8
				break;
			case 0x9: // test eax, imm32
				break;
			case 0xa: // stos BYTE [edi], al
				break;
			case 0xb: // stos DWORD [edi], eax
				break;
			case 0xc: // lods BYTE al, [esi]
				break;
			case 0xd: // lods DWORD eax, [esi]
				break;
			case 0xe: // scas BYTE [edi], al
				break;
			case 0xf: // scas DWORD [edi], eax
				break;
			}
			break;
		case 0xb:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // mov al, imm8
				break;
			case 0x1: // mov cl, imm8
				break;
			case 0x2: // mov dl, imm8
				break;
			case 0x3: // mov bl, imm8
				break;
			case 0x4: // mov ah, imm8
				break;
			case 0x5: // mov ch, imm8
				break;
			case 0x6: // mov dh, imm8
				break;
			case 0x7: // mov bh, imm8
				break;
			case 0x8: // mov eax, imm32
				break;
			case 0x9: // mov ecx, imm32
				break;
			case 0xa: // mov edx, imm32
				break;
			case 0xb: // mov ebx, imm32
				break;
			case 0xc: // mov esp, imm32
				break;
			case 0xd: // mov ebp, imm32
				break;
			case 0xe: // mov esi, imm32
				break;
			case 0xf: // mov edi, imm32
				break;
			}
			break;
		case 0xc:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // ror 8,imm8
				break;
			case 0x1: // ror 32,imm8
				break;
			case 0x2: // ret imm16
				break;
			case 0x3: // ret
				break;
			case 0x4: // ?
				break;
			case 0x5: // ?
				break;
			case 0x6: // ?
				break;
			case 0x7: // ?
				break;
			case 0x8: // enter imm16,imm8
				break;
			case 0x9: // leave
				break;
			case 0xa: // retf imm16
				break;
			case 0xb: // retf
				break;
			case 0xc: // int3
				break;
			case 0xd: // int imm8
				break;
			case 0xe: // into
				break;
			case 0xf: // iretd
				break;
			}
			break;
		case 0xd:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // ror/rcr/shr/sar 8,imm=1
				break;
			case 0x1: // ror/rcr/shr/sar 32,imm=1
				break;
			case 0x2: // ror/rcr/shr/sar 8,cl
				break;
			case 0x3: // ror/rcr/shr/sar 32,cl
				break;
			case 0x4: // aamb imm8 //{ah,al}={al/imm8,al%imm8}
				break;
			case 0x5: // aadb imm8 //{ah,al}={0,ah*imm8+al}
				break;
			case 0x6: // ?
				break;
			case 0x7: // xlat BYTE [ebx] // al=[ebx+al]
				break;
			case 0x8: // fadd/fmul/fcom......
				break;
			case 0x9: // fstp/fldcw/fnstcw......
				break;
			case 0xa: // fimul/ficomp/fisub......
				break;
			case 0xb: // fild/fist/fcmovnb......
				break;
			case 0xc: // fadd/fcom/fmul......
				break;
			case 0xd: // fld/fst/frstor......
				break;
			case 0xe: // fiadd/ficom/fisub......
				break;
			case 0xf: // fild/fist/fbld......
				break;
			}
			break;
		case 0xe:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // loopne rel8 // ecx--; if(ecx!=0 && !ZF) eip+=rel8
				break;
			case 0x1: // loope rel8 // ecx--; if(ecx!=0 && ZF) eip+=rel8
				break;
			case 0x2: // loop rel8 // ecx--; if(ecx!=0) eip+=rel8
				break;
			case 0x3: // jecxz rel8 // if(ecx==0) eip+=rel8
				break;
			case 0x4: // in al, imm8
				break;
			case 0x5: // in eax, imm8
				break;
			case 0x6: // out imm8, al
				break;
			case 0x7: // out imm8, eax
				break;
			case 0x8: // call rel32 // push eip; eip+=rel32
				break;
			case 0x9: // jmp rel32 // eip+=rel32
				break;
			case 0xa: // jmp far imm32:imm16 // cs=imm16; eip=imm32
				break;
			case 0xb: // jmp rel8 // eip+=rel8
				break;
			case 0xc: // in al, dx
				break;
			case 0xd: // in eax, dx
				break;
			case 0xe: // out dx, al
				break;
			case 0xf: // out dx, eax
				break;
			}
			break;
		case 0xf:
			switch (mem[0] & 0x0f)
			{
			case 0x0: // lock // prefix, lock the bus
				break;
			case 0x1: // ?
				break;
			case 0x2: // repne // prefix, repeat if not equal/zero // while(ecx!=0 && !ZF) {ecx--; <next instruction>}
				break;
			case 0x3:// rep // prefix, repeat if equal/zero // while(ecx!=0 && ZF) {ecx--; <next instruction>}
				break;
			case 0x4:// hlt // halt the cpu
				break;
			case 0x5:// cmc // complement carry flag // CF=!CF
				break;
			case 0x6:// ?
				break;
			case 0x7:// ?
				break;
			case 0x8:// clc // clear carry flag // CF=0
				break;
			case 0x9:// stc // set carry flag // CF=1
				break;
			case 0xa:// cli // clear interrupt flag // IF=0
				break;
			case 0xb:// sti // set interrupt flag // IF=1
				break;
			case 0xc:// cld // clear direction flag // DF=0
				break;
			case 0xd:// std // set direction flag // DF=1
				break;
			case 0xe:// dec/inc BYTE
				break;
			case 0xf:// dec/inc DWORD
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
			((uint32_t *)funcmem)[2 * i] = 0xcccccccc;
			((uint32_t *)funcmem)[2 * i + 1] = 0xcccccccc;

			funcmem[8 * i] = i;
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

// make any string to a int64_t (should be less than 8 chars)
constexpr int64_t operator"" _t_i64(const char *str, size_t size)
{
	int64_t ret = 0;
	if (size > 8)
	{
		size = 8;
	}
	for (size_t i = 0; i < size; i++)
	{
		ret |= (int64_t)str[i] << (i * 8);
	}
	return ret;
}
// turn txt raw data to int64_t
int64_t txt_raw_to_i64(const char *str)
{
	int64_t ret = 0;
	for (size_t i = 0; i < 8 && str[i] != 0; i++)
	{
		ret |= (int64_t)str[i] << (i * 8);
	}
	return ret;
}
// return -1 if failed (val is 0), if success return type
int txt_value_to_int(_In_ const char *str, _Out_ size_t *val)
{
	// in worst case, binary, the length of str is sizeof(size_t)*8+3
	const size_t max_len = sizeof(size_t) * 8 + 3;
	// get the actual length of str, if too long, return -1
	char pure_num[max_len + 1] = {0};
	int pure_start = 0; // the start of pure number, not 0x or 0b
	size_t pure_len = 0;
	size_t len = 0;
	while (str[len] != 0)
	{
		len++;
		if (len > max_len)
		{
			return -1;
		}
	}
	// if str is empty, return -1
	if (len == 0)
	{
		return -1;
	}
	else if (len == 1)
	{
		if (str[0] >= '0' && str[0] <= '9')
		{
			*val = str[0] - '0';
			return 0;
		}
		else
		{
			return -1;
		}
	}

	int type = 0;
	// 0:dec: pure number, only 0-9, starts with non-zero
	// 1:hex: starts with 0x, 0X, 0h, 0H, or ends with h, H, or starts with 0 and followed by non-zero and ends with h, H
	// 2:oct: starts with 0o, 0O, 0q, 0Q, or starts with 0 and followed by non-zero and ends with 0~7,
	// 3:bin: starts with 0b, 0B,
	// 4:dec: starts with 0d, 0D,
	// -1:unknown: any other type, return -1 immediately
	char start[2] = {str[0], str[1]};
	char end = str[len - 1];
	// verify the type form the start
	if (start[0] == '0')
	{
		pure_start = 2;
		switch (start[1])
		{
		case 'x':
		case 'X':
			type = 1;
			break;
		case 'o':
		case 'O':
		case 'q':
		case 'Q':
			type = 2;
			break;
		case 'b':
		case 'B':
			type = 3;
			break;
		case 'd':
		case 'D':
			type = 4;
			break;
		case '0':
			return -1;
			break;
		default:
			if (end == 'h' || end == 'H')
			{
				type = 1;
				pure_start -= 1;
			}
			else if (end >= '0' && end <= '7')
			{
				type = 2;
				pure_start -= 1;
			}
			else
			{
				return -1;
			}
			break;
		}
	}
	else if (start[0] >= '1' && start[0] <= '9')
	{
		type = 0;
		pure_start = 0;
	}
	else
	{
		return -1;
	}
	// copy the pure number to pure_num, not including '_',and the 'h' in the end of hex
	for (size_t i = pure_start; i < len; i++)
	{
		if (str[i] == '_')
		{
			continue;
		}
		else if ((str[i] == 'h' || str[i] == 'H') && i == len - 1) // in th end
		{
			if (type == 1)
			{
				break;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			pure_num[pure_len] = str[i];
			pure_len++;
		}
	}
	// in diffent cases, convert the pure number to int
	*val = 0;
	switch (type)
	{
	case 0: // dec
	case 4:
		for (size_t i = 0; i < pure_len; i++)
		{
			if (pure_num[i] >= '0' && pure_num[i] <= '9')
			{
				*val *= 10;
				*val += pure_num[i] - '0';
			}
			else
			{
				return -1;
			}
		}
		break;
	case 1:
		for (size_t i = 0; i < pure_len; i++)
		{
			if (pure_num[i] >= '0' && pure_num[i] <= '9')
			{
				*val *= 16;
				*val += pure_num[i] - '0';
			}
			else if (pure_num[i] >= 'a' && pure_num[i] <= 'f')
			{
				*val *= 16;
				*val += pure_num[i] - 'a' + 10;
			}
			else if (pure_num[i] >= 'A' && pure_num[i] <= 'F')
			{
				*val *= 16;
				*val += pure_num[i] - 'A' + 10;
			}
			else
			{
				return -1;
			}
		}
		break;
	case 2:
		for (size_t i = 0; i < pure_len; i++)
		{
			if (pure_num[i] >= '0' && pure_num[i] <= '7')
			{
				*val *= 8;
				*val += pure_num[i] - '0';
			}
			else
			{
				return -1;
			}
		}
		break;
	case 3:
		for (size_t i = 0; i < pure_len; i++)
		{
			if (pure_num[i] == '0' || pure_num[i] == '1')
			{
				*val *= 2;
				*val += pure_num[i] - '0';
			}
			else
			{
				return -1;
			}
		}
		break;
	default:
		return -1;
		break;
	}
}
// operand of subset of x86
class asm_oprand_subx86
{
public:
	enum oprand_type_t
	{
		none,
		reg,
		mem,
		imm
	};
	oprand_type_t operand_type;
	size_t operand_size_bit; // 0 means unknown size
	// a union,
	// if its a reg, then reg is the reg number, eax = 0,ecx = 1,edx = 2,ebx = 3,esp = 4,ebp = 5,esi = 6,edi = 7
	// if its a mem, then mem is the mem address,[eax]=0,[ecx]=1,[edx]=2,[ebx]=3,[esp]=4,[ebp]=5,[esi]=6,[edi]=7
	// if its a imm, then imm is the imm value.
	union
	{
		size_t reg;
		size_t mem;
		size_t imm;
	} value;

	asm_oprand_subx86()
	{
		operand_type = none;
		operand_size_bit = 0;
	}
	// str is the operand text
	asm_oprand_subx86(const char *str)
	{
		operand_type = none;
		operand_size_bit = 0;
		char t0[8] = {0};
		size_t i = 0;
		// t0 is the first chars before the first space
		for (; i < 8; i++)
		{
			if (str[i] == ' ' || str[i] == 0)
			{
				t0[i] = 0;
				break;
			}
			t0[i] = str[i];
		}

		int64_t t0i = txt_raw_to_i64(t0);
		switch (t0i)
		{
		case "eax"_t_i64:
			value.reg = 0;
			goto reg32;
		case "ecx"_t_i64:
			value.reg = 1;
			goto reg32;
		case "edx"_t_i64:
			value.reg = 2;
			goto reg32;
		case "ebx"_t_i64:
			value.reg = 3;
			goto reg32;
		case "esp"_t_i64:
			value.reg = 4;
			goto reg32;
		case "ebp"_t_i64:
			value.reg = 5;
			goto reg32;
		case "esi"_t_i64:
			value.reg = 6;
			goto reg32;
		case "edi"_t_i64:
			value.reg = 7;
		reg32:
			operand_type = reg;
			operand_size_bit = 32;
			return;
			break;
		case "al"_t_i64:
			value.reg = 0;
			goto reg8;
		case "cl"_t_i64:
			value.reg = 1;
			goto reg8;
		case "dl"_t_i64:
			value.reg = 2;
			goto reg8;
		case "bl"_t_i64:
			value.reg = 3;
			goto reg8;
		case "ah"_t_i64:
			value.reg = 4;
			goto reg8;
		case "ch"_t_i64:
			value.reg = 5;
			goto reg8;
		case "dh"_t_i64:
			value.reg = 6;
			goto reg8;
		case "bh"_t_i64:
			value.reg = 7;
		reg8:
			operand_type = reg;
			operand_size_bit = 8;
			return;
			break;
		case "byte"_t_i64:
			operand_size_bit = 8;
			goto mem;
		case "word"_t_i64:
			operand_size_bit = 16;
			goto mem;
		case "dword"_t_i64:
			operand_size_bit = 32;
			goto mem;
		case "qword"_t_i64:
			operand_size_bit = 64;
			goto mem;
		case "[eax]"_t_i64:
			value.mem = 0;
			goto mem;
		case "[ecx]"_t_i64:
			value.mem = 1;
			goto mem;
		case "[edx]"_t_i64:
			value.mem = 2;
			goto mem;
		case "[ebx]"_t_i64:
			value.mem = 3;
			goto mem;
		case "[esp]"_t_i64:
			value.mem = 4;
			goto mem;
		case "[ebp]"_t_i64:
			value.mem = 5;
			goto mem;
		case "[esi]"_t_i64:
			value.mem = 6;
			goto mem;
		case "[edi]"_t_i64:
			value.mem = 7;
		mem:
			operand_type = mem;
			break;
		default:
			operand_type = imm;
			operand_size_bit = 32;
			txt_value_to_int(str, &value.imm);
			return;
			break;
		}
		// analyze the mem operand
		// if oprand_size_bit is 0, there is only 1 piece,eg: [eax], just return
		if (operand_size_bit == 0)
			return;
		// if the 1st piece is "byte" or "word" or "dword" or "qword", then the 2nd piece is "ptr"
		// check the 2nd piece
		char t1[8] = {0};
		for (i++; i < 8; i++)
		{
			if (str[i] == ' ' || str[i] == 0)
			{
				t1[i] = 0;
				break;
			}
			t1[i] = str[i];
		}
		if (txt_raw_to_i64(t1) != "ptr"_t_i64)
		{
			// error
			operand_type = none;
			operand_size_bit = 0;
			return;
		}
		// check the 3rd piece
		char t2[8] = {0};
		for (int j = 0; j < 8; j++)
		{
			if (str[i] == ' ' || str[i] == 0)
			{
				t2[j] = 0;
				break;
			}
			t2[j] = str[i];
			i++;
		}
		switch (txt_raw_to_i64(t2))
		{
		case "[eax]"_t_i64:
			value.mem = 0;
			break;
		case "[ecx]"_t_i64:
			value.mem = 1;
			break;
		case "[edx]"_t_i64:
			value.mem = 2;
			break;
		case "[ebx]"_t_i64:
			value.mem = 3;
			break;
		case "[esp]"_t_i64:
			value.mem = 4;
			break;
		case "[ebp]"_t_i64:
			value.mem = 5;
			break;
		case "[esi]"_t_i64:
			value.mem = 6;
			break;
		case "[edi]"_t_i64:
			value.mem = 7;
			break;
		default:
			// error
			operand_type = none;
			operand_size_bit = 0;
			return;
			break;
		}
		// check the 4th piece, if it is not 0, then error
		char t3[8] = {0};
		for (int j = 0; j < 8; j++)
		{
			if (str[i] == ' ' || str[i] == 0)
			{
				t3[j] = 0;
				break;
			}
			t3[j] = str[i];
			i++;
		}
		if (txt_raw_to_i64(t3) != 0)
		{
			// error
			operand_type = none;
			operand_size_bit = 0;
			return;
		}
		// done
		return;
	}
};

// assembler of subset of x86
class asm_subx86
{
public:
	// enum of x86 opcode text, using _t_i64
	enum x86_opc_t : int64_t
	{
		// 2 operands
		add_opc_t = "add"_t_i64,
		adc_opc_t = "adc"_t_i64,
		sub_opc_t = "sub"_t_i64,
		sbb_opc_t = "sbb"_t_i64,
		and_opc_t = "and"_t_i64,
		or_opc_t = "or"_t_i64,
		xor_opc_t = "xor"_t_i64,
		cmp_opc_t = "cmp"_t_i64,
		test_opc_t = "test"_t_i64,
		xchg_opc_t = "xchg"_t_i64,
		mov_opc_t = "mov"_t_i64,
		arpl_opc_t = "arpl"_t_i64,
		// 1 operand
		push_opc_t = "push"_t_i64,
		pop_opc_t = "pop"_t_i64,
		bswap_opc_t = "bswap"_t_i64,
		inc_opc_t = "inc"_t_i64,
		dec_opc_t = "dec"_t_i64,
		neg_opc_t = "neg"_t_i64,
		// 0 operand
		daa_opc_t = "daa"_t_i64,
		das_opc_t = "das"_t_i64,
		aaa_opc_t = "aaa"_t_i64,
		pushad_opc_t = "pushad"_t_i64,
		popad_opc_t = "popad"_t_i64,
		cwde_opc_t = "cwde"_t_i64,
		cdq_opc_t = "cdq"_t_i64,
		wait_opc_t = "wait"_t_i64,
		pushfd_opc_t = "pushfd"_t_i64,
		popfd_opc_t = "popfd"_t_i64,
		sahf_opc_t = "sahf"_t_i64,
		lahf_opc_t = "lahf"_t_i64,
		// jump
		jo_opc_t = "jo"_t_i64,
		jno_opc_t = "jno"_t_i64,
		jb_opc_t = "jb"_t_i64,
		jnb_opc_t = "jnb"_t_i64,
		jae_opc_t = "jae"_t_i64,
		jbe_opc_t = "jbe"_t_i64,
		je_opc_t = "je"_t_i64,
		jne_opc_t = "jne"_t_i64,
		jna_opc_t = "jna"_t_i64,
		jg_opc_t = "jg"_t_i64,
		jge_opc_t = "jge"_t_i64,
		jl_opc_t = "jl"_t_i64,
		jle_opc_t = "jle"_t_i64,
		jmp_opc_t = "jmp"_t_i64,
		ja_opc_t = "ja"_t_i64,
		// call
		call_opc_t = "call"_t_i64,
		// ret
		ret_opc_t = "ret"_t_i64,

		// repeat
		rep_opc_t = "rep"_t_i64,
		repe_opc_t = "repe"_t_i64,
		repne_opc_t = "repne"_t_i64,
		// flag
		clc_opc_t = "clc"_t_i64,
		stc_opc_t = "stc"_t_i64,
		cli_opc_t = "cli"_t_i64,
		sti_opc_t = "sti"_t_i64,
		cld_opc_t = "cld"_t_i64,
		std_opc_t = "std"_t_i64,
		// misc
		nop_opc_t = "nop"_t_i64,
		hlt_opc_t = "hlt"_t_i64,

	};
	// enum of x86 machine operation code
	enum x86_machine_operation : uint8_t
	{
		add_reg_to_mem_u8 = 0x00,
		add_reg_to_mem_u32 = 0x01,
		add_mem_to_reg_u8 = 0x02,
		add_mem_to_reg_u32 = 0x03,
		add_imm_to_reg_u8 = 0x04,
		add_imm_to_reg_u32 = 0x05,
		push_es = 0x06,
		pop_es = 0x07,
		or_reg_to_mem_u8 = 0x08,
		or_reg_to_mem_u32 = 0x09,
		or_mem_to_reg_u8 = 0x0A,
		or_mem_to_reg_u32 = 0x0B,
		or_imm_to_reg_u8 = 0x0C,
		or_imm_to_reg_u32 = 0x0D,
		push_cs = 0x0E,
		// 0x0F is 2 bytes opcode

		adc_reg_to_mem_u8 = 0x10,
		adc_reg_to_mem_u32 = 0x11,
		adc_mem_to_reg_u8 = 0x12,
		adc_mem_to_reg_u32 = 0x13,
		adc_imm_to_reg_u8 = 0x14,
		adc_imm_to_reg_u32 = 0x15,
		push_ss = 0x16,
		pop_ss = 0x17,
		sbb_reg_to_mem_u8 = 0x18,
		sbb_reg_to_mem_u32 = 0x19,
		sbb_mem_to_reg_u8 = 0x1A,
		sbb_mem_to_reg_u32 = 0x1B,
		sbb_imm_to_reg_u8 = 0x1C,
		sbb_imm_to_reg_u32 = 0x1D,
		push_ds = 0x1E,
		pop_ds = 0x1F,
		and_reg_to_mem_u8 = 0x20,
		and_reg_to_mem_u32 = 0x21,
		and_mem_to_reg_u8 = 0x22,
		and_mem_to_reg_u32 = 0x23,
		and_imm_to_reg_u8 = 0x24,
		and_imm_to_reg_u32 = 0x25,
		// 0x26 is segment override
		daa = 0x27, // daa: decimal adjust after addition
		sub_reg_to_mem_u8 = 0x28,
		sub_reg_to_mem_u32 = 0x29,
		sub_mem_to_reg_u8 = 0x2A,
		sub_mem_to_reg_u32 = 0x2B,
		sub_imm_to_reg_u8 = 0x2C,
		sub_imm_to_reg_u32 = 0x2D,
		// 0x2E is segment override
		das = 0x2F, // das: decimal adjust after subtraction

		xor_reg_to_mem_u8 = 0x30,
		xor_reg_to_mem_u32 = 0x31,
		xor_mem_to_reg_u8 = 0x32,
		xor_mem_to_reg_u32 = 0x33,
		xor_imm_to_reg_u8 = 0x34,
		xor_imm_to_reg_u32 = 0x35,
		// 0x36 is segment override
		aaa = 0x37, // aaa: ASCII adjust after addition
		cmp_reg_to_mem_u8 = 0x38,
		cmp_reg_to_mem_u32 = 0x39,
		cmp_mem_to_reg_u8 = 0x3A,
		cmp_mem_to_reg_u32 = 0x3B,
		cmp_imm_to_reg_u8 = 0x3C,
		cmp_imm_to_reg_u32 = 0x3D,
		// 0x3E is segment override
		aas = 0x3F, // aas: ASCII adjust AL after subtraction

		inc_eax = 0x40,
		inc_ecx = 0x41,
		inc_edx = 0x42,
		inc_ebx = 0x43,
		inc_esp = 0x44,
		inc_ebp = 0x45,
		inc_esi = 0x46,
		inc_edi = 0x47,
		dec_eax = 0x48,
		dec_ecx = 0x49,
		dec_edx = 0x4A,
		dec_ebx = 0x4B,
		dec_esp = 0x4C,
		dec_ebp = 0x4D,
		dec_esi = 0x4E,
		dec_edi = 0x4F,

		push_eax = 0x50,
		push_ecx = 0x51,
		push_edx = 0x52,
		push_ebx = 0x53,
		push_esp = 0x54,
		push_ebp = 0x55,
		push_esi = 0x56,
		push_edi = 0x57,
		pop_eax = 0x58,
		pop_ecx = 0x59,
		pop_edx = 0x5A,
		pop_ebx = 0x5B,
		pop_esp = 0x5C,
		pop_ebp = 0x5D,
		pop_esi = 0x5E,
		pop_edi = 0x5F,

		pusha = 0x60,
		popa = 0x61,
		bound = 0x62,
		arpl = 0x63,
		// 0x64 is segment override
		// 0x65 is segment override
		// 0x66 is operand size
		// 0x67 is address size
		push_imm32 = 0x68,
		imul_imm32_mem_to_reg_u32 = 0x69,
		push_imm8 = 0x6A,
		imul_imm8_mem_to_reg_u32 = 0x6B,
		ins_byte = 0x6C,
		ins_dword = 0x6D,
		outs_byte = 0x6E,
		out_dword = 0x6F,

		jo_i8 = 0x70,  // jump if OF==1
		jno_i8 = 0x71, // jump if OF==0
		jb_i8 = 0x72,  // jump if CF==1
		jnae_i8 = 0x72,
		jc_i8 = 0x72,
		jnb_i8 = 0x73, // jump if CF==0
		jae_i8 = 0x73,
		jnc_i8 = 0x73,
		jz_i8 = 0x74,  // jump if ZF==1
		jne_i8 = 0x75, // jump if ZF==0
		jbe_i8 = 0x76, // jump if CF==1 or ZF==1
		jna_i8 = 0x76,
		jnbe_i8 = 0x77, // jump if CF==0 and ZF==0
		ja_i8 = 0x77,
		js_i8 = 0x78,  // jump if SF==1
		jns_i8 = 0x79, // jump if SF==0
		jp_i8 = 0x7A,  // jump if PF==1
		jpe_i8 = 0x7A,
		jnp_i8 = 0x7B, // jump if PF==0
		jpo_i8 = 0x7B,
		jl_i8 = 0x7C, // jump if SF!=OF
		jnge_i8 = 0x7C,
		jnl_i8 = 0x7D, // jump if SF==OF
		jge_i8 = 0x7D,
		jle_i8 = 0x7E, // jump if ZF==1 or SF!=OF
		jng_i8 = 0x7E,
		jnle_i8 = 0x7F, // jump if ZF==0 and SF==OF
		jg_i8 = 0x7F,

		// 0x80 is group 1
		// 0x81 is group 1
		// 0x82 is group 1
		// 0x83 is group 1
		test_reg_to_mem_u8 = 0x84,
		test_reg_to_mem_u32 = 0x85,
		xchg_reg_to_mem_u8 = 0x86,
		xchg_reg_to_mem_u32 = 0x87,
		mov_reg_to_mem_u8 = 0x88,
		mov_reg_to_mem_u32 = 0x89,
		mov_mem_to_reg_u8 = 0x8A,
		mov_mem_to_reg_u32 = 0x8B,
		mov_mem_to_sreg = 0x8C,	   // sreg: segment register
		lea_mem_to_reg_u32 = 0x8D, // lea: load effective address
		mov_sreg_to_mem = 0x8E,
		pop_mem = 0x8F,

		nop = 0x90,
		xchg_eax_ecx = 0x91,
		xchg_eax_edx = 0x92,
		xchg_eax_ebx = 0x93,
		xchg_eax_esp = 0x94,
		xchg_eax_ebp = 0x95,
		xchg_eax_esi = 0x96,
		xchg_eax_edi = 0x97,
		cbw = 0x98,		 // cbw: convert byte to word
		cwd = 0x99,		 // cwd: convert word to double word
		call_far = 0x9A, // with 32-bit offset and 16-bit segment
		wait = 0x9B,	 // wait: wait for floating point operation
		pushf = 0x9C,	 // pushf: push flags
		popf = 0x9D,	 // popf: pop flags
		sahf = 0x9E,	 // sahf: store AH into flags
		lahf = 0x9F,	 // lahf: load flags into AH

		mov_mem_imm8_to_al = 0xA0,
		mov_mem_imm32_to_eax = 0xA1,
		mov_al_to_mem_imm8 = 0xA2,
		mov_eax_to_mem_imm32 = 0xA3,
		mov_mem_esi_to_mem_edi_u8 = 0xA4,
		mov_mem_esi_to_mem_edi_u32 = 0xA5,
		mov_mem_edi_to_mem_esi_u8 = 0xA6,
		mov_mem_edi_to_mem_esi_u32 = 0xA7,
		test_al_imm8 = 0xA8,
		test_eax_imm32 = 0xA9,
		stos_mem_edi_u8 = 0xAA, // stos: store string
		stos_mem_edi_u32 = 0xAB,
		lods_mem_esi_u8 = 0xAC, // lods: load string
		lods_mem_esi_u32 = 0xAD,
		scas_mem_edi_u8 = 0xAE, // scas: scan string
		scas_mem_edi_u32 = 0xAF,
		mov_imm8_to_al = 0xB0,
		mov_imm8_to_cl = 0xB1,
		mov_imm8_to_dl = 0xB2,
		mov_imm8_to_bl = 0xB3,
		mov_imm8_to_ah = 0xB4,
		mov_imm8_to_ch = 0xB5,
		mov_imm8_to_dh = 0xB6,
		mov_imm8_to_bh = 0xB7,
		mov_imm32_to_eax = 0xB8,
		mov_imm32_to_ecx = 0xB9,
		mov_imm32_to_edx = 0xBA,
		mov_imm32_to_ebx = 0xBB,
		mov_imm32_to_esp = 0xBC,
		mov_imm32_to_ebp = 0xBD,
		mov_imm32_to_esi = 0xBE,
		mov_imm32_to_edi = 0xBF,
		ror_mem_u8 = 0xC0,
		ror_mem_u32 = 0xC1,
		ret_i16 = 0xC2, // with 16-bit immediate
		ret = 0xC3,
		// 0xC4 is group 2
		// 0xC5 is group 2
		// 0xC6 is group 11
		// 0xC7 is group 11
		enter_i16_i8 = 0xC8, // with 16-bit immediate and 8-bit immediate
		leave = 0xC9,
		retf_i16 = 0xCA,
		retf = 0xCB,
		int3 = 0xCC,
		int_i8 = 0xCD,
		into = 0xCE,
		iretd = 0xCF, // iretd: interrupt return

		ror_reg_u8 = 0xD0,
		ror_reg_u32 = 0xD1,
		ror_mem_cl_u8 = 0xD2,
		ror_mem_cl_u32 = 0xD3,
		aam_i8 = 0xD4, // aam: ASCII adjust after multiply with 8-bit immediate
		aad_i8 = 0xD5, // aad: ASCII adjust after divide with 8-bit immediate
		// 0xD6 is reserved
		xlat_mem_ebx_u8 = 0xD7, // xlat: translate table
		// 0xD8 is group 3
		// 0xD9 is group 3
		// 0xDA is group 3
		// 0xDB is group 3
		// 0xDC is group 3
		// 0xDD is group 3
		// 0xDE is group 3
		// 0xDF is group 3

		loopne_i8 = 0xE0, // loopne: decrement CX register and jump if not zero and not equal
		loope_i8 = 0xE1,  // loope: decrement CX register and jump if not zero and equal
		loop_i8 = 0xE2,	  // loop: decrement CX register and jump if not zero
		jecxz_i8 = 0xE3,  // jcxz: jump if CX register is zero
		jcxz_i8 = 0xE3,
		in_al_imm8 = 0xE4,
		in_eax_imm8 = 0xE5,
		out_imm8_al = 0xE6,
		out_imm8_eax = 0xE7,
		call_i32 = 0xE8, // eip = eip + 32-bit immediate
		jmp_i32 = 0xE9,	 // eip = eip + 32-bit immediate
		jmp_far = 0xEA,	 // eip = 32-bit immediate, cs = 16-bit immediate
		jmp_i8 = 0xEB,	 // eip = eip + 8-bit immediate
		in_al_dx = 0xEC,
		in_eax_dx = 0xED,
		out_dx_al = 0xEE,
		out_dx_eax = 0xEF,

		lock = 0xF0, // lock: lock bus
		// 0xF1 is reserved
		repne = 0xF2, // repne: repeat not equal
		repe = 0xF3,  // repe: repeat equal
		rep = 0xF3,
		hlt = 0xF4, // hlt: halt
		cmc = 0xF5, // cmc: complement carry flag
		// 0xF6 is group 4
		// 0xF7 is group 4
		clc = 0xF8, // clc: clear carry flag
		stc = 0xF9, // stc: set carry flag
		cli = 0xFA, // cli: clear interrupt flag
		sti = 0xFB, // sti: set interrupt flag
		cld = 0xFC, // cld: clear direction flag
		std = 0xFD, // std: set direction flag
					// 0xFE is group 5
					// 0xFF is group 5
	};
	// enum of x86 machine operand code for 8-bit register or memory
	enum x86_machine_operand_u8 : uint8_t
	{
		byte_ptr__eax__al = 0x00,
		byte_ptr__ecx__al = 0x01,
		byte_ptr__edx__al = 0x02,
		byte_ptr__ebx__al = 0x03,
		// 0x04
		// 0x05
		byte_ptr__esi__al = 0x06,
		byte_ptr__edi__al = 0x07,

		byte_ptr__eax__cl = 0x08,
		byte_ptr__ecx__cl = 0x09,
		byte_ptr__edx__cl = 0x0A,
		byte_ptr__ebx__cl = 0x0B,
		// 0x0C
		// 0x0D
		byte_ptr__esi__cl = 0x0E,
		byte_ptr__edi__cl = 0x0F,

		byte_ptr__eax__dl = 0x10,
		byte_ptr__ecx__dl = 0x11,
		byte_ptr__edx__dl = 0x12,
		byte_ptr__ebx__dl = 0x13,
		// 0x14
		// 0x15
		byte_ptr__esi__dl = 0x16,
		byte_ptr__edi__dl = 0x17,

		byte_ptr__eax__bl = 0x18,
		byte_ptr__ecx__bl = 0x19,
		byte_ptr__edx__bl = 0x1A,
		byte_ptr__ebx__bl = 0x1B,
		// 0x1C
		// 0x1D
		byte_ptr__esi__bl = 0x1E,
		byte_ptr__edi__bl = 0x1F,

		byte_ptr__eax__ah = 0x20,
		byte_ptr__ecx__ah = 0x21,
		byte_ptr__edx__ah = 0x22,
		byte_ptr__ebx__ah = 0x23,
		// 0x24
		// 0x25
		byte_ptr__esi__ah = 0x26,
		byte_ptr__edi__ah = 0x27,

		byte_ptr__eax__ch = 0x28,
		byte_ptr__ecx__ch = 0x29,
		byte_ptr__edx__ch = 0x2A,
		byte_ptr__ebx__ch = 0x2B,
		// 0x2C
		// 0x2D
		byte_ptr__esi__ch = 0x2E,
		byte_ptr__edi__ch = 0x2F,

		byte_ptr__eax__dh = 0x30,
		byte_ptr__ecx__dh = 0x31,
		byte_ptr__edx__dh = 0x32,
		byte_ptr__ebx__dh = 0x33,
		// 0x34
		// 0x35
		byte_ptr__esi__dh = 0x36,
		byte_ptr__edi__dh = 0x37,

		byte_ptr__eax__bh = 0x38,
		byte_ptr__ecx__bh = 0x39,
		byte_ptr__edx__bh = 0x3A,
		byte_ptr__ebx__bh = 0x3B,
		// 0x3C
		// 0x3D
		byte_ptr__esi__bh = 0x3E,
		byte_ptr__edi__bh = 0x3F,

		byte_ptr__eax_add_imm8__al = 0x40,
		byte_ptr__ecx_add_imm8__al = 0x41,
		byte_ptr__edx_add_imm8__al = 0x42,
		byte_ptr__ebx_add_imm8__al = 0x43,
		// 0x44
		byte_ptr__ebp_add_imm8__al = 0x45,
		byte_ptr__esi_add_imm8__al = 0x46,
		byte_ptr__edi_add_imm8__al = 0x47,

		byte_ptr__eax_add_imm8__cl = 0x48,
		byte_ptr__ecx_add_imm8__cl = 0x49,
		byte_ptr__edx_add_imm8__cl = 0x4A,
		byte_ptr__ebx_add_imm8__cl = 0x4B,
		// 0x4C
		byte_ptr__ebp_add_imm8__cl = 0x4D,
		byte_ptr__esi_add_imm8__cl = 0x4E,
		byte_ptr__edi_add_imm8__cl = 0x4F,

		byte_ptr__eax_add_imm8__dl = 0x50,
		byte_ptr__ecx_add_imm8__dl = 0x51,
		byte_ptr__edx_add_imm8__dl = 0x52,
		byte_ptr__ebx_add_imm8__dl = 0x53,
		// 0x54
		byte_ptr__ebp_add_imm8__dl = 0x55,
		byte_ptr__esi_add_imm8__dl = 0x56,
		byte_ptr__edi_add_imm8__dl = 0x57,

		byte_ptr__eax_add_imm8__bl = 0x58,
		byte_ptr__ecx_add_imm8__bl = 0x59,
		byte_ptr__edx_add_imm8__bl = 0x5A,
		byte_ptr__ebx_add_imm8__bl = 0x5B,
		// 0x5C
		byte_ptr__ebp_add_imm8__bl = 0x5D,
		byte_ptr__esi_add_imm8__bl = 0x5E,
		byte_ptr__edi_add_imm8__bl = 0x5F,

		byte_ptr__eax_add_imm8__ah = 0x60,
		byte_ptr__ecx_add_imm8__ah = 0x61,
		byte_ptr__edx_add_imm8__ah = 0x62,
		byte_ptr__ebx_add_imm8__ah = 0x63,
		// 0x64
		byte_ptr__ebp_add_imm8__ah = 0x65,
		byte_ptr__esi_add_imm8__ah = 0x66,
		byte_ptr__edi_add_imm8__ah = 0x67,

		byte_ptr__eax_add_imm8__ch = 0x68,
		byte_ptr__ecx_add_imm8__ch = 0x69,
		byte_ptr__edx_add_imm8__ch = 0x6A,
		byte_ptr__ebx_add_imm8__ch = 0x6B,
		// 0x6C
		byte_ptr__ebp_add_imm8__ch = 0x6D,
		byte_ptr__esi_add_imm8__ch = 0x6E,
		byte_ptr__edi_add_imm8__ch = 0x6F,

		byte_ptr__eax_add_imm8__dh = 0x70,
		byte_ptr__ecx_add_imm8__dh = 0x71,
		byte_ptr__edx_add_imm8__dh = 0x72,
		byte_ptr__ebx_add_imm8__dh = 0x73,
		// 0x74
		byte_ptr__ebp_add_imm8__dh = 0x75,
		byte_ptr__esi_add_imm8__dh = 0x76,
		byte_ptr__edi_add_imm8__dh = 0x77,

		byte_ptr__eax_add_imm8__bh = 0x78,
		byte_ptr__ecx_add_imm8__bh = 0x79,
		byte_ptr__edx_add_imm8__bh = 0x7A,
		byte_ptr__ebx_add_imm8__bh = 0x7B,
		// 0x7C
		byte_ptr__ebp_add_imm8__bh = 0x7D,
		byte_ptr__esi_add_imm8__bh = 0x7E,
		byte_ptr__edi_add_imm8__bh = 0x7F,

		byte_ptr__eax_add_imm32_al = 0x80,
		byte_ptr__ecx_add_imm32_al = 0x81,
		byte_ptr__edx_add_imm32_al = 0x82,
		byte_ptr__ebx_add_imm32_al = 0x83,
		// 0x84
		byte_ptr__ebp_add_imm32_al = 0x85,
		byte_ptr__esi_add_imm32_al = 0x86,
		byte_ptr__edi_add_imm32_al = 0x87,

		byte_ptr__eax_add_imm32_cl = 0x88,
		byte_ptr__ecx_add_imm32_cl = 0x89,
		byte_ptr__edx_add_imm32_cl = 0x8A,
		byte_ptr__ebx_add_imm32_cl = 0x8B,
		// 0x8C
		byte_ptr__ebp_add_imm32_cl = 0x8D,
		byte_ptr__esi_add_imm32_cl = 0x8E,
		byte_ptr__edi_add_imm32_cl = 0x8F,

		byte_ptr__eax_add_imm32_dl = 0x90,
		byte_ptr__ecx_add_imm32_dl = 0x91,
		byte_ptr__edx_add_imm32_dl = 0x92,
		byte_ptr__ebx_add_imm32_dl = 0x93,
		// 0x94
		byte_ptr__ebp_add_imm32_dl = 0x95,
		byte_ptr__esi_add_imm32_dl = 0x96,
		byte_ptr__edi_add_imm32_dl = 0x97,

		byte_ptr__eax_add_imm32_bl = 0x98,
		byte_ptr__ecx_add_imm32_bl = 0x99,
		byte_ptr__edx_add_imm32_bl = 0x9A,
		byte_ptr__ebx_add_imm32_bl = 0x9B,
		// 0x9C
		byte_ptr__ebp_add_imm32_bl = 0x9D,
		byte_ptr__esi_add_imm32_bl = 0x9E,
		byte_ptr__edi_add_imm32_bl = 0x9F,

		byte_ptr__eax_add_imm32_ah = 0xA0,
		byte_ptr__ecx_add_imm32_ah = 0xA1,
		byte_ptr__edx_add_imm32_ah = 0xA2,
		byte_ptr__ebx_add_imm32_ah = 0xA3,
		// 0xA4
		byte_ptr__ebp_add_imm32_ah = 0xA5,
		byte_ptr__esi_add_imm32_ah = 0xA6,
		byte_ptr__edi_add_imm32_ah = 0xA7,

		byte_ptr__eax_add_imm32_ch = 0xA8,
		byte_ptr__ecx_add_imm32_ch = 0xA9,
		byte_ptr__edx_add_imm32_ch = 0xAA,
		byte_ptr__ebx_add_imm32_ch = 0xAB,
		// 0xAC
		byte_ptr__ebp_add_imm32_ch = 0xAD,
		byte_ptr__esi_add_imm32_ch = 0xAE,
		byte_ptr__edi_add_imm32_ch = 0xAF,

		byte_ptr__eax_add_imm32_dh = 0xB0,
		byte_ptr__ecx_add_imm32_dh = 0xB1,
		byte_ptr__edx_add_imm32_dh = 0xB2,
		byte_ptr__ebx_add_imm32_dh = 0xB3,
		// 0xB4
		byte_ptr__ebp_add_imm32_dh = 0xB5,
		byte_ptr__esi_add_imm32_dh = 0xB6,
		byte_ptr__edi_add_imm32_dh = 0xB7,

		byte_ptr__eax_add_imm32_bh = 0xB8,
		byte_ptr__ecx_add_imm32_bh = 0xB9,
		byte_ptr__edx_add_imm32_bh = 0xBA,
		byte_ptr__ebx_add_imm32_bh = 0xBB,
		// 0xBC
		byte_ptr__ebp_add_imm32_bh = 0xBD,
		byte_ptr__esi_add_imm32_bh = 0xBE,
		byte_ptr__edi_add_imm32_bh = 0xBF,

		al_al = 0xC0,
		cl_al = 0xC1,
		dl_al = 0xC2,
		bl_al = 0xC3,
		ah_al = 0xC4,
		ch_al = 0xC5,
		dh_al = 0xC6,
		bh_al = 0xC7,

		al_cl = 0xC8,
		cl_cl = 0xC9,
		dl_cl = 0xCA,
		bl_cl = 0xCB,
		ah_cl = 0xCC,
		ch_cl = 0xCD,
		dh_cl = 0xCE,
		bh_cl = 0xCF,

		al_dl = 0xD0,
		cl_dl = 0xD1,
		dl_dl = 0xD2,
		bl_dl = 0xD3,
		ah_dl = 0xD4,
		ch_dl = 0xD5,
		dh_dl = 0xD6,
		bh_dl = 0xD7,

		al_bl = 0xD8,
		cl_bl = 0xD9,
		dl_bl = 0xDA,
		bl_bl = 0xDB,
		ah_bl = 0xDC,
		ch_bl = 0xDD,
		dh_bl = 0xDE,
		bh_bl = 0xDF,

		al_ah = 0xE0,
		cl_ah = 0xE1,
		dl_ah = 0xE2,
		bl_ah = 0xE3,
		ah_ah = 0xE4,
		ch_ah = 0xE5,
		dh_ah = 0xE6,
		bh_ah = 0xE7,

		al_ch = 0xE8,
		cl_ch = 0xE9,
		dl_ch = 0xEA,
		bl_ch = 0xEB,
		ah_ch = 0xEC,
		ch_ch = 0xED,
		dh_ch = 0xEE,
		bh_ch = 0xEF,

		al_dh = 0xF0,
		cl_dh = 0xF1,
		dl_dh = 0xF2,
		bl_dh = 0xF3,
		ah_dh = 0xF4,
		ch_dh = 0xF5,
		dh_dh = 0xF6,
		bh_dh = 0xF7,

		al_bh = 0xF8,
		cl_bh = 0xF9,
		dl_bh = 0xFA,
		bl_bh = 0xFB,
		ah_bh = 0xFC,
		ch_bh = 0xFD,
		dh_bh = 0xFE,
		bh_bh = 0xFF,
	};
	// enum of x86 machine operand code for 32-bit register or memory
	enum x86_machine_operand_u32 : uint8_t
	{
		dword_ptr__eax__eax = 0x00,
		dword_ptr__ecx__eax = 0x01,
		dword_ptr__edx__eax = 0x02,
		dword_ptr__ebx__eax = 0x03,
		// 0x04
		// 0x05
		dword_ptr__esi__eax = 0x06,
		dword_ptr__edi__eax = 0x07,
		dword_ptr__eax__ecx = 0x08,
		dword_ptr__ecx__ecx = 0x09,
		dword_ptr__edx__ecx = 0x0A,
		dword_ptr__ebx__ecx = 0x0B,
		// 0x0C
		// 0x0D
		dword_ptr__esi__ecx = 0x0E,
		dword_ptr__edi__ecx = 0x0F,

		dword_ptr__eax__edx = 0x10,
		dword_ptr__ecx__edx = 0x11,
		dword_ptr__edx__edx = 0x12,
		dword_ptr__ebx__edx = 0x13,
		// 0x14
		// 0x15
		dword_ptr__esi__edx = 0x16,
		dword_ptr__edi__edx = 0x17,
		dword_ptr__eax__ebx = 0x18,
		dword_ptr__ecx__ebx = 0x19,
		dword_ptr__edx__ebx = 0x1A,
		dword_ptr__ebx__ebx = 0x1B,
		// 0x1C
		// 0x1D
		dword_ptr__esi__ebx = 0x1E,
		dword_ptr__edi__ebx = 0x1F,

		dword_ptr__eax__esp = 0x20,
		dword_ptr__ecx__esp = 0x21,
		dword_ptr__edx__esp = 0x22,
		dword_ptr__ebx__esp = 0x23,
		// 0x24
		// 0x25
		dword_ptr__esi__esp = 0x26,
		dword_ptr__edi__esp = 0x27,
		dword_ptr__eax__ebp = 0x28,
		dword_ptr__ecx__ebp = 0x29,
		dword_ptr__edx__ebp = 0x2A,
		dword_ptr__ebx__ebp = 0x2B,
		// 0x2C
		// 0x2D
		dword_ptr__esi__ebp = 0x2E,
		dword_ptr__edi__ebp = 0x2F,

		dword_ptr__eax__esi = 0x30,
		dword_ptr__ecx__esi = 0x31,
		dword_ptr__edx__esi = 0x32,
		dword_ptr__ebx__esi = 0x33,
		// 0x34
		// 0x35
		dword_ptr__esi__esi = 0x36,
		dword_ptr__edi__esi = 0x37,
		dword_ptr__eax__edi = 0x38,
		dword_ptr__ecx__edi = 0x39,
		dword_ptr__edx__edi = 0x3A,
		dword_ptr__ebx__edi = 0x3B,
		// 0x3C
		// 0x3D
		dword_ptr__esi__edi = 0x3E,
		dword_ptr__edi__edi = 0x3F,

		dword_ptr__eax_add_imm8__eax = 0x40,
		dword_ptr__ecx_add_imm8__eax = 0x41,
		dword_ptr__edx_add_imm8__eax = 0x42,
		dword_ptr__ebx_add_imm8__eax = 0x43,
		// 0x44
		dword_ptr__ebp_add_imm8__eax = 0x45,
		dword_ptr__esi_add_imm8__eax = 0x46,
		dword_ptr__edi_add_imm8__eax = 0x47,
		dword_ptr__eax_add_imm8__ecx = 0x48,
		dword_ptr__ecx_add_imm8__ecx = 0x49,
		dword_ptr__edx_add_imm8__ecx = 0x4A,
		dword_ptr__ebx_add_imm8__ecx = 0x4B,
		// 0x4C
		dword_ptr__ebp_add_imm8__ecx = 0x4D,
		dword_ptr__esi_add_imm8__ecx = 0x4E,
		dword_ptr__edi_add_imm8__ecx = 0x4F,

		dword_ptr__eax_add_imm8__edx = 0x50,
		dword_ptr__ecx_add_imm8__edx = 0x51,
		dword_ptr__edx_add_imm8__edx = 0x52,
		dword_ptr__ebx_add_imm8__edx = 0x53,
		// 0x54
		dword_ptr__ebp_add_imm8__edx = 0x55,
		dword_ptr__esi_add_imm8__edx = 0x56,
		dword_ptr__edi_add_imm8__edx = 0x57,
		dword_ptr__eax_add_imm8__ebx = 0x58,
		dword_ptr__ecx_add_imm8__ebx = 0x59,
		dword_ptr__edx_add_imm8__ebx = 0x5A,
		dword_ptr__ebx_add_imm8__ebx = 0x5B,
		// 0x5C
		dword_ptr__ebp_add_imm8__ebx = 0x5D,
		dword_ptr__esi_add_imm8__ebx = 0x5E,
		dword_ptr__edi_add_imm8__ebx = 0x5F,

		dword_ptr__eax_add_imm8__esp = 0x60,
		dword_ptr__ecx_add_imm8__esp = 0x61,
		dword_ptr__edx_add_imm8__esp = 0x62,
		dword_ptr__ebx_add_imm8__esp = 0x63,
		// 0x64
		dword_ptr__ebp_add_imm8__esp = 0x65,
		dword_ptr__esi_add_imm8__esp = 0x66,
		dword_ptr__edi_add_imm8__esp = 0x67,
		dword_ptr__eax_add_imm8__ebp = 0x68,
		dword_ptr__ecx_add_imm8__ebp = 0x69,
		dword_ptr__edx_add_imm8__ebp = 0x6A,
		dword_ptr__ebx_add_imm8__ebp = 0x6B,
		// 0x6C
		dword_ptr__ebp_add_imm8__ebp = 0x6D,
		dword_ptr__esi_add_imm8__ebp = 0x6E,
		dword_ptr__edi_add_imm8__ebp = 0x6F,

		dword_ptr__eax_add_imm8__esi = 0x70,
		dword_ptr__ecx_add_imm8__esi = 0x71,
		dword_ptr__edx_add_imm8__esi = 0x72,
		dword_ptr__ebx_add_imm8__esi = 0x73,
		// 0x74
		dword_ptr__ebp_add_imm8__esi = 0x75,
		dword_ptr__esi_add_imm8__esi = 0x76,
		dword_ptr__edi_add_imm8__esi = 0x77,
		dword_ptr__eax_add_imm8__edi = 0x78,
		dword_ptr__ecx_add_imm8__edi = 0x79,
		dword_ptr__edx_add_imm8__edi = 0x7A,
		dword_ptr__ebx_add_imm8__edi = 0x7B,
		// 0x7C
		dword_ptr__ebp_add_imm8__edi = 0x7D,
		dword_ptr__esi_add_imm8__edi = 0x7E,
		dword_ptr__edi_add_imm8__edi = 0x7F,

		dword_ptr__eax_add_imm32__eax = 0x80,
		dword_ptr__ecx_add_imm32__eax = 0x81,
		dword_ptr__edx_add_imm32__eax = 0x82,
		dword_ptr__ebx_add_imm32__eax = 0x83,
		// 0x84
		dword_ptr__ebp_add_imm32__eax = 0x85,
		dword_ptr__esi_add_imm32__eax = 0x86,
		dword_ptr__edi_add_imm32__eax = 0x87,
		dword_ptr__eax_add_imm32__ecx = 0x88,
		dword_ptr__ecx_add_imm32__ecx = 0x89,
		dword_ptr__edx_add_imm32__ecx = 0x8A,
		dword_ptr__ebx_add_imm32__ecx = 0x8B,
		// 0x8C
		dword_ptr__ebp_add_imm32__ecx = 0x8D,
		dword_ptr__esi_add_imm32__ecx = 0x8E,
		dword_ptr__edi_add_imm32__ecx = 0x8F,

		dword_ptr__eax_add_imm32__edx = 0x90,
		dword_ptr__ecx_add_imm32__edx = 0x91,
		dword_ptr__edx_add_imm32__edx = 0x92,
		dword_ptr__ebx_add_imm32__edx = 0x93,
		// 0x94
		dword_ptr__ebp_add_imm32__edx = 0x95,
		dword_ptr__esi_add_imm32__edx = 0x96,
		dword_ptr__edi_add_imm32__edx = 0x97,
		dword_ptr__eax_add_imm32__ebx = 0x98,
		dword_ptr__ecx_add_imm32__ebx = 0x99,
		dword_ptr__edx_add_imm32__ebx = 0x9A,
		dword_ptr__ebx_add_imm32__ebx = 0x9B,
		// 0x9C
		dword_ptr__ebp_add_imm32__ebx = 0x9D,
		dword_ptr__esi_add_imm32__ebx = 0x9E,
		dword_ptr__edi_add_imm32__ebx = 0x9F,

		dword_ptr__eax_add_imm32__esp = 0xA0,
		dword_ptr__ecx_add_imm32__esp = 0xA1,
		dword_ptr__edx_add_imm32__esp = 0xA2,
		dword_ptr__ebx_add_imm32__esp = 0xA3,
		// 0xA4
		dword_ptr__ebp_add_imm32__esp = 0xA5,
		dword_ptr__esi_add_imm32__esp = 0xA6,
		dword_ptr__edi_add_imm32__esp = 0xA7,
		dword_ptr__eax_add_imm32__ebp = 0xA8,
		dword_ptr__ecx_add_imm32__ebp = 0xA9,
		dword_ptr__edx_add_imm32__ebp = 0xAA,
		dword_ptr__ebx_add_imm32__ebp = 0xAB,
		// 0xAC
		dword_ptr__ebp_add_imm32__ebp = 0xAD,
		dword_ptr__esi_add_imm32__ebp = 0xAE,
		dword_ptr__edi_add_imm32__ebp = 0xAF,

		dword_ptr__eax_add_imm32__esi = 0xB0,
		dword_ptr__ecx_add_imm32__esi = 0xB1,
		dword_ptr__edx_add_imm32__esi = 0xB2,
		dword_ptr__ebx_add_imm32__esi = 0xB3,
		// 0xB4
		dword_ptr__ebp_add_imm32__esi = 0xB5,
		dword_ptr__esi_add_imm32__esi = 0xB6,
		dword_ptr__edi_add_imm32__esi = 0xB7,
		dword_ptr__eax_add_imm32__edi = 0xB8,
		dword_ptr__ecx_add_imm32__edi = 0xB9,
		dword_ptr__edx_add_imm32__edi = 0xBA,
		dword_ptr__ebx_add_imm32__edi = 0xBB,
		// 0xBC
		dword_ptr__ebp_add_imm32__edi = 0xBD,
		dword_ptr__esi_add_imm32__edi = 0xBE,
		dword_ptr__edi_add_imm32__edi = 0xBF,

		eax_eax = 0xC0,
		ecx_eax = 0xC1,
		edx_eax = 0xC2,
		ebx_eax = 0xC3,
		esp_eax = 0xC4,
		ebp_eax = 0xC5,
		esi_eax = 0xC6,
		edi_eax = 0xC7,
		eax_ecx = 0xC8,
		ecx_ecx = 0xC9,
		edx_ecx = 0xCA,
		ebx_ecx = 0xCB,
		esp_ecx = 0xCC,
		ebp_ecx = 0xCD,
		esi_ecx = 0xCE,
		edi_ecx = 0xCF,

		eax_edx = 0xD0,
		ecx_edx = 0xD1,
		edx_edx = 0xD2,
		ebx_edx = 0xD3,
		esp_edx = 0xD4,
		ebp_edx = 0xD5,
		esi_edx = 0xD6,
		edi_edx = 0xD7,
		eax_ebx = 0xD8,
		ecx_ebx = 0xD9,
		edx_ebx = 0xDA,
		ebx_ebx = 0xDB,
		esp_ebx = 0xDC,
		ebp_ebx = 0xDD,
		esi_ebx = 0xDE,
		edi_ebx = 0xDF,

		eax_esp = 0xE0,
		ecx_esp = 0xE1,
		edx_esp = 0xE2,
		ebx_esp = 0xE3,
		esp_esp = 0xE4,
		ebp_esp = 0xE5,
		esi_esp = 0xE6,
		edi_esp = 0xE7,
		eax_ebp = 0xE8,
		ecx_ebp = 0xE9,
		edx_ebp = 0xEA,
		ebx_ebp = 0xEB,
		esp_ebp = 0xEC,
		ebp_ebp = 0xED,
		esi_ebp = 0xEE,
		edi_ebp = 0xEF,

		eax_esi = 0xF0,
		ecx_esi = 0xF1,
		edx_esi = 0xF2,
		ebx_esi = 0xF3,
		esp_esi = 0xF4,
		ebp_esi = 0xF5,
		esi_esi = 0xF6,
		edi_esi = 0xF7,
		eax_edi = 0xF8,
		ecx_edi = 0xF9,
		edx_edi = 0xFA,
		ebx_edi = 0xFB,
		esp_edi = 0xFC,
		ebp_edi = 0xFD,
		esi_edi = 0xFE,
		edi_edi = 0xFF,
	};

	int asm2machine_v1(const char *asmcode_in, int asmsize, uint8_t *machinecode)
	{
		char asmcode[128] = {0};
		// copy asmcode_in to asmcode, replace '\t' with ' ', remove repeat ' '
		int i = 0;
		int j = 0;
		for (i = 0; i < asmsize && j < 128; i++)
		{
			if (asmcode_in[i] == '\t')
			{
				asmcode[j] = ' ';
				j++;
			}
			else if (asmcode_in[i] == ' ')
			{
				if (j > 0 && asmcode[j - 1] != ' ')
				{
					asmcode[j] = asmcode_in[i];
					j++;
				}
			}
			else
			{
				asmcode[j] = asmcode_in[i];
				j++;
			}
		}
		asmsize = j;
		asmcode[asmsize] = '\0';
		char token1[64] = {0};
		// get the first token, operation code
		for (i = 0; i < 64; i++)
		{
			if (asmcode[i] == ' ')
			{
				break;
			}
			token1[i] = asmcode[i];
		}
		// get the second token, operand1, end with ','or '\0'
		char token2[64] = {0};
		for (j = 0; j < 64; j++)
		{
			if (asmcode[i] == '\0')
			{
				break;
			}
			else if (asmcode[i] == ',')
			{
				i++;
				break;
			}
			token2[j] = asmcode[i];
			i++;
		}
		// use asm_oprand_subx86 to get the operand1
		asm_oprand_subx86 op1(token2);
		// get the third token, operand2, end with  '\0'
		char token3[64] = {0};
		for (j = 0; j < 64; j++)
		{
			if (asmcode[i] == '\0')
			{
				break;
			}
			token3[j] = asmcode[i];
			i++;
		}
		// use asm_oprand_subx86 to get the operand2
		asm_oprand_subx86 op2(token3);

		size_t op_size1 = op1.operand_size_bit;
		size_t op_size2 = op2.operand_size_bit;
		// if size is 0, means not sure, use the size of another operand
		// if size is not 0, size should be same
		// if both size is 0, use 32 bit
		if (op_size1 == 0 && op_size2 == 0)
		{
			op_size1 = 32;
			op_size2 = 32;
		}
		else if (op_size1 == 0)
		{
			op_size1 = op_size2;
		}
		else if (op_size2 == 0)
		{
			op_size2 = op_size1;
		}
		if (op_size1 != op_size2)
		{
			printf("operand size is not same");
			return -1;
		}
	}
};

// assembler of x86, but in my style
class asm_x86_bionukg
{
public:
	enum operation_2 : uint8_t
	{
		add_ = 0,
		or_ = 1,
		adc_ = 2,
		sbb_ = 3,
		and_ = 4,
		sub_ = 5,
		xor_ = 6,
		cmp_ = 7,
	};
	enum operand_type_rm : uint8_t
	{
		reg_to_mem_8 = 0,
		reg_to_mem_32 = 1,
		mem_to_reg_8 = 2,
		mem_to_reg_32 = 3,

	};
	enum operand_type_imm8 : uint8_t
	{
		imm_to_reg_8 = 4,
	};
	enum operand_type_imm32 : uint8_t
	{
		imm_to_reg_32 = 5,
	};
	enum operand_value_type_rm : uint8_t
	{
		mem_reg = 0,
		reg_reg = 3,
	};
	enum operand_value_type_imm : uint8_t
	{
		mem_i8_reg = 1,
		mem_i32_reg = 2,
	};
	enum operand_value : uint8_t
	{
		eax = 0,
		ecx = 1,
		edx = 2,
		ebx = 3,
		esp = 4,
		ebp = 5,
		esi = 6,
		edi = 7,
	};
	enum operation_ji8 : uint8_t
	{
		jo = 0x70,	// jump if OF==1
		jno = 0x71, // jump if OF==0
		jb = 0x72,	// jump if CF==1
		jnae = 0x72,
		jc = 0x72,
		jnb = 0x73, // jump if CF==0
		jae = 0x73,
		jnc = 0x73,
		jz = 0x74,	// jump if ZF==1
		jne = 0x75, // jump if ZF==0
		jbe = 0x76, // jump if CF==1 or ZF==1
		jna = 0x76,
		jnbe = 0x77, // jump if CF==0 and ZF==0
		ja = 0x77,
		js = 0x78,	// jump if SF==1
		jns = 0x79, // jump if SF==0
		jp = 0x7a,	// jump if PF==1
		jpe = 0x7a,
		jnp = 0x7b, // jump if PF==0
		jpo = 0x7b,
		jl = 0x7c, // jump if SF!=OF
		jnge = 0x7c,
		jnl = 0x7d, // jump if SF==OF
		jge = 0x7d,
		jle = 0x7e, // jump if ZF==1 or SF!=OF
		jng = 0x7e,
		jnle = 0x7f, // jump if ZF==0 and SF==OF
		jg = 0x7f,
		jmp = 0xe9,	   // jump without condition
		loopne = 0xE0, // ecx--, jump if ecx!=0 and ZF==0
		loope = 0xE1,  // ecx--, jump if ecx!=0 and ZF==1
		loop = 0xE2,   // ecx--, jump if ecx!=0
		jcxz = 0xE3,   // jump if ecx==0
	};
	enum operation_1 : uint8_t // operation with only 1 BYTE
	{
		push_es = 0x06,
		pop_es = 0x07,
		push_cs = 0x0e,
		push_ss = 0x16,
		pop_ss = 0x17,
		push_ds = 0x1e,
		pop_ds = 0x1f,

		inc_eax = 0x40,
		inc_ecx = 0x41,
		inc_edx = 0x42,
		inc_ebx = 0x43,
		inc_esp = 0x44,
		inc_ebp = 0x45,
		inc_esi = 0x46,
		inc_edi = 0x47,
		dec_eax = 0x48,
		dec_ecx = 0x49,
		dec_edx = 0x4A,
		dec_ebx = 0x4B,
		dec_esp = 0x4C,
		dec_ebp = 0x4D,
		dec_esi = 0x4E,
		dec_edi = 0x4F,
		push_eax = 0x50,
		push_ecx = 0x51,
		push_edx = 0x52,
		push_ebx = 0x53,
		push_esp = 0x54,
		push_ebp = 0x55,
		push_esi = 0x56,
		push_edi = 0x57,
		pop_eax = 0x58,
		pop_ecx = 0x59,
		pop_edx = 0x5A,
		pop_ebx = 0x5B,
		pop_esp = 0x5C,
		pop_ebp = 0x5D,
		pop_esi = 0x5E,
		pop_edi = 0x5F,
		pushad = 0x60, // push all general purpose registers
		popad = 0x61,  // pop all general purpose registers
		nop = 0x90,	   // no operation
		xchg_eax_ecx = 0x91,
		xchg_eax_edx = 0x92,
		xchg_eax_ebx = 0x93,
		xchg_eax_esp = 0x94,
		xchg_eax_ebp = 0x95,
		xchg_eax_esi = 0x96,
		xchg_eax_edi = 0x97,
		pushfd = 0x9c, // push all flags
		popfd = 0x9d,  // pop all flags

		ret = 0xc3,
		int3 = 0xcc,
		int_3 = 0xcc,
		cmc = 0xf5, // CF=CF xor 1
		clc = 0xf8, // CF=0
		stc = 0xf9, // CF=1
		cli = 0xfa, // IF=0
		sti = 0xfb, // IF=1
		cld = 0xfc, // DF=0
		std = 0xfd, // DF=1

	};
	enum operation_2_no_imm : uint8_t
	{
		test = 0x84 >> 1,
		xchg_reg_to_mem_ = 0x86 >> 1,
		mov_reg_to_mem = 0x88 >> 1,
		mov_mem_to_reg_ = 0x8A >> 1,
	};
	enum operation_2_no_imm_width : uint8_t
	{
		u8 = 0,
		u32 = 1
	};
	union union_code
	{
		typedef struct op2_mr_
		{
			operation_2 op_2 : 5;
			operand_type_rm type : 3;
			operand_value_type_rm value_type : 2;
			operand_value operand1 : 3;
			operand_value operand2 : 3;
		} op2_mr;
		typedef struct op2_imm8
		{
			operation_2 op_2 : 5;
			operand_type_imm8 type : 3;
			uint8_t imm8 : 8;
		};
		typedef struct op2_imm32
		{
			operation_2 op_2 : 5;
			operand_type_imm32 type : 3;
			uint32_t imm32 : 32;
		};
		typedef struct ji8_
		{
			operation_ji8 op_jcc : 8;
			int8_t offset : 8;
		} ji8;
		typedef struct op1_
		{
			operation_1 op_1 : 8;
		} op1;
		typedef struct op2_no_imm
		{
			operation_2_no_imm op_2 : 7;
			operation_2_no_imm_width width : 1;
			operand_value_type_rm value_type : 2;
			operand_value operand1 : 3;
			operand_value operand2 : 3;
		} op2_no_imm;
	};
};

enum assembler_type
{
	asm_x86 = 0,
	asm_x86_64 = 1,
};
