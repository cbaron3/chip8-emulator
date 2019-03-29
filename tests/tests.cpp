#include "../src/Logger.cpp"
#include "../src/Memory.cpp"
#include <gtest/gtest.h>


// Test to make sure the byte we store is actually stored properly
TEST(MemoryTest, SingleStoreThenRead){
	util::Logger::getInstance()->set_max_log_level(LOGTYPE::NONE);

	chip8::Memory memory;
	memory.store_byte(1);
	ASSERT_EQ(1, memory.read_byte(memory.get_memory_pointer()-1));

	memory.store_byte(0xFF);
	ASSERT_EQ(0xFF, memory.read_byte(memory.get_memory_pointer()-1));
}

// Test to make sure we cannot access a value outside of the memory space
TEST(MemoryTest, AccessibleAddress){
	util::Logger::getInstance()->set_max_log_level(LOGTYPE::NONE);

	chip8::Memory memory;
	ASSERT_ANY_THROW(memory.read_byte(-1));
	ASSERT_ANY_THROW(memory.read_byte(chip8::MEM_SPACE));
	ASSERT_NO_THROW(memory.read_byte(chip8::MEM_SPACE-1));
}

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