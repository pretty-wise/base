cmake_minimum_required (VERSION 2.6)
project (base)
set (base_VERSION_MAJOR 0)
set (base_VERSION_MINOR 1)

include (settings.cmake)

set(TARGET_EXPORT_NAME "base-targets")
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries.")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for binaries.")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for includes.")
set(INSTALL_CMAKE_DIR cmake CACHE PATH "Installation directory for cmake files.")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
	set(var INSTALL_${p}_DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

include_directories (${CMAKE_SOURCE_DIR}/include)

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
 	src/core/time_utils.cpp
  src/core/string_id.cpp
  src/core/shared_object.cpp
  src/core/str.cpp
	include/base/core/log.h
	src/core/log.cpp
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

add_library (base
  ${CORE_FILES}
  ${MATH_FILES}
  ${NETWORK_FILES}
  ${RTTI_FILES}
  ${IO_FILES}
  ${THREAD_FILES}
  ${MEMORY_FILES}
	${PROCESS_FILES}
)

install(TARGETS base
	DESTINATION ${INSTALL_LIB_DIR}
	EXPORT ${TARGET_EXPORT_NAME}
)

export (TARGETS base 
	FILE "${CMAKE_BINARY_DIR}/${TARGET_EXPORT_NAME}.cmake"
)
export (PACKAGE base)

file(RELATIVE_PATH RELATIVE_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
	   "${INSTALL_INCLUDE_DIR}"
)
set(CONF_INCLUDE_DIRS "\${BASE_CMAKE_DIR}/${RELATIVE_INCLUDE_DIR}")

configure_file(${CMAKE_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_BINARY_DIR}/base-config.cmake @ONLY
)

install(DIRECTORY include/
	DESTINATION ${INSTALL_INCLUDE_DIR}
)

install(FILES 
	"${CMAKE_BINARY_DIR}/base-config.cmake"
	DESTINATION ${INSTALL_CMAKE_DIR}
	COMPONENT dev
)

install(EXPORT ${TARGET_EXPORT_NAME}
	DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev
)
