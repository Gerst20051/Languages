OPTION(INFINITE_LOOP_DETECTION "Enable Infinite Loop Detection" ON)
OPTION(INFINITE_RECURSION_DETECTION "Enable Infinite Recursion Detection" ON)
OPTION(REQUEST_TIMEOUT_DETECTION "Enable Timeout Detection" ON)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

IF(NOT DEFINED CMAKE_PREFIX_PATH)
  message(STATUS "CMAKE_PREFIX_PATH was missing, proceeding anyway")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	INCLUDE(CheckCSourceCompiles)
	CHECK_C_SOURCE_COMPILES("#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 40300
#error Need GCC 4.3.0+
#endif
int main() { return 0; }" HAVE_GCC_43)

	if(NOT HAVE_GCC_43)
		message(FATAL_ERROR "Need at least GCC 4.3")
	endif()

endif()

set(FREEBSD FALSE)
set(LINUX FALSE)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
	set(FREEBSD TRUE)
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(LINUX TRUE)
endif()

LIST(APPEND CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH}")

if(APPLE)
	if(EXISTS "/opt/local/var/macports/")
		LIST (APPEND CMAKE_PREFIX_PATH "/opt/local")
		LIST (APPEND CMAKE_LIBRARY_PATH "/opt/local/lib/x86_64")
	endif()
endif()

include(HPHPFunctions)
include(HPHPFindLibs)

add_definitions(-D_GNU_SOURCE -D_REENTRANT=1 -D_PTHREADS=1)

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
	add_definitions(-DRELEASE=1)
endif()

if(APPLE OR FREEBSD)
	add_definitions(-DSKIP_USER_CHANGE=1)
endif()

# eable the OSS options if we have any
add_definitions(-DHPHP_OSS=1)

IF($ENV{CXX} MATCHES "icpc")
	set(CMAKE_C_FLAGS "-no-ipo -fp-model precise -wd584 -wd1418 -wd1918 -wd383 -wd869 -wd981 -wd424 -wd1419 -wd444 -wd271 -wd2259 -wd1572 -wd1599 -wd82 -wd177 -wd593 -w -fPIC")
	set(CMAKE_CXX_FLAGS "-no-ipo -fp-model precise -wd584 -wd1418 -wd1918 -wd383 -wd869 -wd981 -wd424 -wd1419 -wd444 -wd271 -wd2259 -wd1572 -wd1599 -wd82 -wd177 -wd593 -fPIC -fno-omit-frame-pointer -ftemplate-depth-60 -Wall -Woverloaded-virtual -Wno-deprecated -w1 -Wno-strict-aliasing -Wno-write-strings -Wno-invalid-offsetof -fno-operator-names")
else()
	set(CMAKE_C_FLAGS "-w -fPIC")
	set(CMAKE_CXX_FLAGS "-fPIC -fno-omit-frame-pointer -ftemplate-depth-60 -Wall -Woverloaded-virtual -Wno-deprecated -Wno-parentheses -Wno-strict-aliasing -Wno-write-strings -Wno-invalid-offsetof -fno-operator-names")
endif()

include_directories(${HPHP_HOME}/src)
include_directories(${HPHP_HOME}/src/lib/system/gen)
