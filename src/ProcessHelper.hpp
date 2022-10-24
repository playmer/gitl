// tiny-process-library is always compiled as mbcs, so we need those functions availible to us.

#ifdef UNICODE
#define UNICODE_WAS_DEFINED
#endif

#include "process.hpp"

#ifdef UNICODE_WAS_DEFINED
#define UNICODE
#endif
