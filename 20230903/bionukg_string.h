// bionukg's string class

#include <iostream>
#include <vector>
#ifndef bionukg_string_h
#define bionukg_string_h

#ifdef namespace_bionukg
namespace bionukg
{
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

    template <typename charT = char>
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

    template <typename charT = char>
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

    template <typename charT = char>
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

    constexpr uint64_t operator""_raw64(const char *str, size_t len)
    {
        uint64_t ret = 0;

        for (size_t i = 0; i < len; i++)
        {
            ret <<= 8;
            ret |= str[i];
        }
        return ret;
    }

    // Base64 variant encoding
    constexpr uint64_t operator""_b64(const char *str, size_t len)
    {
        uint64_t ret = 0;
        for (size_t i = 0; i < len; i++)
        {
            // rotate shift left 6 bits
            uint8_t tmp = ret >> 58;
            ret <<= 6;
            ret |= tmp;
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
        }
        // store the length in high 4 bits
        ret &= 0x0fffffffffffffff;
        ret |= (uint64_t)len << 60;
        return ret;
    }

    string_<char> operator""_asm(const char *str, size_t len)
    {
        string_<char> ret;
        ret.StrSize = len;
        ret.Str = new char[len];
        for (size_t i = 0; i < len; i++)
        {
            ret.Str[i] = str[i];
        }
        return ret;
    }

#ifdef namespace_bionukg
};
#endif
#endif /* bionukg_string_h */
