/*
Copyright (C) 2006 - 2011 Evan Teran
                          eteran@alum.rit.edu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ByteShiftArray.h"

//------------------------------------------------------------------------------
// Name: ByteShiftArray(std::size_t size)
// Desc: constructor
//------------------------------------------------------------------------------
ByteShiftArray::ByteShiftArray(std::size_t size) : data_(size) {
}

//------------------------------------------------------------------------------
// Name: ByteShiftArray(const ByteShiftArray& other)
// Desc: copy constructor
//------------------------------------------------------------------------------
ByteShiftArray::ByteShiftArray(const ByteShiftArray& other) : data_(other.data_) {
}

//------------------------------------------------------------------------------
// Name: operator=(const ByteShiftArray &other
// Desc: assignment operator
//------------------------------------------------------------------------------
ByteShiftArray &ByteShiftArray::operator=(const ByteShiftArray &other) {
	if(&other != this) {
		ByteShiftArray(other).swap(*this);
	}
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap(ByteShiftArray &other)
// Desc:
//------------------------------------------------------------------------------
void ByteShiftArray::swap(ByteShiftArray &other) {
	qSwap(data_, other.data_);
}

//------------------------------------------------------------------------------
// Name: shl()
// Desc: shifts data left one byte and shifts in a 0
//------------------------------------------------------------------------------
ByteShiftArray &ByteShiftArray::shl() {
	for(int i = 1; i < data_.size(); ++i) {
		data_[i - 1] = data_[i];
	}
	data_.back() = 0;
	return *this;
}

//------------------------------------------------------------------------------
// Name: shr()
// Desc: shifts data right one byte and shifts in a 0
//------------------------------------------------------------------------------
ByteShiftArray &ByteShiftArray::shr() {
	for(int i = 0; i < data_.size() - 1; ++i) {
		data_[i + 1] = data_[i];
	}
	data_.first() = 0;
	return *this;
}

//------------------------------------------------------------------------------
// Name: size() const
// Desc: returns size of this byte array
//------------------------------------------------------------------------------
std::size_t ByteShiftArray::size() const {
	return data_.size();
}

//------------------------------------------------------------------------------
// Name: operator[](std::size_t i)
// Desc: returns and l-value version of an element in the byte array
//------------------------------------------------------------------------------
quint8 &ByteShiftArray::operator[](std::size_t i) {
	return data_[i];
}

//------------------------------------------------------------------------------
// Name: operator[](std::size_t i)
// Desc: returns and r-value version of an element in the byte array
//------------------------------------------------------------------------------
quint8 ByteShiftArray::operator[](std::size_t i) const {
	return data_[i];
}

//------------------------------------------------------------------------------
// Name: data() const
// Desc: returns a read only pointer to the data this byte array holds
//------------------------------------------------------------------------------
const quint8 *ByteShiftArray::data() const {
	return &data_[0];
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc: zeros out the byte array
//------------------------------------------------------------------------------
void ByteShiftArray::clear() {
	data_.fill(0);
}

//------------------------------------------------------------------------------
// Name: operator<<(quint8 x)
// Desc:
//------------------------------------------------------------------------------
ByteShiftArray &ByteShiftArray::operator<<(quint8 x) {
	shl();
	data_.back() = x;
	return *this;
}
