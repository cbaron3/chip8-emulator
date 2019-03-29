#include "../src/Logger.cpp"
#include "../src/Memory.cpp"
#include <gtest/gtest.h>

TEST(MemoryTest, MemoryMap)
{
	chip8::MemoryMap memory(10, 1);
	ASSERT_ANY_THROW(memory.store(0, std::byte(10)));
	ASSERT_NO_THROW(memory.store(1, std::byte(10)));
}

int main(int argc, char **argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}