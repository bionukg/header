// bionukg's string class
#pragma once
#include <iostream>
#include <vector>
#ifndef bionukg_string_h
#define bionukg_string_h

#ifdef namespace_bionukg
namespace bionukg
{
#endif
#ifdef _DEBUG
#define dbgerr(msg) do{printf("%s",msg);throw msg;}while(0);
#elif defined bionukg_print_exception
#define dbgerr(msg) printf(msg)
#else
#define dbgerr(msg)
#endif

    // declarations
    constexpr uint64_t operator""_raw64(const char *str, size_t len);
    constexpr uint64_t operator""_b64(const char *str, size_t len);
    template <typename charT = char>
    class string_;
    template <typename charT = char>
    class string_view_;
    template <typename charT = char>
    class string_view_multiple_;

    template <typename charT>
    class string_
    {
    public:
        size_t StrSize;
        charT *Str;

    private:
        void deleteStr()
        {
            if (Str != nullptr)
            {
                for (size_t i = 0; i < StrSize; i++)
                {
                    Str[i] = charT();
                }
                delete[] Str;
                Str = nullptr;
            }
        }

    public:
        string_() : StrSize(0), Str(nullptr){};
        template <typename = typename std::enable_if<std::is_same<charT, char>::value>::type>
        string_(const char *str)
        {
            StrSize = strlen(str);
            Str = new charT[StrSize + 1];
            for (size_t i = 0; i < StrSize; i++)
            {
                Str[i] = str[i];
            }
            Str[StrSize] = '\0';
        }
        string_(const string_<charT> &str)
        {
            StrSize = str.StrSize;
            Str = new charT[StrSize + 1];
            for (size_t i = 0; i < StrSize; i++)
            {
                Str[i] = str.Str[i];
            }
            Str[StrSize] = charT();
        }
        string_(const string_view_multiple_<charT> &str)
        {
            StrSize = str.StrViewsSizeSum;
            Str = new charT[StrSize + 1];
            size_t index = 0;
            for (size_t i = 0; i < str.StrViewsCount; i++)
            {
                for (size_t j = 0; j < str.StrViews[i].StrViewSize; j++)
                {
                    Str[index] = str.StrViews[i].StrView[j];
                    index++;
                }
            }
            Str[StrSize] = charT();
        }
        ~string_()
        {
            deleteStr();
        }

        // enable only if Ty is a const chatT[]
        template <typename Ty>
        string_(Ty str)
        {
            for (size_t i = 0; str[i] != charT(); i++)
            {
                StrSize++;
            }
            Str = new charT[StrSize + 1];
            for (size_t i = 0; i < StrSize; i++)
            {
                Str[i] = str[i];
            }
        }

        template <typename = typename std::enable_if<std::is_same<charT, char>::value>::type>
        bool been_viewed_by(const string_view_<char> &substr);

        string_view_<charT> find_substr(const string_view_<charT> &substr, size_t start = 0);

        string_view_<charT> findnext_substr(const string_view_<charT> &substr, string_view_<charT> *last_substr = nullptr);

        bool replace_once(const string_view_<charT> &matched_str, const string_view_<charT> &replaced_str, size_t start = 0);

        const string_view_<charT> &substr(size_t start, size_t len) const
        {
            if (start + len > StrSize)
            {
#ifndef bionukg_nothrow
                throw std::out_of_range("substr out of range");
#endif
                return *new string_view_<charT>();
            }
            string_view_<charT> *ret = new string_view_<charT>();
            ret->StrViewSize = len;
            ret->StrView = Str + start;
            return *ret;
        }

        charT &operator[](size_t index) const
        {
            if (index >= StrSize)
            {
#ifndef bionukg_nothrow
                throw std::out_of_range("operator[] out of range");
#endif
                return *new charT();
            }
            return Str[index];
        }
        // allow std::cout to print
        friend std::ostream &operator<<(std::ostream &os, const string_<charT> &str)
        {
            for (size_t i = 0; i < str.StrSize; i++)
            {
                os << str.Str[i];
            }
            return os;
        }
    };

    template <typename charT>
    class string_view_
    {
    public:
        size_t StrViewSize;
        const charT *StrView;

    public:
        string_view_() : StrViewSize(0), StrView(nullptr) {}
        // constructor available only when charT is char
        template <typename = typename std::enable_if<std::is_same<charT, char>::value>::type>
        string_view_(const string_<charT> &str) : StrViewSize(str.StrSize), StrView(str.Str){};
        string_view_(const string_view_<charT> &str) : StrViewSize(str.StrViewSize), StrView(str.StrView) {}
        template <typename = typename std::enable_if<std::is_same<charT, char>::value>::type>
        string_view_(const char *str) : StrViewSize(strlen(str)), StrView(str) {}
        string_view_(const charT *str, size_t size) : StrViewSize(size), StrView(str) {}
        ~string_view_(){};
        bool empty() const
        {
            return StrViewSize == 0;
        }
        charT operator[](size_t index) const
        {
            if (index >= StrViewSize)
            {
#ifndef bionukg_nothrow
                throw std::out_of_range("operator[] out of range");
#endif
                return charT();
            }
            return StrView[index];
        }
        void operator=(const string_view_<charT> &str)
        {
            StrViewSize = str.StrViewSize;
            StrView = str.StrView;
        }
        void operator=(const string_<charT> &str)
        {
            StrViewSize = str.StrSize;
            StrView = str.Str;
        }

        template <typename = typename std::enable_if<std::is_same<charT, char>::value>::type>
        string_view_ next_line(string_view_ *last_line = nullptr, charT end_line = '\n')
        {
            size_t start = 0;
            if (last_line != nullptr)
            {
                start = last_line->StrView - StrView + last_line->StrViewSize + 1;
            }
            size_t end = start;
            for (; end < StrViewSize; end++)
            {
                if (StrView[end] == end_line)
                {
                    break;
                }
            }
            string_view_ ret;
            ret.StrView = StrView + start;
            ret.StrViewSize = end - start;
            return ret;
        }
        string_view_ next_piece(charT separator, string_view_ *last_piece = nullptr) const
        {
            size_t start = 0;
            if (last_piece != nullptr)
            {
                start = last_piece->StrView - StrView + last_piece->StrViewSize + 1;
            }
            size_t end = start;
            for (; end < StrViewSize; end++)
            {
                if (StrView[end] == separator)
                {
                    break;
                }
            }
            string_view_ ret;
            ret.StrView = StrView + start;
            ret.StrViewSize = end - start;
            return ret;
        }

        // allow std::cout to print string_view_
        friend std::ostream &operator<<(std::ostream &os, const string_view_<charT> &str)
        {
            for (size_t i = 0; i < str.StrViewSize; i++)
            {
                os << str.StrView[i];
            }
            return os;
        }
    };

    template <typename charT>
    class string_view_multiple_
    {
    public:
        string_view_<charT> *StrViews;
        size_t StrViewsSizeSum;
        size_t StrViewsCount;

    public:
        string_view_multiple_() : StrViews(nullptr), StrViewsSizeSum(0){};
        string_view_multiple_(const string_view_<charT> &str)
        {
            StrViews = new string_view_<charT>[1];
            StrViews[0] = str;
            StrViewsSizeSum = str.StrViewSize;
            StrViewsCount = 1;
        }
        template <typename = typename std::enable_if<std::is_same<charT, char>::value>::type>
        string_view_multiple_(const char *str)
        {
            StrViews = new string_view_<charT>[1];
            StrViews[0] = string_view_<charT>(str);
            StrViewsSizeSum = StrViews[0].StrViewSize;
            StrViewsCount = 1;
        }
        ~string_view_multiple_()
        {
            if (StrViews != nullptr)
            {
                delete[] StrViews;
                StrViews = nullptr;
            }
        }

        charT operator[](size_t index) const
        {
            if (index >= StrViewsSizeSum)
            {
#ifndef bionukg_nothrow
                throw std::out_of_range("operator[] out of range");
#endif
                return charT();
            }
            size_t i = 0;
            while (index >= StrViews[i].StrViewSize)
            {
                index -= StrViews[i].StrViewSize;
                i++;
            }
            return StrViews[i].StrView[index];
        }
        void operator=(const string_view_<charT> &str)
        {
            if (StrViews != nullptr)
            {
                delete[] StrViews;
                StrViews = nullptr;
            }
            StrViews = new string_view_<charT>[1];
            StrViews[0] = str;
            StrViewsSizeSum = str.StrViewSize;
            StrViewsCount = 1;
        }
        void operator+=(const string_view_<charT> &str)
        {
            string_view_<charT> *tmp = new string_view_<charT>[StrViewsCount + 1];
            for (size_t i = 0; i < StrViewsCount; i++)
            {
                tmp[i] = StrViews[i];
            }
            tmp[StrViewsCount] = str;
            StrViewsCount++;
            StrViewsSizeSum += str.StrViewSize;
            if (StrViews != nullptr)
            {
                delete[] StrViews;
                StrViews = nullptr;
            }
            StrViews = tmp;
        }

        // allow std::cout to print string_view_multiple_
        friend std::ostream &operator<<(std::ostream &os, const string_view_multiple_<charT> &str)
        {
            for (size_t i = 0; i < str.StrViewsCount; i++)
            {
                os << str.StrViews[i];
            }
            return os;
        }
    };
    template <typename charT>
    template <typename>
    bool string_<charT>::been_viewed_by(const string_view_<char> &substr)
    {
        if (substr.StrView + substr.StrViewSize > this->Str + this->StrSize ||
            substr.StrView < this->Str)
        {
            return false;
        }
        return true;
    }
    template <typename charT>
    string_view_<charT> string_<charT>::find_substr(const string_view_<charT> &substr, size_t start)
    {
        if (substr.StrViewSize > StrSize)
        {
            return string_view_<charT>();
        }
        for (size_t i = start; i < StrSize - substr.StrViewSize; i++)
        {
            bool is_match = true;
            for (size_t j = 0; j < substr.StrViewSize; j++)
            {
                if (Str[i + j] != substr.StrView[j])
                {
                    is_match = false;
                    break;
                }
            }
            if (is_match)
            {
                return string_view_<charT>(Str + i, substr.StrViewSize);
            }
        }
        return string_view_<charT>();
    }

    template <typename charT>
    string_view_<charT> string_<charT>::findnext_substr(const string_view_<charT> &substr, string_view_<charT> *last_substr)
    {
        if (last_substr == nullptr)
        {
            return find_substr(substr);
        }
        else
        {
            return find_substr(substr, last_substr->StrView + last_substr->StrViewSize - this->Str);
        }
    }

    template <typename charT>
    inline bool string_<charT>::replace_once(const string_view_<charT> &matched_str, const string_view_<charT> &replaced_str, size_t start)
    {
        string_view_<charT> substr = find_substr(matched_str, start);
        if (substr.StrViewSize == 0)
        {
            return false;
        }
        else
        {
            size_t new_size = StrSize - matched_str.StrViewSize + replaced_str.StrViewSize;
            charT *new_str = new charT[new_size];
            size_t i = 0;
            for (; i < substr.StrView - Str; i++)
            {
                new_str[i] = Str[i];
            }
            for (size_t j = 0; j < replaced_str.StrViewSize; j++)
            {
                new_str[i + j] = replaced_str.StrView[j];
            }
            for (size_t j = 0; j < StrSize - (substr.StrView - Str) - substr.StrViewSize; j++)
            {
                new_str[i + replaced_str.StrViewSize + j] = Str[i + substr.StrViewSize + j];
            }
            delete[] Str;
            Str = new_str;
            StrSize = new_size;
            return true;
        }
        return false;
    }
    constexpr uint64_t operator_raw64(const char *str, size_t len)
    {
        uint64_t ret = 0;

        for (size_t i = 0; i < len; i++)
        {
            ret <<= 8;
            ret |= str[i];
        }
        return ret;
    }

    int operator_raw64_reverse(uint64_t raw64, char *outbuf, int bufsize)
    {
        for (int i = 0; i < bufsize; i++)
        {
            char c = raw64 & 0xFF;
            if (c == 0)
            {
                return i;
            }
            outbuf[i] = c;
            raw64 >>= 8;
        }
        return bufsize;
    };

    int operator_raw64_reverse(uint64_t raw64, char (&outbuf)[8])
    {
        return operator_raw64_reverse(raw64, outbuf, 8);
    }

    template <int len>
    constexpr uint64_t operator_raw64(const char (&str)[len])
    {
        return operator_raw64(str, len);
    }

    constexpr uint64_t operator""_raw64(const char *str, size_t len)
    {
        return operator_raw64(str, len);
    }

    constexpr uint64_t operator_base64(const char *str, size_t len)
    {
        uint64_t ret = 0;
        for (size_t i = 0; i < len; i++)
        {
            // rotate shift left 6 bits
            /*uint8_t tmp = ret >> 58;
            ret <<= 6;
            ret |= tmp;*/
            ret = (ret << 6) | (ret >> 58);
            // encode, accept ' ','_','0'-'9','A'-'Z','a'-'z'
            if (str[i] == ' ')
                ret ^= 0;
            else if (str[i] == '_')
                ret ^= 1;
            else if (str[i] >= '0' && str[i] <= '9')
                ret ^= (uint64_t)str[i] - '0' + 2;
            else if (str[i] >= 'A' && str[i] <= 'Z')
                ret ^= (uint64_t)str[i] - 'A' + 12;
            else if (str[i] >= 'a' && str[i] <= 'z')
                ret ^= (uint64_t)str[i] - 'a' + 38;
        };
        // store the length in high 4 bits
        constexpr uint64_t mask60 = (1ui64 << 61) - 1;
        ret &= mask60;
        ret |= (uint64_t)len << 60;
        return ret;
    };

    constexpr uint64_t operator_base64(const char *str)
    {

        for (int len = 0;; len++)
        {
            if (str[len] == 0)
                return operator_base64(str, len);
        }
    };

    int operator_base64_reverse(uint64_t base64, char *outbuf, int bufsize)
    {
        int size = base64 >> 60;
        if (size > bufsize)
        {
            size = bufsize;
        }
        constexpr int maxchar = 10; // 10*6bit<64bit
        if (size > maxchar)
        {
            return -1;
        }
        for (int i = 0; i < size; i++)
        {
            char c = base64 & 0x3F;
            if (c == 0)
            {
                return i;
            }
            if (c == 1)
            {
                outbuf[i] = '_';
            }
            else if (c >= 2 && c <= 11)
            {
                outbuf[i] = c - 2 + '0';
            }
            else if (c >= 12 && c <= 37)
            {
                outbuf[i] = c - 12 + 'A';
            }
            else if (c >= 38 && c <= 63)
            {
                outbuf[i] = c - 38 + 'a';
            }
            base64 >>= 6;
        }
        return size;
    };

    template <int len>
    constexpr uint64_t operator_base64(const char (&str)[len])
    {
        return operator_base64(str, len);
    }
    // Base64 variant encoding
    constexpr uint64_t operator""_b64(const char *str, size_t len)
    {
        return operator_base64(str, len);
    }
    constexpr uint64_t operator""_base64(const char *str, size_t len)
    {
        return operator_base64(str, len);
    }

    class sflie
    {
    public:
        char *buf;
        int offset;
        int len;
        int size;
        sflie(int capa)
        {
            buf = new char[capa]{};
            offset = 0;
            len = capa;
            size = capa;
        }
        sflie(FILE *fp)
        {
            int now = ftell(fp);
            fseek(fp, 0, SEEK_END);
            len = ftell(fp);
            size = len;
            fseek(fp, 0, SEEK_SET);
            buf = new char[size]{};
            offset = 0;
            fread(buf, 1, size, fp);
            fseek(fp, now, SEEK_SET);
        }
        sflie(const sflie &another)
        {
            buf = new char[another.size];
            memcpy(buf, another.buf, another.size);
            offset = another.offset;
            len = another.len;
            size = another.size;
        }

        void move_from(sflie &&another)
        {
            delete[] buf;
            buf = another.buf;
            offset = another.offset;
            len = another.len;
            size = another.size;
            another.buf = 0;
            another.offset = 0;
            another.len = 0;
            another.size = 0;
        }
        void move_from(sflie &another)
        {
            return move_from(std::move(another));
        }
        void clear()
        {
            memset(buf, 0, size);
            offset = 0;
            len = 0;
        }
        void extend(int newsize)
        {
            if (newsize > size)
            {
                char *newbuf = new char[newsize];
                memcpy(newbuf, buf, size);
                delete[] buf;
                buf = newbuf;
                size = newsize;
            }
        }

        char *cur() { return buf + offset; };

        ~sflie()
        {
            if (buf != 0)
                delete[] buf;
        }
        int sfread(void *dst, int len)
        {
            int read = 0;
            if (offset + len > this->len)
                len = this->len - offset;
            memcpy(dst, buf + offset, len);
            return read;
        }
        int sfwrite(const char *src, int len)
        {
            int write = 0;
            extend(offset + len);
            memcpy(buf + offset, src, len);
            offset += len;
            if (offset > this->len)
                this->len = offset;
            return write;
        }
        int sfseek(int offset, int origin)
        {
            switch (origin)
            {
            case SEEK_SET:
                this->offset = offset;
                break;
            case SEEK_CUR:
                this->offset += offset;
                break;
            case SEEK_END:
                this->offset = len + offset;
                break;
            }
            return this->offset;
        };

        bool strcmp_issame(const char *src, int srcl)
        {
            if (srcl == 0)
                return true;
            if (srcl > len)
                return false;
            for (int i = 0; i < srcl; i++)
            {
                if (buf[offset + i] != src[i])
                    return false;
            }
            return true;
        }

        int replace(const char *src, const char *dst)
        {
            int src_len = strlen(src);
            int dst_len = strlen(dst);
            int count = 0;
            for (int i = 0; i < size - src_len; i++)
            {
                sfseek(i, SEEK_SET);
                if (strcmp_issame(src, src_len))
                {
                    count++;
                    i += src_len;
                };
            }

            int newlen = len + (dst_len - src_len) * count;

            sflie dstfile(size);

            for (int i = 0; i < size - src_len;)
            {
                sfseek(i, SEEK_SET);
                if (strcmp_issame(src, src_len))
                {
                    dstfile.sfwrite(dst, dst_len);
                    i += src_len;
                }
                else
                {
                    dstfile.sfwrite(buf + i, 1);
                    i++;
                }
            }
            move_from(dstfile);

            return count;
        };

        typedef bool (*cmp_match_callback)(_In_ const char *data, _In_ int datalen, _Out_ int *matchlen, _Out_ int *dstlen);

        typedef void (*match_wrt_dst_callback)(_In_ const char *data, _In_ int matchlen, char *dst);

        int replace(cmp_match_callback cmp_match, match_wrt_dst_callback match_wrt_dst)
        {
            int count = 0;
            int newlen = this->len;

            for (int i = 0; i < size;)
            {
                sfseek(i, SEEK_SET);
                int matchlen = 0, dstlen = 0;
                if (cmp_match(buf + i, size - i, &matchlen, &dstlen))
                {
                    count++;
                    newlen += dstlen - matchlen;
                    i += matchlen;
                }
                else
                {
                    i++;
                }
            }
            sflie dstfile(newlen);

            for (int i = 0; i < size;)
            {
                sfseek(i, SEEK_SET);
                int matchlen = 0, dstlen = 0;
                if (cmp_match(buf + i, size - i, &matchlen, &dstlen))
                {
                    match_wrt_dst(buf + i, matchlen, dstfile.cur());
                    dstfile.offset += dstlen;
                    i += matchlen;
                }
                else
                {
                    dstfile.sfwrite(buf + i, 1);
                    i++;
                }
            };

            move_from(dstfile);

            return count;
        };

        int fwrite_out(FILE *dst)
        {
            return fwrite(buf, 1, len, dst);
        }
    };

    enum str_err
    {
        str_err_buffer_limit_reached = -10,
        str_err_source_limit_reached,
        str_err_pattern_match_failed,
        str_err_unknown_pattern
    };

    union str_ret_t
    {
        str_err err;
        int value;
        str_ret_t(int val = 0)
        {
            value = val;
        };
    };
    class bit256
    {
        union
        {
            uint8_t data[32] = {};
            uint32_t data32[8];
            uint64_t data64[4];
        };

    public:
        enum inittype
        {
            bit256_all_1,
            bit256_all_0,
            bit256__0_9,
            bit256__A_Z,
            bit256__a_z,
        };

        constexpr bit256(){};
        constexpr bit256(char c)
        {
            uint8_t idx = (uint8_t)c;
            data[idx >> 3] |= (1 << (idx & 0b111));
        };
        constexpr bit256(const char *str, size_t len)
        {
            for (size_t i = 0; i < len; i++)
            {
                set(str[i]);
            }
        };
        constexpr bit256(inittype tp)
        {
            switch (tp)
            {
            case bit256_all_1:
                for (int i = 0; i < 32; i++)
                {
                    data[i] = 0xFF;
                }
                break;
            case bit256_all_0:
                break;
            case bit256__0_9:
                for (char i = '0'; i <= '9'; i++)
                {
                    set(i);
                }
                break;
            case bit256__A_Z:
                for (char i = 'A'; i <= 'Z'; i++)
                {
                    set(i);
                }
                break;
            case bit256__a_z:
                for (char i = 'a'; i <= 'z'; i++)
                {
                    set(i);
                }
                break;
            default:
                break;
            }
        };
        constexpr bit256(const bit256 &another)
        {
            for (int i = 0; i < 32; i++)
            {
                data[i] = another.data[i];
            }
        };
        void operator=(const bit256 &another)
        {
            for (int i = 0; i < 32; i++)
            {
                data[i] = another.data[i];
            }
        };
        void self_logical_not()
        {
            for (int i = 0; i < 32; i++)
            {
                data[i] = ~data[i];
            }
        };
        void operator&=(const bit256 &another)
        {
            for (int i = 0; i < 32; i++)
            {
                data[i] &= another.data[i];
            }
        };
        void operator|=(const bit256 &another)
        {
            for (int i = 0; i < 32; i++)
            {
                data[i] |= another.data[i];
            }
        };
        bit256 operator~() const
        {
            bit256 ret = *this;
            ret.self_logical_not();
            return ret;
        };
        bit256 operator&(const bit256 &another) const
        {
            bit256 ret = *this;
            ret &= another;
            return ret;
        };
        bit256 operator|(const bit256 &another) const
        {
            bit256 ret = *this;
            ret |= another;
            return ret;
        };

        uint16_t one_count() const
        {
            /*uint8_t ret = 0;
            for (int i = 0; i < 4; i++)
            {
                ret += __popcnt64(data64[i]);
            }*/
            // x64
#ifdef _WIN64
            return __popcnt64(data64[0]) + __popcnt64(data64[1]) + __popcnt64(data64[2]) + __popcnt64(data64[3]);
#else
        // x86
        return __popcnt(data32[0]) + __popcnt(data32[1]) + __popcnt(data32[2]) + __popcnt(data32[3]) +
               __popcnt(data32[4]) + __popcnt(data32[5]) + __popcnt(data32[6]) + __popcnt(data32[7]);
#endif
        };

        bool get(uint8_t idx) const
        {
            return (data[idx >> 3] & (1 << (idx & 7))) != 0;
        }
        bool get(char idx) const
        {
            return get((uint8_t)idx);
        }
        void set(uint8_t idx, bool value = true)
        {
            if (value)
                data[idx >> 3] |= (1 << (idx & 0b111));
            else
                data[idx >> 3] &= ~(1 << (idx & 0b111));
        }
        void set(char idx, bool value = true)
        {
            set((uint8_t)idx, value);
        };
    };

    struct range_
    {
        size_t mini;
        size_t maxi;
        constexpr static const size_t rangemax = (~(size_t(0))) >> 1;
        range_(size_t min_ = 1, size_t max_ = 1) : mini(min_), maxi(max_){};
    };

    struct bitfmt
    {
        bit256 bits;
        size_t fmtlen;
        range_ repeat;
        bitfmt(bit256 bits_, size_t fmtlen_, range_ repeat_ = range_())
            : bits(bits_), fmtlen(fmtlen_), repeat(repeat_){};

        //'c' -> single char, range(1,1)
        //'s' -> multiple chars, range(0, rangemax)
        //'r'-> specified range. range(c[1],c[1]+c[2]) (c[1],c[2] as unsigned int)
        // else -> error, clear bits, range(0,0)
        bitfmt(bit256 bits_, size_t fmtlen_, const char *repeat_type, int repeat_type_len = 1) : bits(bits_), fmtlen(fmtlen_)
        {
            switch (repeat_type[0])
            {
            case 'c':
                repeat = range_(1, 1);
                return;
            case 's':
                repeat = range_(0, range_::rangemax);
                return;
            case 'r':
                if (repeat_type_len < 3)
                {
                    dbgerr("bitfmt error: repeat type 'r' need 2 more chars");
                    break;
                }
                repeat = range_((uint8_t)repeat_type[1], (uint16_t)repeat_type[1] + (uint16_t)repeat_type[2]);
                fmtlen += 2;
                return;

            default:
                bits = bit256();
                repeat = range_(0, 0);
                dbgerr("bitfmt error: unknown repeat type");
                break;
            }
        }
    };

    /// <summary>
    ///
    /// </summary>
    /// <param name="fmt_match_rule"></param>
    /// <param name="%c">any single char</param>
    /// <param name="%s">non-negative count of any chars, stop at next possible matched char</param>
    /// <param name="%s%c">undefined behavior</param>
    /// <param name="%dc">a single char of '0'-'9'</param>
    /// <param name="%[ab]">a single char of 'a' or 'b'. end with "]". </param>
    /// <param name="%ds">non-negative count of numberic chars ('0'-'9'), stop at non'0'-'9'</param>
    /// <param name="%%">a char '%'</param>
    /// <returns>the length matched in src. negative if failed.</returns>
    str_ret_t sscanf_chars(const char *const src, size_t srcl, const char *const fmt, size_t fmtlen, char *buf, size_t buflen)
    {
#define lambda_check_return(expression) \
    do                                  \
    {                                   \
        auto r = expression;            \
        if (r < 0)                      \
        {                               \
            ret.err = (str_err)r;       \
            return ret;                 \
        }                               \
    } while (0);
        str_ret_t ret = {};
        size_t srci = 0, fmti = 0, bufi = 0;
        auto lambda_c = [&](char l, char h) -> int
        {
            if (src[srci] >= l && src[srci] <= h)
            {
                buf[bufi++] = src[srci];
                return 0;
            }
            else
            {
                return str_err_pattern_match_failed;
            }
        };
        auto lambda_b256 = [&](const bit256 &bits) -> int
        {
            if (bits.get(src[srci]))
            {
                buf[bufi++] = src[srci];
                return 0;
            }
            else
            {
                return str_err_pattern_match_failed;
            }
        };
        auto lambda_s = [&](char l, char h) -> int
        {
            for (;;)
            {
                if (!(src[srci] >= l && src[srci] <= h))
                {
                    return 0;
                }
                buf[bufi++] = src[srci++];
                if (srci >= srcl)
                    return str_err_source_limit_reached;
                if (bufi >= buflen)
                    return str_err_buffer_limit_reached;
            };
        };

        auto bit256_init = [](const char *const fmt, int fmtlen) -> bitfmt
        {
            switch (fmt[0])
            {
            case '%':
                switch (fmt[1])
                {
                case 'A': // any big alpha char:%Ac
                    if (fmt[2] == 'c')
                        return bitfmt(bit256(bit256::bit256__A_Z), 3);
                    if (fmt[2] == 's')
                        return bitfmt(bit256(bit256::bit256__A_Z), 3, range_(0, range_::rangemax));
                    break;
                case 'a': // any small alpha char:%ac
                    if (fmt[2] == 'c')
                        return bitfmt(bit256(bit256::bit256__a_z), 3);
                    if (fmt[2] == 's')
                        return bitfmt(bit256(bit256::bit256__a_z), 3, range_(0, range_::rangemax));
                    break;
                case 'c': // any char
                    return bitfmt(bit256(bit256::bit256_all_1), 2);
                    break;
                case 'd': // any numberic char:%dc
                    if (fmt[2] == 'c')
                        return bitfmt(bit256(bit256::bit256__0_9), 3);
                    if (fmt[2] == 's')
                        return bitfmt(bit256(bit256::bit256__0_9), 3, range_(0, range_::rangemax));
                    break;
                case 's':
                    return bitfmt(bit256(bit256::bit256_all_1), 2, range_(0, range_::rangemax));
                case '%':
                    return bitfmt(bit256('%'), 2);
                case '[':
                    //%[ab%]c -> any single char of 'a' or 'b'. end with "%]c"
                    //%[ab%]s -> any multiple chars of 'a' or 'b'. end with "%]s"
                    //%[ab%] -> invalid
                    for (size_t len = 0;; len++)
                    {
                        if (fmt[len + 2] == '%' && fmt[len + 3] == ']')
                        {
                            if (fmt[len + 4] == 'c')
                                return bitfmt(bit256(&fmt[2], len), len + 5);
                            else if (fmt[len + 4] == 's')
                                return bitfmt(bit256(&fmt[2], len), len + 5, range_(0, range_::rangemax));
                            else
                            {
                                dbgerr("invalid format string: missing 'c' or 's' after %]");
                                return bitfmt(bit256(), len);
                            }
                        }
                        if (len >= fmtlen)
                        {
                            dbgerr("invalid format string: unmatched end for %[ start");
                            return bitfmt(bit256(), len);
                        }
                    };
                    // switch (fmt[1]) case '['
                default: // switch (fmt[1]) default
                    dbgerr("invalid format string: unknown pattern");
                    return bitfmt(bit256(), 2);
                };
                break; // switch (fmt[0]) case '%'
            default:
                break;
            };
            return bitfmt(bit256(fmt[0]), 1);
        };

        for (;;)
        {

            switch (fmt[fmti])
            {
            case '%':
                switch (fmt[fmti + 1])
                {
                case '%':
                    if (src[srci] != '%')
                    {
                        return {str_err_pattern_match_failed};
                    }
                    break;
                case 'A':
                    //%Ac -> any single char of 'A'-'Z'
                    //%As -> any multiple chars of 'A'-'Z', stop at non'A'-'Z'
                    switch (fmt[fmti + 2])
                    {
                    case 'c':
                        lambda_check_return(lambda_c('A', 'Z'));
                        break;
                    case 's':
                        lambda_check_return(lambda_s('A', 'Z'));
                        break;
                    default:
                        return {str_err_unknown_pattern};
                        break;
                    }
                    fmti++;
                    break;
                case '[':
                    //%[ab] -> any single char of 'a' or 'b'. not support ']'.
                    do
                    {
                        size_t len = 0;
                        while (fmt[fmti + 2 + len] != ']')
                        {
                            len++;
                            if (fmti + 2 + len >= fmtlen)
                                return {str_err_unknown_pattern};
                        }
                        bit256 bits(&fmt[fmti + 2], len);
                        lambda_check_return(lambda_b256(bits));
                        fmti += len + 1; //"ab"->len ,"]"->1
                    } while (0);

                    break;
                case 'a':
                    //%ac -> any single char of 'a'-'z'
                    //%as -> any multiple chars of 'a'-'z', stop at non'a'-'z'
                    switch (fmt[fmti + 2])
                    {
                    case 'c':
                        lambda_check_return(lambda_c('a', 'z'));
                        break;
                    case 's':
                        lambda_check_return(lambda_s('a', 'z'));

                        break;
                    default:
                        return {str_err_unknown_pattern};
                        break;
                    }
                    fmti++;
                    break;

                case 'c':
                    buf[bufi++] = src[srci];
                    break;

                case 'd':
                    //%dc -> any single char of '0'-'9'
                    //%ds -> any multiple chars of '0'-'9', stop at non'0'-'9'
                    switch (fmt[fmti + 2])
                    {
                    case 'c':
                        lambda_check_return(lambda_c('0', '9'));
                        break;
                    case 's':
                        lambda_check_return(lambda_s('0', '9'));
                        break;
                    default:
                        return {str_err_unknown_pattern};
                        break;
                    }
                    fmti++;
                    break;
                case 's':
                    for (;;)
                    {
                        buf[bufi++] = src[srci++];

                        if (src[srci] == fmt[fmti + 2])
                        {
                            srci--;
                            break;
                        }
                        if (srci >= srcl)
                            return {str_err_source_limit_reached};
                        if (bufi >= buflen)
                            return {str_err_buffer_limit_reached};
                    };

                    break;
                default:
                    return {str_err_unknown_pattern};
                    break;
                };
                srci++;
                fmti += 2;
                break;
            default:
                if (fmt[fmti] != src[srci])
                {
                    return {str_err_pattern_match_failed};
                };
                srci++;
                fmti++;
                break;
            };

            if (fmti >= fmtlen)
            {
                ret.value = srci;
                return ret;
            }
            if (srci >= srcl)
                return {str_err_source_limit_reached};
            if (bufi >= buflen)
                return {str_err_buffer_limit_reached};
        };
    };
    /// <summary>
    ///
    /// </summary>
    /// <param name="fmt_match_rule"></param>
    /// <param name="%c">any single char</param>
    /// <param name="%s">non-negative count of any chars, stop at next possible matched char</param>
    /// <param name="perfix">: char type</param>
    /// <param name=".&#x9;&quot;%d&quot;:&#x9;"> char of '0'-'9'</param>
    /// <param name=".&#x9;&quot;%a&quot;:&#x9;"> char of 'a'-'z'</param>
    /// <param name=".&#x9;&quot;%A&quot;:&#x9;"> char of 'A'-'Z'</param>
    /// <param name=".    &quot;%[a%]&quot;:&#x9;"> any char between %[ and %] </param>
    /// <param name=".    &quot;%]a%[&quot;:&#x9;"> any char not between %] and %[ </param>
    /// <param name="suffix">: char length range</param>
    /// <param name=".&#x9;&quot;c&quot;:&#x9;"> range(1,1)</param>
    /// <param name=".&#x9;&quot;s&quot;:&#x9;"> range(0,range_max)</param>
    /// <param name=".   &quot;r\3\5&quot;:&#x9;"> range(3,3+5)</param>
    /// <param name="%%">a char '%'</param>
    /// <returns>the length matched in src. negative if failed.</returns>
    str_ret_t sscanf_chars_v1(const char *const src, size_t srcl, const char *const fmt, size_t fmtlen, char *const buf, size_t buflen)
    {
        auto format_analizer = [](const char *const fmt, int fmtlen) -> bitfmt
        {
            switch (fmt[0])
            {
            case '%':
                switch (fmt[1])
                {
                case 'A': // any big alpha char:%Ac
                    return bitfmt(bit256(bit256::bit256__A_Z), 3, &fmt[2], fmtlen - 2);
                    break;
                case 'a': // any small alpha char:%ac
                    return bitfmt(bit256(bit256::bit256__a_z), 3, &fmt[2], fmtlen - 2);
                    break;
                case 'c': // any char
                    return bitfmt(bit256(bit256::bit256_all_1), 2);
                    break;
                case 'd': // any numberic char:%dc
                    return bitfmt(bit256(bit256::bit256__0_9), 3, &fmt[2], fmtlen - 2);
                    break;
                case 's':
                    return bitfmt(bit256(bit256::bit256_all_1), 2, range_(0, range_::rangemax));
                case '%':
                    return bitfmt(bit256('%'), 2);
                case '[':
                    //%[ab%]c -> any single char of 'a' or 'b'. end with "%]c"
                    //%[ab%]s -> any multiple chars of 'a' or 'b'. end with "%]s"
                    //%[ab%] -> invalid
                    for (size_t len = 0;; len++)
                    {
                        if (fmt[len + 2] == '%' && fmt[len + 3] == ']')
                        {
                            return bitfmt(bit256(&fmt[2], len), len + 5, &fmt[len + 4], fmtlen - len - 4);
                        }
                        if (len >= fmtlen)
                        {
                            dbgerr("invalid format string: unmatched end for %[ start");
                            return bitfmt(bit256(), len);
                        }
                    };
                case ']':
                    //%[ab%]c -> any single char of 'a' or 'b'. end with "%]c"
                    //%[ab%]s -> any multiple chars of 'a' or 'b'. end with "%]s"
                    //%[ab%] -> invalid
                    for (size_t len = 0;; len++)
                    {
                        if (fmt[len + 2] == '%' && fmt[len + 3] == '[')
                        {
                            return bitfmt(~bit256(&fmt[2], len), len + 5, &fmt[len + 4], fmtlen - len - 4);
                        }
                        if (len >= fmtlen)
                        {
                            dbgerr("invalid format string: unmatched end for %[ start");
                            return bitfmt(bit256(), len);
                        }
                    };
                    // switch (fmt[1]) case '['
                default: // switch (fmt[1]) default
                    dbgerr("invalid format string: unknown pattern");
                    return bitfmt(bit256(), 2);
                };
                break; // switch (fmt[0]) case '%'
            default:
                break;
            };
            return bitfmt(bit256(fmt[0]), 1);
        };
        size_t srci = 0, fmti = 0, bufi = 0;
        while (true)
        {
            // 1. get the format of expression
            auto pack = format_analizer(&fmt[fmti], fmtlen - fmti);
            auto onecount = pack.bits.one_count();

            auto nextpack = (onecount == 256 && fmti + pack.fmtlen < fmtlen) ? format_analizer(&fmt[fmti + pack.fmtlen], fmtlen - fmti - pack.fmtlen) : bitfmt(bit256(), 0);
            // 2. match the expression
            for (size_t i = 0;;)
            {
                if (srci + i >= srcl)
                { // check if reached min limit
                    if (i >= pack.repeat.mini)
                    {
                        srci += i > 0 ? i - 1 : 0;
                        break;
                    }
                    else
                    {
                        return {str_err_source_limit_reached};
                    }
                };

                // expect to match
                // pack.repeat.min~ pack.repeat.max
                // of chars
                if (onecount == 256 && pack.repeat.maxi==pack.repeat.rangemax&&// matched any char, use next for stop
                    nextpack.bits.get(src[srci + i]) && // next match success, may stop
                    i >= pack.repeat.mini)               // if match enough chars
                {
                    /*buf[bufi++] = src[srci + i];
                    i++;*/
                    goto match_stop;
                };
                if (pack.bits.get(src[srci + i])) // match success
                {
                    if (bufi >= buflen)
                        return {str_err_buffer_limit_reached};
                    if (onecount > 1) // record the uncertain char
                    {
                        buf[bufi++] = src[srci + i];
                    }
                    
                    i++;
                }
                else // match failed, stop
                {
                    goto match_stop;
                }

                

                if (i >= pack.repeat.maxi)
                {
                    srci += i;
                    break;
                }
                continue;
                //
            match_stop:
                if (onecount == 0) // match failed, stop
                {
                    return {str_err_pattern_match_failed};
                }

                if (i < pack.repeat.mini) // fail to match enough chars
                {
                    return {str_err_pattern_match_failed};
                }
                else // match enough chars, break
                {
                    srci += i;
                    break;
                }
            }

            // 3. get next expression
            fmti += pack.fmtlen;
            if (fmti >= fmtlen)
            {
                return str_ret_t(srci);
            }
        };
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="fmt_match_rule"></param>
    /// <param name="%c">any single char</param>
    /// <param name="%s">non-negative count of any chars, stop at next possible matched char</param>
    /// <param name="%dc">a single char of '0'-'9'</param>
    /// <param name="%ds">non-negative count of numberic chars ('0'-'9'), stop at non'0'-'9'</param>
    /// <param name="%%">a char '%'</param>
    /// <returns>the length matched in src. negative if failed.</returns>
    template <size_t fmtlen, size_t buflen>
    auto sscanf_chars(const char *src, size_t srcl, const char (&fmt)[fmtlen], char (&buf)[buflen])
    {
        return sscanf_chars(src, srcl, fmt, fmtlen - 1, buf, buflen);
    }

    template <size_t srclen, size_t fmtlen, size_t buflen>
    auto sscanf_chars_v1(const char (&src)[srclen], const char (&fmt)[fmtlen], char (&buf)[buflen])
    {
        return sscanf_chars_v1(src, srclen - 1, fmt, fmtlen - 1, buf, buflen);
    }

#ifdef namespace_bionukg
};
#endif
#endif /* bionukg_string_h */
