/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#pragma once

#include <cstring> // size_t

namespace Base {

enum OpenMode {
  OM_Append = 0x1,
  OM_Read = 0x2,
  OM_Write = 0x4,
  OM_ReadWrite = 0x8,
  OM_Trunc = 0x10,
  OM_Create = 0x20
};

typedef int FileHandle;

static const FileHandle kInvalidHandle = -1;

FileHandle Open(const char *path, int mode);
void Close(FileHandle file);
size_t Read(FileHandle file, void *buffer, size_t nbytes);
size_t Write(FileHandle file, const void *buffer, size_t nbytes);
size_t Size(FileHandle file);

} // namespace Base
