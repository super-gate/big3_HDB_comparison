# 요구 CMake 최소 버전
CMAKE_MINIMUM_REQUIRED ( VERSION 3.11)
 
# 프로젝트 이름 및 버전
PROJECT ( "HDB_supergate" )
SET ( PROJECT_VERSION_MAJOR 0 )
SET ( PROJECT_VERSION_MINOR 1 )
 
# 빌드 형상(Configuration) 및 주절주절 Makefile 생성 여부
#SET ( CMAKE_BUILD_TYPE Debug )
#SET ( CMAKE_BUILD_TYPE RelWithDebInfo)
SET (CMAKE_BUILD_TYPE Release)
SET ( CMAKE_VERBOSE_MAKEFILE true )
 
# 빌드 대상 바이너리 파일명 및 소스 파일 목록
#SET ( OUTPUT_ELF
#        "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.out"
#       )
#SET ( SRC_FILES 
#	EigenD.cpp
#	)

# 공통 컴파일러
#SET ( CMAKE_C_COMPILER "clang++" )
SET ( CMAKE_C_COMPILER "c" )
 
# Change the compliler...
#SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_C_COMPILER             "/usr/bin/c")
SET (CMAKE_C_FLAGS                "-Wall -std=c++17 ")


SET (CMAKE_C_FLAGS                "-Wall")
SET (CMAKE_C_FLAGS_DEBUG          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
#SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
SET (CMAKE_CXX_COMPILER             "/usr/bin/c++")
SET (CMAKE_CXX_FLAGS                "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
#SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
#SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")


#SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
#SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")


SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
#SET(CMAKE_CXX_EXTENSIONS OFF)

# 공통 컴파일 옵션, 링크 옵션
ADD_COMPILE_OPTIONS ( -g -Wall )
# In MAC, static option should be removed
#SET ( CMAKE_EXE_LINKER_FLAGS "-static -Wl,--gc-sections" )







# 공통 헤더 파일 Include 디렉토리 (-I)

INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/include)


INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}/include)

INCLUDE_DIRECTORIES (/usr/local/include)


# 공통 링크 라이브러리 (-l)

find_package(helib REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})


LINK_LIBRARIES(helib)
#LINK_LIBRARIES(mpfr)




# 공통 링크 라이브러리 디렉토리 (-L)
LINK_DIRECTORIES ( /usr/lib )
LINK_DIRECTORIES (/usr/local/lib)
 

# 이제부터 user 코드들을 가지고 빌드하는 곳
SET( SOURCESS 
    ${CMAKE_SOURCE_DIR}/src/HDB_supergate.cpp
    ${CMAKE_SOURCE_DIR}/src/HDB_supergate_server.cpp
    ${CMAKE_SOURCE_DIR}/src/HDB_supergate_user.cpp
    )

SET( HEADERSS
    ${CMAKE_SOURCE_DIR}/include/HDB_supergate.hpp
    ${CMAKE_SOURCE_DIR}/include/HDB_supergate_server.hpp
    ${CMAKE_SOURCE_DIR}/include/HDB_supergate_user.hpp
    )

# Make Library 
ADD_LIBRARY(HDB_supergate_lib STATIC ${SOURCESS})
# TARGET_INCLUDE_DIRECTORIES(HDB_supergate_lib PUBLIC include)


INSTALL(TARGETS HDB_supergate_lib
    DESTINATION ${CMAKE_SOURCE_DIR}/lib/bin
    #    DESTINATION ~/Dropbox/Developes/HDB_supergate/lib/lib
)

INSTALL(FILES ${HEADERSS} 
    DESTINATION ${CMAKE_SOURCE_DIR}/lib/include)

#cmake -Dhelib_DIR=/home/smhan/Documents/helib_install/helib_pack/share/cmake/helib ..