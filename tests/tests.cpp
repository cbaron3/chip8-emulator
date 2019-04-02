#include <gtest/gtest.h>

#include "test_MemoryMap.cpp"
#include "test_CPU.cpp"

int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}