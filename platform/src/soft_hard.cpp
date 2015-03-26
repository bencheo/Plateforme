#include "soft_hard_common.hpp"
#ifdef UNIX
#include "soft_hard_linux.hpp"
#else
#include "soft_hard_win.hpp"
#endif