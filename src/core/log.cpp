/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#include "base/core/log.h"
#include "base/core/time_utils.h"

#include <stdio.h>
#include <thread>
#include <stdarg.h>
#include <vector>

namespace Base {
namespace Log {

static const LogChannel kAny("");

struct HookData {
  LogChannel channel;
  int filter;
  LogHook callback;
  void *context;
};

// todo(kstasik): better init/release
static std::vector<HookData> g_hooks;

static int gLogLevelMask =
    kLogDebug | kLogInfo | kLogWarning | kLogError | kLogCritical;

void SetLevelFilter(int level_mask) { gLogLevelMask = level_mask; }

int GetLevelFilter() { return gLogLevelMask; }

const char *GetLevelString(int level) {
  switch(level) {
  case kLogDebug:
    return "DBG";
  case kLogInfo:
    return "INF";
  case kLogWarning:
    return "WAR";
  case kLogError:
    return "ERR";
  case kLogCritical:
    return "CRI";
  default:
    break;
  }
  return "???";
}

const char *SkipParentDirs(const char *path) {
  const char *result = path;
  while(result[0] == '.' && result[1] == '.' && result[2] == '/') {
    result = &result[3];
  }
  return result;
}

void WriteImpl(const char *file, int line, int level, const LogChannel &channel,
               const char *format, va_list arg_list) {
  const int max_line_length = Base::Log::kLogLineMaxLength;
  char logline[max_line_length];
  // NOTE: remove three bytes to ensure that there is space for the CRLF
  //			 and the null terminator.
  int avail = max_line_length - 3;
  int offset = 0;
  int n = 0;

  const char *level_string = GetLevelString(level);
  n = snprintf(&logline[offset], avail, "%d|%s|%s|msg=",
               Base::Time::GetTimeMs(), level_string, channel.GetName());
  offset += n;
  avail -= n;

  // write the log message
  n = vsnprintf(&logline[offset], avail, format, arg_list);
  if(n >= 0 && n < avail) {
    offset += n;
    avail -= n;
  } else {
    offset = max_line_length - 3;
    avail = 0;
  }

  // early out if there is no message or buffer full.
  if(0 >= n || avail < 0) {
    return;
  }

  // write the log suffix
  n = snprintf(&logline[offset], avail, " file=%s line=%d",
               SkipParentDirs(file), line);
  if(n >= 0 && n < avail) {
    offset += n;
    avail -= n;
  } else {
    offset = max_line_length - 3;
    avail = 0;
  }

  logline[offset++] = '\r';
  logline[offset++] = '\n';
  logline[offset] = 0;

  if(g_hooks.empty()) {
    ConsoleOutput(level, logline, offset, nullptr);
  }

  for(auto it : g_hooks) {
    if((it.channel == kAny || it.channel == channel) &&
       (it.filter & level) != 0) {
      it.callback(level, logline, offset, it.context);
    }
  }
}

void ConsoleOutput(int level, const char *logline, int length, void *context) {
  printf("%s", logline);
}

void Write(const char *file, int line, int level, const LogChannel &channel,
           const char *format, ...) {
  va_list args;
  va_start(args, format);
  WriteImpl(file, line, level, channel, format, args);
  va_end(args);
}

void Register(int filter, LogHook callback, void *context) {
  Register(kAny, filter, callback, context);
}

void Register(const LogChannel &channel, int filter, LogHook callback,
              void *context) {
  HookData data = {channel, filter, callback, context};
  g_hooks.push_back(data);
}

} // namespace Log
} // namespace Base
