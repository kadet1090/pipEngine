#ifndef GC_H_
#define GC_H_

#include <gc_cpp.h>

typedef gc_cleanup GCed;
#define gc_new new (UseGC)
#define normal_new new (NoGC) //TODO How the heck Boehm GC does it?

#endif
