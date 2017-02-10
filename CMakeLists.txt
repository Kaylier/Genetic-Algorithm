cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)

project(GeneticAlgorithm)


set(CMAKE_C_FLAGS "-std=c11 -pipe ")
set(CMAKE_CXX_FLAGS "-std=c++14 -pipe ")

## C/C++ Warning options
string(APPEND C_CXX_FLAGS_WARNINGS "-Wall ") # many warnings
string(APPEND C_CXX_FLAGS_WARNINGS "-Wextra ") # more warnings
string(APPEND C_CXX_FLAGS_WARNINGS "-Wpedantic ") # strict ISO
## C/C++ Advanced warning options
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Warray-bounds ") # warn about subscripts to arrays that are always out of bounds
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wcast-align ") # warn when the alignment is not respected in a cast
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wcast-qual ") # check stranges cast with differents qualifier
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wconversion ") # warn for implicit conversions
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wdate-time ") # warn for the use of "__TIME__", "__DATE__" or "__TIMESTAMP__"
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wdouble-promotion ") # warn for literals treated as double instead of float
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wduplicated-cond ") # warn for duplicate conditions
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wfloat-equal ") # warn if floating-point values are compared
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wformat=2 ") # check calls to printf, scanf, ...
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wformat-signedness ") # also check for signe
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wlogical-op ") # warn about suspicious logical operations
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wmissing-include-dirs ") # warn if an include directory doesn't exist
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wnormalized=none ") # check for similar characters (UTF...) which are differents
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wplacement-new=2 ") # check for placements
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wredundant-decls ") # warn if anything is declared twice in the same scope
#string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wshadow ") # warn if a local variable shadow something
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wwrite-strings ") # check for const char*
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wmissing-declarations ") # warn if a function is defined without prototype
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wundef ") # warn about undefined preprocessor identifiers
string(APPEND C_CXX_FLAGS_WARNINGS_ADV "-Wunused ") # warn about unused things
## C/C++ Optimisation warnings
string(APPEND C_CXX_FLAGS_OPTIM "-O3 ") # enable optimization
#string(APPEND CCXX_FLAGS_OPTIM "-Os ") # enable optimization for size
string(APPEND C_CXX_FLAGS_OPTIM "-Wdisabled-optimization ") # warn if a requested optimization pass is disabled
string(APPEND C_CXX_FLAGS_OPTIM "-Wstrict-overflow ") # warn about simplification that may modify overflow behaviour
string(APPEND C_CXX_FLAGS_OPTIM "-Wstrict-overflow=4 ")
string(APPEND C_CXX_FLAGS_OPTIM "-Wunsafe-loop-optimizations ") # warn if a loop optimization modify overflow behaviour
## C/C++ Tracking options
string(APPEND C_CXX_FLAGS_DEBUG "-ggdb2 -DDEBUG ")
string(APPEND C_CXX_FLAGS_DEBUG "-fstack-protector-all ") # track buffer overflows
string(APPEND C_CXX_FLAGS_DEBUG "-fdelete-null-pointer-checks ") # follow null dereference
string(APPEND C_CXX_FLAGS_DEBUG "-flto-odr-type-merging ") # enable detailed diagnostic about definitions
string(APPEND C_CXX_FLAGS_DEBUG "-feliminate-unused-debug-symbols ") # debug info don't have unused symbols
string(APPEND C_CXX_FLAGS_DEBUG "-fvar-tracking ") # track variables
## C Warnings
string(APPEND C_FLAGS_WARNINGS "-Wbad-function-cast ") # warn when a cast don't match
string(APPEND C_FLAGS_WARNINGS "-Wmissing-prototypes ") # warn if a function is defined without prototype
string(APPEND C_FLAGS_WARNINGS "-Wincompatible-pointer-types ") # warn for incompatible type conversion of pointers
string(APPEND C_FLAGS_WARNINGS "-Wstrict-prototypes ") # check the argument types
string(APPEND C_FLAGS_WARNINGS "-Wjump-misses-init ") # warn if a jump cause problem with initializations
string(APPEND C_FLAGS_WARNINGS "-Wunsuffixed-float-constants ") # warn for floating constant without a suffix
## C++ Warnings
string(APPEND CXX_FLAGS_WARNINGS "-Wctor-dtor-privacy ") # check strange privacy in classes
string(APPEND CXX_FLAGS_WARNINGS "-Wnon-virtual-dtor ") # check for dangerous non-virtual destructors
string(APPEND CXX_FLAGS_WARNINGS "-Woverloaded-virtual ") # warn if a virtual function is hidden
string(APPEND CXX_FLAGS_WARNINGS "-Wsign-promo ") # warn when overload chooses a signed type instead of an unsigned one
string(APPEND CXX_FLAGS_WARNINGS "-Wzero-as-null-pointer-constant ") # warn when 0 is used instead of NULL
string(APPEND CXX_FLAGS_WARNINGS "-Wsign-conversion ") # check sign conversions
string(APPEND CXX_FLAGS_WARNINGS "-Wstrict-null-sentinel ") # warn about NULL sentinels
## C++ Suggestions
string(APPEND CXX_FLAGS_SUGGEST "-Wsuggest-final-types ") # suggest to make final somes virtual types
string(APPEND CXX_FLAGS_SUGGEST "-Wsuggest-final-methods ") # suggest to make final somes virtual methods
string(APPEND CXX_FLAGS_SUGGEST "-Wsuggest-override ") # suggest to make override somes functions
string(APPEND CXX_FLAGS_SUGGEST "-Wuseless-cast ") # useless cast...

## Linkage
set(CMAKE_EXE_LINKER_FLAGS "-L$(LIBDIR) ")


enable_testing()

include_directories(include src)

file(GLOB_RECURSE HEADER_FILES include/*.h include/*.hpp)
file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.cpp)

set(EXECUTABLE exe)
add_executable(${EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(${EXECUTABLE} PROPERTIES COMPILE_FLAGS "${C_CXX_FLAGS_WARNINGS} ${C_CXX_FLAGS_OPTIM}")

set(DEBUG_EXECUTABLE debug)
add_executable(${DEBUG_EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(${DEBUG_EXECUTABLE} PROPERTIES COMPILE_FLAGS
        "${C_CXX_FLAGS_WARNINGS} ${C_CXX_FLAGS_WARNINGS_ADV} ${C_CXX_FLAGS_DEBUG} ${CXX_FLAGS_WARNINGS} ${C_CXX_FLAGS_SUGGEST}")


