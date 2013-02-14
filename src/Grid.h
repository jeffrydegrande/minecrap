#ifndef MINECRAP_GRID_H
#define MINECRAP_GRID_H

#include <vector>

template<class T>
class Grid
{
public:
	Grid(size_t numRows, size_t numColumns);

	Grid(size_t numRows, size_t numColumns, const T & inInitialValue);

	const T & get(size_t row, size_t col) const;

	T & get(size_t row, size_t col);

	void set(size_t row, size_t col, const T & inT);

	size_t numRows() const;

	size_t numColumns() const;

	size_t actualSize() const;

private:
	size_t mNumRows;
	size_t mNumColumns;
	std::vector<T> mData;
};


#include "Grid.h"


template<class T>
Grid<T>::Grid(size_t numRows, size_t numColumns):
	mNumRows(numRows),
	mNumColumns(numColumns)
{
	mData.resize(numRows*numColumns);
}

template<class T>
Grid<T>::Grid(size_t numRows, size_t numColumns, const T & inInitialValue):
	mNumRows(numRows),
	mNumColumns(numColumns)
{
	mData.resize(numRows*numColumns, inInitialValue);
}


template<class T>
const T & Grid<T>::get(size_t rowIdx, size_t colIdx) const
{
	return mData[rowIdx*mNumColumns + colIdx];
}


template<class T>
T & Grid<T>::get(size_t rowIdx, size_t colIdx)
{
	return mData[rowIdx*mNumColumns + colIdx];
}


template<class T>
void Grid<T>::set(size_t rowIdx, size_t colIdx, const T & inT)
{
	mData[rowIdx * mNumColumns + colIdx] = inT;
}


template<class T>
size_t Grid<T>::numRows() const
{
	return mNumRows;
}


template<class T>
size_t Grid<T>::numColumns() const
{
	return mNumColumns;
}

template <class T>
size_t Grid<T>::actualSize() const
{
	return mData.size();
}

#endif