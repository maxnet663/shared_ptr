add_executable(shared_ptr main.cpp ${MY_INCLUDE_DIR}/shared_ptr.hpp)

target_include_directories(shared_ptr PUBLIC ${MY_INCLUDE_DIR})