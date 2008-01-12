#ifndef __ZIA_H__
#define __ZIA_H__

#ifdef WIN32
 #ifdef FD_SETSIZE
  #undef FD_SETSIZE
 #endif
 #define FD_SETSIZE 512 // By default on windows FD are limited to 64.
#endif

#endif // __ZIA_H__