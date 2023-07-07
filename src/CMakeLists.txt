add_executable(shared_ptr main.cpp)

target_include_directories(shared_ptr PUBLIC ${MY_INCLUDE_DIR})

add_library(my_lib ${MY_INCLUDE_DIR}/shared_ptr.h
        ${MY_INCLUDE_DIR}/shared_ptr_def.h shared_ptr_inst.cpp)

target_include_directories(my_lib PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(shared_ptr PRIVATE my_lib)

