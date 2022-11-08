include_directories(. ../bst_tests)

add_header_problem(
	NAME avl 
	TEST_SOURCE 
		test_insert.cpp
   	 	test_remove.cpp)
