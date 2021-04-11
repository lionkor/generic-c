project(generic_c)

add_definitions(-Wall
    -Wextra
    -pedantic
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
    -Werror
    -fsanitize=undefined,address)

set(CMAKE_C_STANDARD 11)

add_executable(test src/main.c)

target_link_libraries(test asan ubsan)

include_directories(include)

