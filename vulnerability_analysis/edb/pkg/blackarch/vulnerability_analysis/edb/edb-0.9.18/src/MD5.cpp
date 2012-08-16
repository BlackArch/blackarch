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

#include "MD5.h"

#include <algorithm>
#include <climits>
#include <cstring>

namespace {

//------------------------------------------------------------------------------
// Name: F(quint32 x, quint32 y, quint32 z)
// Desc:
//------------------------------------------------------------------------------
quint32 F(quint32 x, quint32 y, quint32 z) {
	return (x & y) | (~x & z);
}

//------------------------------------------------------------------------------
// Name: G(quint32 x, quint32 y, quint32 z)
// Desc:
//------------------------------------------------------------------------------
quint32 G(quint32 x, quint32 y, quint32 z) {
	return (z & x) | (~z & y);
}

//------------------------------------------------------------------------------
// Name: H(quint32 x, quint32 y, quint32 z)
// Desc:
//------------------------------------------------------------------------------
quint32 H(quint32 x, quint32 y, quint32 z) {
	return x ^ y ^ z;
}

//------------------------------------------------------------------------------
// Name: I(quint32 x, quint32 y, quint32 z)
// Desc:
//------------------------------------------------------------------------------
quint32 I(quint32 x, quint32 y, quint32 z) {
	return y ^ (x | ~z);
}

template <typename T>
static T circular_shift(T v, int n) {
	static const std::size_t bits(CHAR_BIT * sizeof(T));
	const T mask(~(T(-1) << n));
	return (v << n) | ((v >> (bits - n)) & mask);
}

template <quint32 (*Fun)(quint32, quint32, quint32)>
static void do_transform(quint32& a, quint32 b, quint32 c, quint32 d, quint32 x, quint32 s, quint32 ac) {
	a += Fun(b, c, d) + x + ac;
	a = circular_shift(a, s) + b;
}

}

//------------------------------------------------------------------------------
// Name: MD5()
// Desc: constructor
//------------------------------------------------------------------------------
MD5::MD5() : length_(0), message_block_index_(0), corrupted_(false) {

	h_[0] = 0x67452301;
	h_[1] = 0xefcdab89;
	h_[2] = 0x98badcfe;
	h_[3] = 0x10325476;

	pad_message();
}

//------------------------------------------------------------------------------
// Name: MD5(const void *message, std::size_t n)
// Desc:
//------------------------------------------------------------------------------
MD5::MD5(const void *message, std::size_t n) : length_(0), message_block_index_(0), corrupted_(false) {

	h_[0] = 0x67452301;
	h_[1] = 0xefcdab89;
	h_[2] = 0x98badcfe;
	h_[3] = 0x10325476;

	const quint8 *message_ptr = static_cast<const quint8 *>(message);

	while(n-- && !corrupted_) {
		message_block_[message_block_index_++] = *message_ptr++;

		length_ += 8;
		if(length_ == 0) {
			// message is too long
			corrupted_ = true;
		}

		if(message_block_index_ == sizeof(message_block_)) {
			process_message_block();
		}
	}

	pad_message();
}

//------------------------------------------------------------------------------
// Name: MD5(const MD5 &other)
// Desc:
//------------------------------------------------------------------------------
MD5::MD5(const MD5 &other) : length_(other.length_), message_block_index_(other.message_block_index_), corrupted_(other.corrupted_) {
	for(int i = 0; i < 4; ++i) {
		h_[i] = other.h_[i];
	}

	for(std::size_t i = 0; i < sizeof(message_block_); ++i) {
		message_block_[i] = other.message_block_[i];
	}
}

//------------------------------------------------------------------------------
// Name: operator=(const MD5 &rhs)
// Desc:
//------------------------------------------------------------------------------
MD5 &MD5::operator=(const MD5 &rhs) {
	if(this != &rhs) {
		MD5(rhs).swap(*this);
	}
	return *this;
}


//------------------------------------------------------------------------------
// Name: result(quint32 *message_digest_array) const
// Desc:
//------------------------------------------------------------------------------
bool MD5::result(quint32 *message_digest_array) const {
	if(corrupted_) {
		return false;
	}

	for(int i = 0; i < 4; ++i) {
		message_digest_array[i] = h_[i];
	}

	return true;
}

//------------------------------------------------------------------------------
// Name: swap(MD5 &other)
// Desc:
//------------------------------------------------------------------------------
void MD5::swap(MD5 &other) {
	using std::swap;

	for(int i = 0; i < 4; ++i) {
		swap(h_[i], other.h_[i]);
	}

	for(std::size_t i = 0; i < sizeof(message_block_); ++i) {
		swap(message_block_[i], other.message_block_[i]);
	}

	swap(length_, other.length_);
	swap(message_block_index_, other.message_block_index_);
	swap(corrupted_, other.corrupted_);
}

//------------------------------------------------------------------------------
// Name: to_string() const
// Desc:
//------------------------------------------------------------------------------
QString MD5::to_string() const {
	static const char hexchars[] = "0123456789abcdef";
	char buf[32] = "";

	if(!corrupted_) {
		for(int i = 0; i < 4; ++i) {
			buf[0 + 8 * i] = hexchars[(h_[i] & 0x000000f0) >> 0x04];
			buf[1 + 8 * i] = hexchars[(h_[i] & 0x0000000f) >> 0x00];
			buf[2 + 8 * i] = hexchars[(h_[i] & 0x0000f000) >> 0x0c];
			buf[3 + 8 * i] = hexchars[(h_[i] & 0x00000f00) >> 0x08];
			buf[4 + 8 * i] = hexchars[(h_[i] & 0x00f00000) >> 0x14];
			buf[5 + 8 * i] = hexchars[(h_[i] & 0x000f0000) >> 0x10];
			buf[6 + 8 * i] = hexchars[(h_[i] & 0xf0000000) >> 0x1c];
			buf[7 + 8 * i] = hexchars[(h_[i] & 0x0f000000) >> 0x18];
		}
	}

	return QString::fromLatin1(buf, sizeof(buf));
}


//------------------------------------------------------------------------------
// Name: pad_message()
// Desc: finalizes the state, making digest valid
//------------------------------------------------------------------------------
void MD5::pad_message() {

	if(message_block_index_ > 55) {
		message_block_[message_block_index_++] = 0x80;
		while(message_block_index_ < sizeof(message_block_)) {
			message_block_[message_block_index_++] = 0;
		}

		process_message_block();

		while(message_block_index_ < 56) {
			message_block_[message_block_index_++] = 0;
		}
	} else {
		message_block_[message_block_index_++] = 0x80;
		while(message_block_index_ < 56) {
			message_block_[message_block_index_++] = 0;
		}
	}

	// append length (before padding)
	message_block_[56] = (length_) & 0xff;
	message_block_[57] = (length_ >> 8) & 0xff;
	message_block_[58] = (length_ >> 16) & 0xff;
	message_block_[59] = (length_ >> 24) & 0xff;
	message_block_[60] = (length_ >> 32) & 0xff;
	message_block_[61] = (length_ >> 40) & 0xff;
	message_block_[62] = (length_ >> 48) & 0xff;
	message_block_[63] = (length_ >> 56) & 0xff;
	process_message_block();

	// clear out sensitive information
	std::memset(message_block_, 0, sizeof(message_block_));
}

//------------------------------------------------------------------------------
// Name: process_message_block()
// Desc: MD5 basic transformation. Transforms state based on block.
//------------------------------------------------------------------------------
void MD5::process_message_block() {

#define FF do_transform<F>
#define GG do_transform<G>
#define HH do_transform<H>
#define II do_transform<I>

	// Constants for process_message_block routine
	static const int S11 = 7;
	static const int S12 = 12;
	static const int S13 = 17;
	static const int S14 = 22;
	static const int S21 = 5;
	static const int S22 = 9;
	static const int S23 = 14;
	static const int S24 = 20;
	static const int S31 = 4;
	static const int S32 = 11;
	static const int S33 = 16;
	static const int S34 = 23;
	static const int S41 = 6;
	static const int S42 = 10;
	static const int S43 = 15;
	static const int S44 = 21;

	quint32 a = h_[0];
	quint32 b = h_[1];
	quint32 c = h_[2];
	quint32 d = h_[3];

	const quint32 *const x = reinterpret_cast<const quint32 *>(message_block_);

	/* Round 1 */
	FF(a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF(b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], S22, 0x02441453); /* 22 */
	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[ 6], S34, 0x04881d05); /* 44 */
	HH(a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II(d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	h_[0] += a;
	h_[1] += b;
	h_[2] += c;
	h_[3] += d;

	message_block_index_ = 0;

#undef FF
#undef GG
#undef HH
#undef II
}

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
bool MD5::operator<(const MD5 &rhs) const {
	return
		((h_[0] < rhs.h_[0]) || ((h_[0] == rhs.h_[0]) &&
		((h_[1] < rhs.h_[1]) || ((h_[1] == rhs.h_[1]) &&
		((h_[2] < rhs.h_[2]) || ((h_[2] == rhs.h_[2]) &&
		(h_[3] < rhs.h_[3])))))));
}

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
bool MD5::operator==(const MD5 &rhs) const {
	return
		h_[0] == rhs.h_[0] &&
		h_[1] == rhs.h_[1] &&
		h_[2] == rhs.h_[2] &&
		h_[3] == rhs.h_[3];
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
MD5 MD5::from_raw(const quint32 *value) {
	MD5 md5((internal_tag()));

	md5.length_              = 0;
	md5.corrupted_           = false;
	md5.message_block_index_ = 0;

	md5.h_[0] = value[0];
	md5.h_[1] = value[1];
	md5.h_[2] = value[2];
	md5.h_[3] = value[3];

#if 0
	std::memset(md5.message_block_, 0, sizeof(md5.message_block_));
#endif
	return md5;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
MD5 MD5::from_string(const QString &hash) {

	MD5 md5((internal_tag()));
	md5.length_              = 0;
	md5.corrupted_           = false;
	md5.message_block_index_ = 0;

	if(hash.size() == 32) {
		for(int i = 0; i < 4; ++i) {
			md5.h_[i]  = (hash[0 + i * 8].isDigit() ? hash[0 + i * 8].toAscii() - '0' : hash[0 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x1c;
			md5.h_[i] |= (hash[1 + i * 8].isDigit() ? hash[1 + i * 8].toAscii() - '0' : hash[1 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x18;
			md5.h_[i] |= (hash[2 + i * 8].isDigit() ? hash[2 + i * 8].toAscii() - '0' : hash[2 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x14;
			md5.h_[i] |= (hash[3 + i * 8].isDigit() ? hash[3 + i * 8].toAscii() - '0' : hash[3 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x10;
			md5.h_[i] |= (hash[4 + i * 8].isDigit() ? hash[4 + i * 8].toAscii() - '0' : hash[4 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x0c;
			md5.h_[i] |= (hash[5 + i * 8].isDigit() ? hash[5 + i * 8].toAscii() - '0' : hash[5 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x08;
			md5.h_[i] |= (hash[6 + i * 8].isDigit() ? hash[6 + i * 8].toAscii() - '0' : hash[6 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x04;
			md5.h_[i] |= (hash[7 + i * 8].isDigit() ? hash[7 + i * 8].toAscii() - '0' : hash[7 + i * 8].toUpper().toAscii() - 'A' + 10) << 0x00;
		}
	}
	return md5;
}
