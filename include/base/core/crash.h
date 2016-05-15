#pragma once

namespace Base {

// Spawns a daemon process that listens for crash data and processes it.
bool RegisterCrashHandler();

void Crash();

} // namespace Base
