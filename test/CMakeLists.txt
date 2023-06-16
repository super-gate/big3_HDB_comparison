# 요구 CMake 최소 버전
CMAKE_MINIMUM_REQUIRED ( VERSION 3.11)
 
# 프로젝트 이름 및 버전
PROJECT ( "HDB_supergate_test" )
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
INCLUDE_DIRECTORIES (/usr/local/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../lib/include/)


# HELIB
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})


# 공통 링크 라이브러리 (-l)

FIND_PACKAGE(helib REQUIRED)
LINK_LIBRARIES(helib)

#LINK_LIBRARIES(gmp)
#LINK_LIBRARIES(mpfr)


# 공통 링크 라이브러리 디렉토리 (-L)
LINK_DIRECTORIES (/usr/lib )
LINK_DIRECTORIES (/usr/local/lib)
LINK_DIRECTORIES (${CMAKE_SOURCE_DIR}/../lib/bin/)





# 테스트 코드 
SET(TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/main_query_test.cpp)


# 테스트 코드 빌드 이는 따로 파일을 만들어야 함
ADD_EXECUTABLE(HDB_supergate_test 
		${CMAKE_SOURCE_DIR}/../comp_lib/comparator.cpp
		${CMAKE_SOURCE_DIR}/../comp_lib/tools.cpp
		${TEST_SOURCES} 
		)

#TARGET_INCLUDE_DIRECTORIES(HDB_supergate_test INTERFACE ${CMAKE_SOURCE_DIR}/../lib/include/)

#TARGET_LINK_DIRECTORIES(HDB_supergate_test INTERFACE ${CMAKE_SOURCE_DIR}/../lib/bin/)


TARGET_LINK_LIBRARIES(HDB_supergate_test ${CMAKE_SOURCE_DIR}/../lib/bin/libHDB_supergate_lib.a)

SET_TARGET_PROPERTIES(HDB_supergate_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Test_dir)












# 정적 라이브러리 만들기
# add_library(HDB_supergate STATIC 
#    ./src/HDB_supergate.cpp
#)

# After ... ORIGN

# INSTALL(TARGETS DSEnum
# 	DESTINATION /usr/local/lib/
# )
# INSTALL(FILES ./include/DSEnum.hpp
#	DESTINATION /usr/local/include/)

