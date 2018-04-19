#if !defined (_WIN32) && !defined (_WIN64)
#define LINUX
#include <unistd.h>
#else
#define WINDOWS
#include <windows.h>
#endif

unsigned core_count()
{
  unsigned count = 1;
  #if defined (LINUX)
  count = sysconf(_SC_NPROCESSORS_CONF);
  #elif defined (WINDOWS)
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  count = si.dwNumberOfProcessors;
  #endif
  return count;
}

#include <iostream>

int main()
{
  unsigned sz = core_count();
  std::cout << sz << (1 == sz ? " core" : " cores") << '\n';
}
