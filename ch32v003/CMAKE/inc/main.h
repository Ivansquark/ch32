#ifndef MAIN_H
#define MAIN_H


// Macros for visibility attributes.
//   _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY
//   _GLIBCXX_VISIBILITY
# define _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY 1

#if _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY
//# define _GLIBCXX_VISIBILITY(V) __attribute__ ((__visibility__ (#V)))
# define _GLIBCXX_VISIBILITY(V) __attribute__ ((__visibility__ ("default")))
#else
// If this is not supplied by the OS-specific or CPU-specific
// headers included below, it will be defined to an empty default.
# define _GLIBCXX_VISIBILITY(V) _GLIBCXX_PSEUDO_VISIBILITY(V)
#endif


#include "ch32v00x.h"

#endif // MAIN_H
