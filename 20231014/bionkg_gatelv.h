#pragma once
#ifndef BIONKG_GATELV_H
#define BIONKG_GATELV_H

#include <stdint.h>

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg
    class nBit;
    class nBit_bool;


    class nBit
    {
        uint8_t *data;
        size_t bitSize;
        uint64_t buf;
    public:
        nBit()
        {
            this->data = (uint8_t *)&buf;
            this->bitSize = 0;
            buf = 0;
        }
        nBit(uint8_t data)
        {
            this->data = (uint8_t *)&buf;
            this->bitSize = 8;
            buf = data;
        }
        nBit(uint16_t data)
        {
            this->data = (uint8_t *)&buf;
            this->bitSize = 16;
            buf = data;
        }
        nBit(uint32_t data)
        {
            this->data = (uint8_t *)&buf;
            this->bitSize = 32;
            buf = data;
        }
        nBit(uint64_t data)
        {
            this->data = (uint8_t *)&buf;
            this->bitSize = 64;
            buf = data;
        }
        nBit(const nBit &that)
        {
            this->data = (uint8_t *)&buf;
            this->bitSize = 0;
            *this = that;
        }
        void resize(size_t size)
        {
            uint8_t *oldData = this->data;
            if (size > this->bitSize)
            {
                if (size > sizeof(buf) * 8)
                {
                    this->data = new uint8_t[size / 8 + 1];
                }
            }
            else
            {
                if (size <= sizeof(buf) * 8)
                {
                    this->data = (uint8_t *)&buf;
                }
                else
                {
                    this->data = new uint8_t[size / 8 + 1];
                }
            }
            if (this->data != oldData)
            {
                for (size_t i = 0; i < this->bitSize / 8; i++)
                {
                    this->data[i] = oldData[i];
                }
                if (oldData != (uint8_t *)&buf)
                {
                    delete[] oldData;
                }
            }
            this->bitSize = size;
        }
        size_t size() const
        {
            return this->bitSize;
        }

        void set(size_t idx)
        { // set idx bit to 1
            if (idx < this->bitSize)
            {
                this->data[idx / 8] |= 1 << (idx % 8);
            }
            return;
        }
        void clr(size_t idx)
        { // set idx bit to 0
            if (idx < this->bitSize)
            {
                this->data[idx / 8] &= ~(1 << (idx % 8));
            }
            return;
        }
        void flip(size_t idx)
        { // flip idx bit
            if (idx < this->bitSize)
            {
                this->data[idx / 8] ^= 1 << (idx % 8);
            }
            return;
        }
        void operator=(const nBit &that)
        {
            this->resize(that.bitSize);
            for (size_t i = 0; i < this->bitSize / 8; i++)
            {
                this->data[i] = that.data[i];
            }
            return;
        }
        void operator=(uint8_t that)
        {
            this->resize(8);
            buf = that;
            return;
        }
        void operator=(uint16_t that)
        {
            this->resize(16);
            buf = that;
            return;
        }
        void operator=(uint32_t that)
        {
            this->resize(32);
            buf = that;
            return;
        }
        void operator=(uint64_t that)
        {
            this->resize(64);
            buf = that;
            return;
        }
        nBit_bool operator[](size_t idx);
        bool operator[](size_t idx) const
        {
            if (idx < this->bitSize)
            {
                return this->data[idx / 8] & (1 << (idx % 8));
            }
            else
            {
                return 0;
            }
        }
    };

    class nBit_bool
    {
        nBit* data;
        size_t idx;
    public:
        nBit_bool()
        {
            this->data = 0;
            this->idx = 0;
        }
        nBit_bool(nBit* data, size_t idx)
        {
            this->data = data;
            this->idx = idx;
        }
        nBit_bool(const nBit_bool &that)
        {
            this->data = that.data;
            this->idx = that.idx;
        }
        void operator=(const nBit_bool &that)
        {
            *this = that.data->operator[](that.idx);
        }
        void operator=(bool that)
        {
            if (that)
            {
                this->data->set(this->idx);
            }
            else
            {
                this->data->clr(this->idx);
            }
        }
        operator bool()
        {
            return ((const nBit*)(this->data))->operator[](this->idx);
        }
        bool value() const
        {
            return ((const nBit*)(this->data))->operator[](this->idx);
        }
    };

    nBit_bool nBit::operator[](size_t idx)
    {
        return nBit_bool(this, idx);
    }
    enum gateEnum
    {
        nand_g = 0,
        nor_g = 1,
        and_g = 2,
        or_g = 3,
        xor_g = 4,
        xnor_g = 5,
    };

    template <int gate_width = 4, int gate_height = 4>
    class gateConnection
    {
    public:
        constexpr static int pin_count = gate_height * 2;
        using pin_t = uint8_t;
        pin_t connection[gate_width][gate_height][2] = {0};
        pin_t out[gate_height] = {0};
        gateConnection()
        {
        }
        gateConnection(const gateConnection &that)
        {
            *this = that;
        }
        void operator=(const gateConnection &that)
        {
            for (size_t x = 0; x < gate_width; x++)
            {
                for (size_t y = 0; y < gate_height; y++)
                {
                    this->connection[x][y] = that.connection[x][y];
                }
            }
        }
        void set(size_t x, size_t y, uint8_t pin, uint8_t isUp = 1)
        {
            if (x < gate_width && y < gate_height && pin < pin_count)
            {
                this->connection[x][y][isUp] = pin;
            }
        }
        void clr(size_t x, size_t y, uint8_t pin, uint8_t isUp = 1)
        {
            if (x < gate_width && y < gate_height && pin < pin_count)
            {
                this->connection[x][y][isUp] = 0;
            }
        }
        pin_t get_up(size_t x, size_t y)
        {
            if (x < gate_width && y < gate_height)
            {
                return this->connection[x][y][1];
            }
            else
            {
                return 0;
            }
        }
        pin_t get_down(size_t x, size_t y)
        {
            if (x < gate_width && y < gate_height)
            {
                return this->connection[x][y][0];
            }
            else
            {
                return 0;
            }
        }
        
        /// @brief out[0] is sum, out[1] is carry
        void set_full_adder_nand()
        {
            static_assert(gate_width==6&&gate_height==3,"gate_width==6&&gate_height==3 for full_adder_nand");
            uint8_t map[6][3][2] = {
                {{0, 1}, {2, 2}, {0, 0}},
                {{3, 0}, {4, 0}, {1, 1}},
                {{0, 1}, {3, 3}, {2, 2}},
                {{0, 5}, {1, 1}, {2, 2}},
                {{0, 3}, {0, 2}, {0, 1}},
                {{0, 1}, {0, 0}, {0, 0}},
            };
            uint8_t sum=0,carry=5;
            out[0]=sum;
            out[1]=carry;
            for (size_t x = 0; x < 6; x++)
            {
                for (size_t y = 0; y < 3; y++)
                {
                    connection[x][y][0] = map[x][y][0];
                    connection[x][y][1] = map[x][y][1];
                }
            }
            return;
        }
    
    
    };

    template <int gate_width = 4, int gate_height = 4, gateEnum gate_type = nand_g>
    class GateLevel
    {
    public:
        constexpr static int pin_count = gate_height * 2;
        constexpr static int connection_count = gate_width * gate_height * pin_count;
        constexpr static int gate_count = gate_width * gate_height;
        constexpr static int max_input_count = pin_count;
        constexpr static int max_output_count = pin_count;

        gateConnection<gate_width, gate_height> connection;

        GateLevel()
        {
            
        }
        GateLevel(const nBit &connection)
        {
            this->connection = connection;
        }

        void run(const nBit &input, nBit &output)
        {
            output.resize(max_output_count);

            uint8_t pins[pin_count] = {}, buf[gate_height] = {};
            for (size_t i = 0; i < pin_count && i < input.size(); i++)
            {
                pins[i] = input[i];
            }


            static_assert(gate_type == nand_g, "gate_type == nand_g");
            auto logic = [](bool a, bool b) { return !(a && b); };
            
            for (size_t x = 0; x < gate_width; x++)
            {
                // from the first column to the last column
                for (size_t y = 0; y < gate_height; y++)
                {
                    auto p_up=connection.get_up(x, y);
                    auto p_down=connection.get_down(x, y);
                    buf[y] = logic(pins[p_up], pins[p_down]);
                }
                // pins(first half)->pins(second half);buf->pins(first half)
                for (size_t y = 0; y < gate_height; y++)
                {
                    pins[y + gate_height] = pins[y];
                    pins[y] = buf[y];
                }
            }
            for (size_t i = 0; i < max_output_count; i++)
            {
                output[i] = pins[connection.out[i]];
            }
        
        }
    };

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // BIONKG_GATELV_H