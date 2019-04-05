#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test_MemoryMap.cpp"
#include "test_Interpreter.cpp"

int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}