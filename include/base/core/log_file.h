#pragma once

#include "base/core/log.h"

namespace Base {
namespace Log {

typedef struct LogFileData *LogFile;

// Creates a log file at path location.
// @param path Location where to create a file.
// @return A context of a log sink.
LogFile CreateLogFile(const char *path);

// Close log file sink.
// @pram sink A context to destroy.
void DestroyLogFile(LogFile sink);

// Adds a channel/filter pair for a given sink.
// @param sink Log file to output the logs to.
// @param channel Log channel to output to sink.
// @param filter Log channel's filter to apply.
// @return true if successfully added the channel/filter to sink.
bool AddFilter(LogFile sink, const LogChannel &channel, int filter);

// Removes a channel from log sink
// @param sink Log sink to remove channel from.
// @param channnel Log channel to remove from sink.
void RemoveFilter(LogFile sink, const LogChannel &channel);

} // namespace Log
} // namespace Base
