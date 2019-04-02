#include "../src/CPU.cpp"
#include "../src/Logger.cpp"
#include "../include/GenerateOpcodes.hpp"

class Chip8CPU : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Disable logging for tests
        util::Logger::getInstance()->set_max_log_level(LOGTYPE::NONE); 
    }

    void TearDown() override
    {

    }

    std::unique_ptr<chip8::CPU> cpu = chip8::CPU::makeCPU();
};

// TODO: Clear screen test
// Function to test clear screen opcode
// 1. Set screen to non-zero state then call clear screen opcode. Pixel array should be all 0s
TEST_F(Chip8CPU, ClearScreenTest)
{
    // Test 1. Clear screen.
    // Set array to non zero state

    // Check pixel array
    std::array<std::array<bool, 64>, 32> pixels = cpu->get_pixels();
    for (auto & outer_array : pixels)  
    {
        for(auto & inner_array : outer_array)
        {
            ASSERT_EQ(false, inner_array) << "There exists a non-false pixel in the array";
        }
    }

}

// Function to test subroutine opcodes
// 1. Return from a subroutine call with an empty stack should set exit flag. Stack underflow
// 2. Adding an address to the stack should result in the prog counter being updated to that same value after a subroutine return
// 3. Adding 3 different addresses should result in the same 3 address being returned in LIFO order
// 4. Adding 17 addresses to the stack should result in stack overflow
TEST_F(Chip8CPU, SubroutineTest)
{
    // For opcode simulators
    using namespace chip8::util;

    // Test 1. Return from subroutine with empty stack.
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->execute(ret_subr_call());
    ASSERT_EQ(true, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->set_exit_flag(false);                  // Force exit flag reset

    // Test 2. Add stack entry and make sure same value is returned as program counter.
    cpu->execute(subr_call(0x345));
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->execute(ret_subr_call());
    ASSERT_EQ(cpu->get_pc(), 0x345) << "Program counter is wrong";
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->set_exit_flag(false);                  // Force exit flag reset

    // Test 3. Add 3 stack entries and make sure they return in LIFO order.
    cpu->execute(subr_call(0x100));
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->execute(subr_call(0x200));
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->execute(subr_call(0x300));
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";
    cpu->set_exit_flag(false);                  // Force exit flag reset

    cpu->execute(ret_subr_call());
    ASSERT_EQ(cpu->get_pc(), 0x300) << "Program counter is wrong";
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";

    cpu->execute(ret_subr_call());
    ASSERT_EQ(cpu->get_pc(), 0x200) << "Program counter is wrong";
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong"; 

    cpu->execute(ret_subr_call());
    ASSERT_EQ(cpu->get_pc(), 0x100) << "Program counter is wrong";
    ASSERT_EQ(false, cpu->get_exit_flag()) << "Exit flag is wrong";     

    // Test 4. Add 17 stack entries to overflow stack.
    for(int i = 0; i < 17; i++)
    {
        cpu->execute(subr_call(0x100));
    }

    ASSERT_EQ(true, cpu->get_exit_flag()) << "Exit flag is wrong";
}

// Function to test setting program counter opcode
// 1. Get current program counter, set the pc to the current value + an offset. Verify change
TEST_F(Chip8CPU, SetPCTest)
{
    // For opcode simulators
    using namespace chip8::util;

    // Set new pc and validate current pc gets updated
    unsigned int temp_pc = cpu->get_pc() + 0x100;
    cpu->execute(set_pc_call(temp_pc));
    ASSERT_EQ(cpu->get_pc(), temp_pc);
}

// Opcodes to set registers
// 6xkk
// 7xkk
// 8***
// Cxkk
// Fx07
TEST_F(Chip8CPU, SetRegisterTest)
{

}

// TODO: Need to test setting registers first
// Function to test skip instruction opcodes
// 1. 3xkk
// 2. 4xkk
// 3. 5xy0
// 4. 9xy0
TEST_F(Chip8CPU, SkipInstructTest)
{

}