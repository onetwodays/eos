
add_executable(blockvault_tests ../log_store.cpp log_store_tests.cpp)
target_link_libraries(blockvault_tests nuraft fc Boost::unit_test_framework)