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
TEST_F(Chip8CPU, subroutine_test)
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
TEST_F(Chip8CPU, set_pc_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Set new pc and validate current pc gets updated
    unsigned int temp_pc = cpu->get_pc() + 0x100;
    cpu->execute(set_pc_call(temp_pc));
    ASSERT_EQ(cpu->get_pc(), temp_pc);
}

// Function to test load byte into vx opcode 6xkk
TEST_F(Chip8CPU, ld_vx_byte_test)
{
    // For opcode simulators
    using namespace chip8::util;

    unsigned int vx = 5, byte = 10;
    unsigned int ld_val = cpu->registers[vx] + byte;

    cpu->execute(set_reg_call(vx, byte));

    ASSERT_NE(0, cpu->registers[vx]);

    ASSERT_EQ(ld_val, cpu->registers[vx]);
}

// Function to test load byte into vx opcode 6xkk
TEST_F(Chip8CPU, add_vx_byte_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Test 1
    unsigned int vx = 9, add = 100;
    unsigned int org_val = cpu->registers[9];

    cpu->execute(add_to_reg_call(vx, add));

    ASSERT_NE(0, cpu->registers[vx]);

    ASSERT_EQ(org_val+add, cpu->registers[vx]); 
}

// Function to test load vy into vx opcode 8xy0
TEST_F(Chip8CPU, ld_vx_vy_test)
{
    // For opcode simulators
    using namespace chip8::util;

    unsigned int vx = 2, vy = 4;
    unsigned int org_vy = (cpu->registers[vy] = 10);    // Make sure vy is a non zero before updating vx

    cpu->execute(set_reg_equal_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);

    ASSERT_EQ(org_vy, cpu->registers[vx]);   
    ASSERT_EQ((org_vy), cpu->registers[vy]); 
}

// Function to test load vx | vy into vx opcode 8xy1
TEST_F(Chip8CPU, or_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 2, vy = 4;

    unsigned int org_vx = (cpu->registers[vx] = 12);
    unsigned int org_vy = (cpu->registers[vy] = 15);

    cpu->execute(or_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);

    ASSERT_EQ((org_vx | org_vy), cpu->registers[vx]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);
}

// Function to test load vx & vy into vx opcode 8xy2
TEST_F(Chip8CPU, and_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 3, vy = 5;

    unsigned int org_vx = (cpu->registers[vx] = 4);
    unsigned int org_vy = (cpu->registers[vy] = 12);

    cpu->execute(and_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);

    ASSERT_EQ((org_vx & org_vy), cpu->registers[vx]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);
}

// Function to test load vx ^ vy into vx opcode 8xy3
TEST_F(Chip8CPU, xor_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 4, vy = 6;

    unsigned int org_vx = (cpu->registers[vx] = 2);
    unsigned int org_vy = (cpu->registers[vy] = 99);

    cpu->execute(xor_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx ^ org_vy), cpu->registers[vx]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);
}

// Function to test load vx + vy into vx opcode 8xy4
// 1. Regular addition (carry not set)
// 2. Overflow addition (carry set)
TEST_F(Chip8CPU, add_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12, vy = 13;

    unsigned int org_vx = (cpu->registers[vx] = 50);
    unsigned int org_vy = (cpu->registers[vy] = 25);

    cpu->execute(add_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx + org_vy) & 0xFF, cpu->registers[vx]);
    ASSERT_EQ(0, cpu->registers[15]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);

    org_vx = (cpu->registers[vx] = 100);
    org_vy = (cpu->registers[vy] = 200);

    cpu->execute(add_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx + org_vy) & 0xFF, cpu->registers[vx]);
    ASSERT_EQ(1, cpu->registers[15]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);
}

// Function to test load vx - vy into vx opcode 8xy5
// 1. Regular subtraction (carry not set)
// 2. Underflow subtraction (carry set)
TEST_F(Chip8CPU, sub_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 0, vy = 1;

    unsigned int org_vx = (cpu->registers[vx] = 60);
    unsigned int org_vy = (cpu->registers[vy] = 30);

    cpu->execute(sub_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx - org_vy) & 0xFF, cpu->registers[vx]);
    ASSERT_EQ(0, cpu->registers[15]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);

    org_vx = (cpu->registers[vx] = 100);
    org_vy = (cpu->registers[vy] = 105);

    cpu->execute(sub_reg_call(vx, vy));

    ASSERT_EQ( (org_vx - org_vy) & 0xFF, cpu->registers[vx]);
    ASSERT_EQ(1, cpu->registers[15]);
    ASSERT_EQ((org_vy), cpu->registers[vy]);
}

// Function to test load vx >> 1 into vx opcode 8xy6
TEST_F(Chip8CPU, shr_vx_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12;
    unsigned int org_vx = (cpu->registers[vx] = 6);

    cpu->execute(shr_reg_call(vx));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx >> 1), cpu->registers[vx]);
    ASSERT_EQ(0, cpu->registers[15]);

    org_vx = (cpu->registers[vx] = 5);
    cpu->execute(shr_reg_call(vx));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx >> 1), cpu->registers[vx]);
    ASSERT_EQ(1, cpu->registers[15]);
}

// Function to test load vy - vx into vx opcode 8xy7
// 1. Regular subtraction (carry not set)
// 2. Underflow subtraction (carry set)
TEST_F(Chip8CPU, subn_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 0, vy = 1;

    unsigned int org_vx = (cpu->registers[vx] = 30);
    unsigned int org_vy = (cpu->registers[vy] = 60);

    cpu->execute(subn_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vy-org_vx) & 0xFF, cpu->registers[vx]);
    ASSERT_EQ(1, cpu->registers[15]);

    org_vx = (cpu->registers[vx] = 60);
    org_vy = (cpu->registers[vy] = 30);

    cpu->execute(subn_reg_call(vx, vy));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vy-org_vx) & 0xFF, cpu->registers[vx]);
    ASSERT_EQ(0, cpu->registers[15]);
}

// Function to test load vx << 1 into vx opcode 8xyE
TEST_F(Chip8CPU, shl_vx_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12;
    unsigned int org_vx = (cpu->registers[vx] = 127);

    cpu->execute(shl_reg_call(vx));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx << 1), cpu->registers[vx]);
    ASSERT_EQ(0, cpu->registers[15]);

    org_vx = (cpu->registers[vx] = 129);
    cpu->execute(shl_reg_call(vx));

    ASSERT_NE(0, cpu->registers[vx]);
    ASSERT_EQ((org_vx << 1), cpu->registers[vx]);
    ASSERT_EQ(1, cpu->registers[15]);
}


// Function to test skip instruction opcodes
// 1. 3xkk
// 2. 4xkk
// 3. 5xy0
// 4. 9xy0
TEST_F(Chip8CPU, skip_instruct_test)
{

}