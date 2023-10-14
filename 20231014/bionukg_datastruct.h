// 建议使用utf-8
#pragma once

#ifndef bionukg_datastruct_h
#define bionukg_datastruct_h
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

// in msvc
#ifdef _MSC_VER
#include <intrin.h>
// in clang
#elif defined(__clang__)
#include <x86intrin.h>
// in gcc
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif
#include <iostream>

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
		dataType &operator[](size_t idx)
		{
			static dataType null_data = dataType();
			if (idx < this->length)
			{
				return this->list[idx];
			}
			else
			{
				null_data = dataType();
				return null_data;
			}
		}
		const dataType &operator[](size_t idx) const
		{
			static dataType null_data = dataType();
			if (idx < this->length)
			{
				return this->list[idx];
			}
			else
			{
				null_data = dataType();
				return null_data;
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
				delete[] this->list;
			this->list = 0;
		}
		void resize(size_t size)
		{
			dataType *ndata;

			if (this->length > size)
			{
				this->length = size;
			}

			ndata = new dataType[size]{};
			if (ndata != NULL)
			{
				if (list != NULL)
				{
					for (size_t i = 0; i < this->length; i++)
					{
						ndata[i] = this->list[i];
					}
					delete[] this->list;
				}
				this->list = ndata;
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
		dataType &x() { return vec[0]; };
		dataType &y()
		{
			static_assert(N >= 2, "N>=2");
			return vec[1];
		};
		dataType &z()
		{
			static_assert(N >= 3, "N>=3");
			return vec[2];
		};
		dataType &w()
		{
			static_assert(N >= 4, "N>=4");
			return vec[3];
		};

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

	using vector2d = vector_<2, double>;
	using vector3d = vector_<3, double>;



	template<class T, class arg>
	class lazy_sington_nonvoid
	{
		T* data;

		template<class argp=arg>
		auto argpack(const argp* a=nullptr)
		{
			static auto ret = *a;
			return ret;
		}

		public:
		lazy_sington_nonvoid<T,arg>(arg a,T*) :data(nullptr)
		{
			argpack(&a);
		}



		lazy_sington_nonvoid() = delete;
		~lazy_sington_nonvoid()
		{
			if (data != nullptr)
			{
				delete data;
			}
		}

		template<class argp=arg>
		T* operator->();

		template<>
		T* operator-><void>()
		{
			if (data == nullptr)
			{
				data = new T();
			}
			return data;
		}
	};

	template<class T>
	class lazy_sington_void
	{
		T* data;

		public:
			lazy_sington_void(T*) :data(nullptr)
			{		};

		lazy_sington_void() = delete;
		~lazy_sington_void()
		{
			if (data != nullptr)
			{
				delete data;
			}
		}

		T* operator->()
		{
			if (data == nullptr)
			{
				data = new T();
			}
			return data;
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

	class FILE_
	{
	public:
		int read(void *buf, size_t size_in_byte);
		int write(const void *buf, size_t size_in_byte);
	};

	class FILE_sys : public FILE_
	{
		FILE *fp;

	public:
		FILE_sys(const char *filename, const char *mode)
		{
#ifdef _MSC_VER
			fopen_s(&fp, filename, mode);
#else
		fp = fopen(filename, mode);
#endif
		}
		~FILE_sys()
		{
			fclose(fp);
		}
		int read(void *buf, int size_in_byte)
		{
			return (int)fread(buf, 1, size_in_byte, fp);
		}
		int write(const void *buf, int size_in_byte)
		{
			return (int)fwrite(buf, 1, size_in_byte, fp);
		}
		int seek(long offset, int origin)
		{
			return fseek(fp, offset, origin);
		}
		long tell()
		{
			return ftell(fp);
		}
	};

	class FILE_memview : public FILE_
	{
	public:
		void *buf;
		size_t size;
		size_t pos;
		FILE_memview(void *buf, size_t size)
		{
			this->buf = buf;
			this->size = size;
			this->pos = 0;
		}
		int read(void *buf, int size_in_byte)
		{
			if (pos + size_in_byte > size)
			{
				size_in_byte = int(size - pos);
			}
			memcpy(buf, (char *)this->buf + pos, size_in_byte);
			pos += size_in_byte;
			return size_in_byte;
		}
		int write(const void *buf, int size_in_byte)
		{
			if (pos + size_in_byte > size)
			{
				size_in_byte = int(size - pos);
			}
			memcpy((char *)this->buf + pos, buf, size_in_byte);
			pos += size_in_byte;
			return size_in_byte;
		}
		int seek(long offset, int origin)
		{
			switch (origin)
			{
			case SEEK_SET:
				pos = offset;
				break;
			case SEEK_CUR:
				pos += offset;
				break;
			case SEEK_END:
				pos = size + offset;
				break;
			default:
				return -1;
			}
			return 0;
		}
		size_t tell()
		{
			return pos;
		}
	};

	class FILE_mem : public FILE_
	{
		void *buf;
		size_t size;
		size_t pos;

	public:
		FILE_mem(size_t size)
		{
			this->buf = malloc(size);
			this->size = size;
			this->pos = 0;
		}
		FILE_mem(const void *copymem, size_t size)
		{
			this->buf = malloc(size);
			if (this->buf == NULL)
			{
				this->size = 0;
				this->pos = 0;
				return;
			}
			memcpy(this->buf, copymem, size);
			this->size = size;
			this->pos = 0;
		}
		~FILE_mem()
		{
			free(buf);
		}
		int read(void *buf, int size_in_byte)
		{
			if (pos + size_in_byte > size)
			{
				size_in_byte = int(size - pos);
			}
			memcpy(buf, (char *)this->buf + pos, size_in_byte);
			pos += size_in_byte;
			return size_in_byte;
		}
		int write(const void *buf, int size_in_byte)
		{
			if (pos + size_in_byte > size)
			{
				size_in_byte = int(size - pos);
			}
			memcpy((char *)this->buf + pos, buf, size_in_byte);
			pos += size_in_byte;
			return size_in_byte;
		}
		int seek(long offset, int origin)
		{
			switch (origin)
			{
			case SEEK_SET:
				pos = offset;
				break;
			case SEEK_CUR:
				pos += offset;
				break;
			case SEEK_END:
				pos = size + offset;
				break;
			default:
				return -1;
			}
			return 0;
		}
		size_t tell()
		{
			return pos;
		}
	};

// if winsock2.h is included
#ifdef _WINSOCK2API_
	class FILE_socket : public FILE_
	{
	public:
		SOCKET sock;
		FILE_socket(SOCKET sock)
		{
			this->sock = sock;
		}
		int read(void *buf, size_t size_in_byte)
		{
			return recv(sock, (char *)buf, size_in_byte, 0);
		}
		int write(const void *buf, size_t size_in_byte)
		{
			return send(sock, (char *)buf, size_in_byte, 0);
		}
		int seek(long offset, int origin)
		{
			return -1;
		}
		long tell()
		{
			return -1;
		}
	};
#endif


// pipe
#ifdef _NAMEDPIPE_H_
	class FILE_pipeRead : public FILE_
	{
		HANDLE hPipe;

	public:
		FILE_pipeRead(HANDLE hPipe)
		{
			this->hPipe = hPipe;
		}
		FILE_pipeRead()
		{
			this->hPipe = INVALID_HANDLE_VALUE;
		}
		HANDLE *Pipe_()
		{
			return &hPipe;
		}
		int read(void *buf, int size_in_byte)
		{
			DWORD dwRead;
			BOOL bSuccess = ReadFile(
				hPipe,		  // handle to pipe
				buf,		  // buffer to receive data
				size_in_byte, // size of buffer
				&dwRead,	  // number of bytes read
				NULL);		  // not overlapped I/O

			if (!bSuccess || dwRead == 0)
			{
				return -1;
			}
			return dwRead;
		}
		int write(const void *buf, size_t size_in_byte)
		{
			return -1;
		}
	};

	class FILE_pipeWrite : public FILE_
	{
		HANDLE hPipe;

	public:
		FILE_pipeWrite(HANDLE hPipe)
		{
			this->hPipe = hPipe;
		}
		FILE_pipeWrite()
		{
			this->hPipe = INVALID_HANDLE_VALUE;
		}
		HANDLE *Pipe_()
		{
			return &hPipe;
		}
		int read(void *buf, size_t size_in_byte)
		{
			return -1;
		}
		int write(const void *buf, DWORD size_in_byte)
		{
			DWORD dwWritten;
			BOOL bSuccess = WriteFile(
				hPipe,		 // handle to pipe
				buf,		 // buffer to write from
				size_in_byte, // number of bytes to write
				&dwWritten,	 // number of bytes written
				NULL);		 // not overlapped I/O

			if (!bSuccess || dwWritten == 0)
			{
				return -1;
			}
			return dwWritten;
		}
	};

	class FILE_pipePair
	{
		FILE_pipeRead ReadPipe;
		FILE_pipeWrite WritePipe;

	public:
		FILE_pipePair()
		{
			HANDLE hReadPipe, hWritePipe;
			BOOL bSuccess = CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);
			if (!bSuccess)
			{
				hReadPipe = INVALID_HANDLE_VALUE;
				hWritePipe = INVALID_HANDLE_VALUE;
			}
			ReadPipe = FILE_pipeRead(hReadPipe);
			WritePipe = FILE_pipeWrite(hWritePipe);
		}
		FILE_pipeRead *Reader()
		{
			return &ReadPipe;
		}
		FILE_pipeWrite *Writer()
		{
			return &WritePipe;
		}

		~FILE_pipePair()
		{
			if (ReadPipe.Pipe_() != INVALID_HANDLE_VALUE)
			{
				CloseHandle(*ReadPipe.Pipe_());
			}
			if (WritePipe.Pipe_() != INVALID_HANDLE_VALUE)
			{
				CloseHandle(*WritePipe.Pipe_());
			}
		}
	};
	
	//unistd.h
#elif defined(_UNISTD_H)
	class FILE_pipeRead : public FILE_
	{
		int fd;

	public:
		FILE_pipeRead(int fd)
		{
			this->fd = fd;
		}
		FILE_pipeRead()
		{
			this->fd = -1;
		}
		int *Pipe_()
		{
			return &fd;
		}
		int read(void *buf, size_t size_in_byte)
		{
			return ::read(fd, buf, size_in_byte);
		}
		int write(const void *buf, size_t size_in_byte)
		{
			return -1;
		}
	};
	
	class FILE_pipeWrite : public FILE_
	{
		int fd;

	public:
		FILE_pipeWrite(int fd)
		{
			this->fd = fd;
		}
		FILE_pipeWrite()
		{
			this->fd = -1;
		}
		int *Pipe_()
		{
			return &fd;
		}
		int read(void *buf, size_t size_in_byte)
		{
			return -1;
		}
		int write(const void *buf, size_t size_in_byte)
		{
			return ::write(fd, buf, size_in_byte);
		}
	};

	class FILE_pipePair
	{
		FILE_pipeRead ReadPipe;
		FILE_pipeWrite WritePipe;

	public:
		FILE_pipePair()
		{
			int fd[2];
			int bSuccess = pipe(fd);
			if (bSuccess != 0)
			{
				fd[0] = -1;
				fd[1] = -1;
			}
			ReadPipe = FILE_pipeRead(fd[0]);
			WritePipe = FILE_pipeWrite(fd[1]);
		}
		FILE_pipeRead *Reader()
		{
			return &ReadPipe;
		}
		FILE_pipeWrite *Writer()
		{
			return &WritePipe;
		}

		~FILE_pipePair()
		{
			if (ReadPipe.Pipe_() != -1)
			{
				close(*ReadPipe.Pipe_());
			}
			if (WritePipe.Pipe_() != -1)
			{
				close(*WritePipe.Pipe_());
			}
		}
	};

#endif


#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // !bionukg_datastruct_h
