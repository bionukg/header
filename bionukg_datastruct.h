#pragma once

#ifndef bionukg_datastruct_h
#define bionukg_datastruct_h
#include<stdlib.h>
#include<stdio.h>

template<typename dataType>
class list_
{
public:
	dataType* list = 0;
	size_t capacity = 0;
	size_t length = 0;
	list_() {};
	list_(const list_& that)
	{
		list= (dataType*)malloc(that.capacity * sizeof(dataType));
		capacity = that.capacity;
		for (size_t i = 0; i < length; i++)
		{
			list[i] = that.list[i];
		}
	}
	list_(size_t size)
	{
		list = (dataType*)malloc(size * sizeof(dataType));
		capacity = size;
	}
	~list_()
	{
		if (list != 0)
			free(list);
	}
	void resize(size_t size)
	{
		void* ndata = realloc(list, size * sizeof(dataType));
		if (ndata != NULL)
		{
			list = (dataType*)ndata;
			capacity = size;
		}
		return;
	};
	void add(const dataType& adata)
	{
		if (length + 1 < capacity)
			resize(length + 1);
		list[length] = adata;
		length++;
	}
	void add(const list_& that)
	{
		if (length + that.length < capacity)
		{
			resize(length + that.length);
		}
		for (size_t i = length; i < length + that.length; i++)
		{
			list[i] = that.list[i];
		}
		length += that.length;
		return;
	}
private:

};

#endif // !bionukg_datastruct_h



