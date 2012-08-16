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

#ifndef MD5_20070328_H_
#define MD5_20070328_H_

#include <QtGlobal>
#include <QString>
#include <cstddef>

class MD5 {
public:
	template <class In>
	MD5(In first, In last) : length_(0), message_block_index_(0), corrupted_(false) {

		h_[0] = 0x67452301;
		h_[1] = 0xefcdab89;
		h_[2] = 0x98badcfe;
		h_[3] = 0x10325476;

	while(first != last && !corrupted_) {
			message_block_[message_block_index_++] = *first++;

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


	MD5(const void *message, std::size_t n);
	MD5();
	MD5(const MD5 &other);
	MD5 &operator=(const MD5 &rhs);

private:
	struct internal_tag {};
	MD5(const internal_tag&) {}

public:
	bool operator<(const MD5 &rhs) const;
	bool operator==(const MD5 &rhs) const;

public:
	static MD5 from_raw(const quint32 *value);
	static MD5 from_string(const QString &hash);

public:
	const quint32 *digest() const {
		return h_;
	}
	
	bool result(quint32 *message_digest_array) const;
	void swap(MD5 &other);
	QString to_string() const;

private:
	void input(const void *input, std::size_t input_length);
	void process_message_block();
	void pad_message();

private:
	quint8      message_block_[64]; // input buffer
	quint32     h_[4];              // current digest
	quint64     length_;            // number of bits
	std::size_t message_block_index_;
	bool        corrupted_;
};

#endif

