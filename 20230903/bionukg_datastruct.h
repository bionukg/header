// 建议使用utf-8
#pragma once

#ifndef bionukg_datastruct_h
#define bionukg_datastruct_h
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <intrin.h>

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	template <typename dataType>
	class list_
	{
	public:
		size_t capacity = 0;
		size_t length = 0;
		dataType *list = 0;

	public:
		// size_t get_capacity()const { return this->capacity };
		const size_t &get_length() const { return this->length; };
		const dataType &operator[](size_t idx) const
		{
			if (idx < this->length)
			{
				return this->list[idx];
			}
			else
			{
				return dataType();
			}
		}

		list_(){};
		list_(const list_ &that)
		{
			*this = that;
		}
		void operator=(const list_ &that)
		{
			this->resize(that.capacity);
#ifdef _DEBUG
			if (this->list == 0)
				return;
#endif
			this->capacity = that.capacity;
			this->length = that.length;
			for (size_t i = 0; i < length; i++)
			{
				this->list[i] = that.list[i];
			}
		}
		list_(size_t size)
		{
			this->resize(size);
		}
		~list_()
		{
			if (this->list != 0)
				free(this->list);
			this->list = 0;
		}
		void resize(size_t size)
		{
			void *ndata;

			if (this->length > size)
			{
				this->length = size;
			}

			ndata = malloc(size * sizeof(dataType));
			if (ndata != NULL)
			{
				memset(ndata, 0, size * sizeof(dataType));
				if (list != NULL)
				{
					memcpy(ndata, list, this->length * sizeof(dataType));
				}
				this->list = (dataType *)ndata;
				this->capacity = size;
			}
			else
			{
				system("pause");
			}
			return;
		};
		void add(const dataType &adata)
		{
			if (this->length + 4 > this->capacity)
				resize(this->capacity + 4);
			this->list[length] = adata;
			this->length++;
		}
		void add(const list_ &that)
		{
			if (this->length + that.length > this->capacity)
			{
				resize(this->length + that.length + 4);
			}
			for (size_t i = length; i < length + that.length; i++)
			{
				this->list[i] = that.list[i];
			}
			this->length += that.length;
			return;
		}
		bool remove(const dataType &adata)
		{
			bool found = false;
			for (size_t i = 0; i < length; i++)
			{
				if (found)
				{
					this->list[i - 1] = this->list[i];
				}
				else
				{
					if (this->list[i] == adata)
						found = true;
				}
			}
			if (found)
			{
				this->length--;
			}
			return found;
		}
		bool remove(size_t idx)
		{
			if (idx >= this->length)
				return false;
			for (size_t i = idx; i < this->length; i++)
			{
				this->list[i - 1] = this->list[i];
			}
			this->length--;
			return true;
		}
		void imm__clear()
		{
			if (this->list != 0)
				free(this->list);
			this->list = 0;
			this->capacity = 0;
			this->length = 0;
		};
	};

	template <int N, typename dataType>
	class vector_
	{
	public:
		dataType vec[N] = {0};
		void operator+=(const vector_ &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] += that.vec[var];
			}
		}
		void operator-=(const vector_ &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] -= that.vec[var];
			}
		}
		void operator*=(const vector_ &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] *= that.vec[var];
			}
		}
		void operator*=(const dataType &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] *= that;
			}
		}
		void operator/=(const vector_ &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] /= that.vec[var];
			}
		}
		void operator/=(const dataType &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] /= that;
			}
		}
		void operator=(const vector_ &that)
		{
			for (size_t var = 0; var < N; var++)
			{
				this->vec[var] = that.vec[var];
			}
		}
		vector_ operator+(const vector_ &that) const
		{
			vector_ ret = *this;
			ret += that;
			return ret;
		};
		vector_ operator-(const vector_ &that) const
		{
			vector_ ret = *this;
			ret -= that;
			return ret;
		};
		vector_ operator-(void) const
		{
			vector_ ret;
			ret -= *this;
			return ret;
		};
		vector_ operator*(const vector_ &that) const
		{
			vector_ ret = *this;
			ret *= that;
			return ret;
		};
		vector_ operator*(const dataType &that) const
		{
			vector_ ret = *this;
			ret *= that;
			return ret;
		};
		vector_ operator/(const vector_ &that) const
		{
			vector_ ret = *this;
			ret /= that;
			return ret;
		};
		vector_ operator/(const dataType &that) const
		{
			vector_ ret = *this;
			ret /= that;
			return ret;
		};
		vector_<N, bool> operator>(const vector_<N, dataType> &that) const
		{
			vector_<N, bool> ret;
			for (size_t i = 0; i < N; i++)
			{
				ret.vec[i] = this->vec[i] > that.vec[i] ? true : false;
			}
			return ret;
		}
		vector_<N, bool> operator<(const vector_<N, dataType> &that) const
		{
			vector_<N, bool> ret;
			for (size_t i = 0; i < N; i++)
			{
				ret.vec[i] = this->vec[i] < that.vec[i] ? true : false;
			}
			return ret;
		}
		vector_<N, bool> operator>=(const vector_<N, dataType> &that) const
		{
			vector_<N, bool> ret;
			for (size_t i = 0; i < N; i++)
			{
				ret.vec[i] = this->vec[i] >= that.vec[i] ? true : false;
			}
			return ret;
		}
		vector_<N, bool> operator<=(const vector_<N, dataType> &that) const
		{
			vector_<N, bool> ret;
			for (size_t i = 0; i < N; i++)
			{
				ret.vec[i] = this->vec[i] <= that.vec[i] ? true : false;
			}
			return ret;
		}
		bool is_all_finite() const
		{
			for (size_t i = 0; i < N; i++)
			{
				if (!isfinite(vec[i]))
				{
					return false;
				}
			}
			return true;
		}
		// vector_<N, bool> compare(const vector_<N, dataType>& that, bool comp(const dataType& a, const dataType& b))const
		//{
		//	vector_<N, bool> ret;
		//	for (size_t i = 0; i < N; i++)
		//	{
		//		ret.vec[i] = comp(this->vec[i] , that.vec[i])
		//	}
		//	return ret;
		// }
		dataType modulus() const
		{
			dataType ret = 0;
			for (size_t i = 0; i < N; i++)
			{
				ret += vec[i] * vec[i];
			}
			return sqrt(ret);
		}
		dataType distance(const vector_ &that) const
		{
			dataType ret = 0;
			for (size_t i = 0; i < N; i++)
			{
				ret += (vec[i] - that.vec[i]) * (vec[i] - that.vec[i]);
			}
			return sqrt(ret);
		}
		dataType &operator[](size_t size)
		{
			return vec[size];
		}
		// void print()
		//{
		//	std::cout << "(";
		//	for (int i = 0; i < N; i++) { //
		//		printf("%4.4e", this->vec[i]); //
		//		if (i < N - 1) { //
		//			std::cout << ", ";
		//		}
		//	}
		//	std::cout << ")"; //
		// }
		vector_() {}
		vector_(dataType x, dataType y)
		{
			static_assert(N == 2, "N==2");
			vec[0] = x;
			vec[1] = y;
		}
		vector_(dataType x, dataType y, dataType z)
		{
			static_assert(N == 3, "N==3");
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
		}
	};

	template <int N>
	class vector_<N, size_t>
	{
	public:
		size_t vec[N];

		vector_() = default;

		void vector_constuctor_(const vector_<N - 1, size_t> &that, size_t end_element)
		{
			vec[N - 1] = end_element;
			for (size_t i = 0; i < N - 1; i++)
			{
				vec[i] = that.vec[i];
			}
		}
		void vector_constuctor_(size_t start_element, const vector_<N - 1, size_t> &that)
		{
			vec[0] = start_element;
			for (size_t i = 0; i < N - 1; i++)
			{
				vec[i + 1] = that.vec[i];
			}
		}
		vector_(const vector_<N - 1, size_t> &that, size_t x)
		{
			this->vector_constuctor_(that, x);
		};

		vector_(size_t x, const vector_<N - 1, size_t> &that)
		{
			this->vector_constuctor_(x, that);
		}

		template <typename... ts>
		vector_(size_t arg, ts... args)
		{
			static_assert(sizeof...(args) == N - 1, "sizeof...(args) == N - 1");

			// use constructor
			this->vector_constuctor_(arg, vector_<N - 1, size_t>(args...));
		};

		vector_<N + 1, size_t> operator,(size_t x)
		{
			return vector_<N + 1, size_t>(*this, x);
		};
	};
	template <>
	class vector_<1, size_t>
	{
	public:
		size_t vec[1];

		vector_() = default;
		vector_(size_t x)
		{
			vec[0] = x;
		}
		vector_<2, size_t> operator,(size_t x)
		{
			return vector_<2, size_t>(*this, x);
		}
	};

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // !bionukg_datastruct_h
