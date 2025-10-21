find_package(GTest CONFIG REQUIRED)

add_executable(mem_pool_test mem_pool_test.cc)
target_compile_options(mem_pool_test PRIVATE -fsanitize=address)
target_link_libraries(mem_pool_test PRIVATE Common gtest gtest_main -fsanitize=address)

add_executable(lf_queue_test lf_queue_test.cc)
target_compile_options(lf_queue_test PRIVATE -fsanitize=address)
target_link_libraries(lf_queue_test PRIVATE Common gtest gtest_main -fsanitize=address)

add_executable(time_util_test time_util_test.cc)
target_link_libraries(time_util_test PRIVATE Common gtest gtest_main)

include(GoogleTest)

gtest_discover_tests(mem_pool_test)
gtest_discover_tests(lf_queue_test)
gtest_discover_tests(time_util_test)
