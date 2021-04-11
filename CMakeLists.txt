project(generic_c)

add_definitions(-Wall
    -Wextra
    -pedantic
    -fanalyzer 
    -D_FORTIFY_SOURCE=2 
    -fstack-protector-all -Wstack-protector --param ssp-buffer-size=4
    -fPIE 
    -pie 
    -Wl,-z,relro,-z,now
    -Wconversion -Wsign-conversion
    -Wformat-security
    -mindirect-branch=thunk -mfunction-return=thunk
    -Wstack-protector
    -fstack-clash-protection
    -Wl,-z,noexecstack
    -O
    -Werror)

set(CMAKE_C_STANDARD 11)

add_executable(test src/main.c)

include_directories(include)

