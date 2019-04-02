#include "../src/Memory.cpp"

TEST(MemoryTest, MemoryMap)
{
	std::unique_ptr<chip8::MemoryMap> memory = chip8::MemoryMap::makeMemoryMap(10, 1);
	ASSERT_ANY_THROW(memory->store(std::byte(10),0) );
	ASSERT_NO_THROW(memory->store(std::byte(10), 1));
}