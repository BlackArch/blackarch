#include <bits/wordsize.h>

#if __WORDSIZE == 32
#include "curlbuild-32.h"
#elif __WORDSIZE == 64
#include "curlbuild-64.h"
#else
#error "Unknown word size"
#endif
