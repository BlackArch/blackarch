#  Makefile for the project
#  Robert Swiecki <robert@swiecki.net>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307

CC ?= gcc
COMMON_CFLAGS = -fPIC -O3 -g -c -std=gnu11  -I. \
                 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_GNU_SOURCE \
                 -Wall -Werror -Wextra -Wno-address-of-packed-member

COMMON_LDFLAGS = -fPIE -lpthread

CFLAGS += $(COMMON_CFLAGS)

LD ?= $(CC)
LDFLAGS += $(COMMON_LDFLAGS)

SRCS = debug.c intrace.c threads.c listener.c \
       sender.c display.c ipv4.c ipv6.c

OBJS = $(SRCS:.c=.o)
BIN = intrace

all: $(BIN)

.c.o: %.c
	$(CC) $(CFLAGS) $<

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

clean:
	rm -f core $(OBJS) $(BIN)

indent:
	indent -linux -l100 -lc100 *.c *.h; rm -f *~

# DO NOT DELETE
