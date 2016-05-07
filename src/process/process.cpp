/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */

#include "base/process/process.h"
#include "base/core/macro.h"

#include <errno.h>

namespace Base {
namespace Process {

pid_t getpid() { return ::getpid(); }

pid_t fork() { return ::fork(); }

int exec(const char *path, char *const argv[]) { return ::execv(path, argv); }

pid_t spawn(const char *path, char *const argv[]) {
  pid_t pid = fork();
  if(pid < 0) {
    return -1; // error: fork() failed.
  }
  if(pid == 0) {
    int res = exec(path, argv);
    if(res == -1) {
      BASE_LOG("exec failed with errno: %d\n", errno);
      return -1;
    }
  }
  return pid;
}

} // namespace Process
} // namespace Base
