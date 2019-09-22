#ifdef _WINDOWS
#include <time.h>
#else
#include <unistd.h>
#define sleep(x) usleep((x)*1000)
#endif
