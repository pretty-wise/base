cmake_minimum_required (VERSION 3.0)
project (Base)
set (Base_VERSION_MAJOR 0)
set (Base_VERSION_MINOR 1)
set (Base_VERSION 0.1)

include (settings.cmake)

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries.")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for binaries.")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for includes.")
set(INSTALL_CMAKE_DIR cmake CACHE PATH "Installation directory for cmake files.")

set (CORE_FILES 
	include/base/core/types.h
  include/base/core/macro.h
  include/base/core/handle.h
  include/base/core/ref_counted.h
  include/base/core/service_locator.h
  include/base/core/shared_object.h
  include/base/core/shared_ptr.h
  include/base/core/string_id.h
  include/base/core/time_utils.h
  include/base/core/types.h
  include/base/core/str.h
  include/base/core/crash.h
 	src/core/time_utils.cpp
  src/core/string_id.cpp
  src/core/shared_object.cpp
  src/core/str.cpp
  src/core/crash.cpp
	include/base/core/log.h
	src/core/log.cpp
	include/base/core/log_file.h
	src/core/log_file.cpp
)

set (MATH_FILES 
	include/base/math/crc.h
  include/base/math/crc.inl
  include/base/math/math.h
  include/base/math/math.inl
  include/base/math/matrix4x4.h
  include/base/math/quaternion.h
  include/base/math/ray.h
  include/base/math/vec2.h
  include/base/math/vec3.h
  include/base/math/vec4.h
)

set (NETWORK_FILES 
	include/base/network/address.h
  include/base/network/url.h
  include/base/network/socket.h
  include/base/network/epoll.h
  src/network/address.cpp
  src/network/url.cpp
  src/network/socket.cpp
)

if(APPLE)
set (NETWORK_FILES 
  ${NETWORK_FILES}
  src/network/epoll_macosx.cpp
)
elseif(UNIX)
set (NETWORK_FILES 
  ${NETWORK_FILES}
  src/network/epoll_linux.cpp
)
endif()


set (RTTI_FILES 
	include/base/rtti/dynamic_types.h
  include/base/rtti/rtti.h
)

set (IO_FILES 
  include/base/io/binary_reader.h
  include/base/io/binary_writer.h
  include/base/io/bit_reader.h
  include/base/io/bit_writer.h
  include/base/io/memory_stream.h
  include/base/io/reader.h
  include/base/io/stream.h
  include/base/io/writer.h
	include/base/io/base_file.h
  src/io/binary_writer.cpp
  src/io/binary_reader.cpp
  src/io/bit_writer.cpp
  src/io/bit_reader.cpp
  src/io/memory_stream.cpp
  src/io/reader.cpp
	src/io/base_file_linux.cpp
)

set (MEMORY_FILES 
	include/base/memory/allocator.h
  include/base/memory/ring_allocator.h
  src/memory/ring_allocator.cpp
  include/base/memory/ring_buffer.h
  src/memory/ring_buffer.cpp
)

set (THREAD_FILES 
	include/base/threading/mutex.h
  include/base/threading/condition_variable.h
  include/base/threading/thread.h
  src/threading/condition_variable.cpp
  src/threading/mutex.cpp
  src/threading/thread.cpp
)

set (PROCESS_FILES
	include/base/process/process.h
	src/process/process.cpp
)

add_library (Base STATIC
  ${CORE_FILES}
  ${MATH_FILES}
  ${NETWORK_FILES}
  ${RTTI_FILES}
  ${IO_FILES}
  ${THREAD_FILES}
  ${MEMORY_FILES}
	${PROCESS_FILES}
)

target_include_directories (Base PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_include_directories (Base PRIVATE src)

install (TARGETS Base EXPORT base_targets
	LIBRARY DESTINATION ${INSTALL_LIB_DIR}
	ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
	RUNTIME DESTINATION ${INSTALL_BIN_DIR}
	INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
)

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/base"
	DESTINATION ${INSTALL_INCLUDE_DIR}
  FILES_MATCHING PATTERN "*.*h"
)

install (EXPORT base_targets
	FILE BaseTargets.cmake
	NAMESPACE Base::
	DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("../BaseConfigVersion.cmake"
	VERSION ${Base_VERSION}
	COMPATIBILITY SameMajorVersion
)

install (FILES "BaseConfig.cmake" "BaseConfigVersion.cmake"
	DESTINATION cmake
)
