#pragma once

#ifndef _bionukg_metasm_h
#define _bionukg_metasm_h
#include <stdint.h>
#include <vector>

#ifdef namespace_bionukg
namespace bionukg
{
#endif

    namespace _x86_
    {
        struct instruction
        {
            uint8_t size;
            uint8_t opcode[15];
        };
        class CodeX86
        {
            std::vector<uint8_t> _opcode;

        public:
            CodeX86() {};
            CodeX86(const uint8_t* opcode, uint8_t size)
            {
                _opcode.resize(size);
                memcpy(_opcode.data(), opcode, size);
            }
            CodeX86(const instruction& inst)
            {
                _opcode.resize(inst.size);
                memcpy(_opcode.data(), inst.opcode, inst.size);
            }

            void operator+=(const instruction& inst)
            {
                _opcode.resize(_opcode.size() + inst.size);
                memcpy(_opcode.data() + _opcode.size() - inst.size, inst.opcode, inst.size);
            }


        };
    };

#ifdef namespace_bionukg
};
#endif

#endif // !_bionukg_metasm_h