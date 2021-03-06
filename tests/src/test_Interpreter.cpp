#define private public

#include "../../include/Interpreter.h"
#include "../../src/Interpreter.cpp"
#include "../../src/Logger.cpp"
#include "GenerateOpcodes.hpp"


class MockMemory: public chip8::MemoryMap
{
public:
    MockMemory() {}

    /*****************************************
     * Dequeue()
     * @brief   Mock Method of BinaryQueue::Dequeue
     *
     * @details Mock Method of BinaryQueue::Dequeue. The original function being mocked:
     *          virtual bool Dequeue(void *item, TickType_t Timeout = portMAX_DELAY)
     *****************************************/
    MOCK_METHOD3(store, bool(const std::byte& val, const unsigned int& adr, const bool& update));
    MOCK_METHOD1(read, std::byte(const unsigned int& adr));

};

class Chip8CPU : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Disable logging for tests
        util::Logger::get_instance()->set_max_log_level(LOGTYPE::NONE); 
    }

    std::unique_ptr<chip8::Interpreter> interpreter = chip8::Interpreter::make_interpreter(std::make_unique<MockMemory>());
};

// Function to test clear screen opcode
// 1. Set screen to non-zero state then call clear screen opcode. Pixel array should be all 0s
TEST_F(Chip8CPU, ClearScreenTest)
{
    // For opcode simulators
    using namespace chip8::util;
    
    interpreter->execute(clear_scr_call());

    // Check pixel array
    std::array< unsigned int, 64 * 32> pixels = interpreter->screen();
    for (auto & pixel : pixels)  
    {
        ASSERT_EQ( 0 , pixel ) << "There exists a non-false pixel in the array";
    }
}

// Functions to test subroutine opcodes
// 1. Return from a subroutine call with an empty stack should set exit flag. Stack underflow
TEST_F(Chip8CPU, empty_stack_subroutine_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Test 1. Return from subroutine with empty stack.
    ASSERT_EQ(false, interpreter->exit()) << "Exit flag is wrong";
    interpreter->execute( ret_subr_call() );
    ASSERT_EQ(true, interpreter->exit()) << "Exit flag is wrong";
}

// Functions to test subroutine opcodes
// 2. Calling a subroutine puts the current PC onto the stack and updates the PC to the requested value
// Return from a subroutine should reset the PC back to the original PC pushed on to the stack
TEST_F(Chip8CPU, simple_subroutine_push_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // subr_call adds current pc to stack, then updates program counter to param
    unsigned int stack_pc = interpreter->m_program_counter;
    interpreter->execute(subr_call(0x345));
    ASSERT_EQ(false, interpreter->exit()) << "Stack should not overflow, exit flag is wrong";
    
    // Make sure pc is update after subr call
    ASSERT_EQ(interpreter->m_program_counter, 0x345) << "Program counter is wrong";

    // Return from subroutine should change pc back to original pc and should not cause stack underflow
    interpreter->execute(ret_subr_call());
    ASSERT_EQ(interpreter->m_program_counter, stack_pc ) << "Program counter is wrong";
    ASSERT_EQ(false, interpreter->exit()) << "Stack should not underflow, exit flag is wrong";
}

// Functions to test subroutine opcodes
// 3. Adding 17 addresses to the stack should result in stack overflow
TEST_F(Chip8CPU, full_stack_subroutine_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Test 4. Add 17 stack entries to overflow stack.
    for(int i = 0; i < 17; i++)
    {
        interpreter->execute(subr_call(0x100));
    }

    ASSERT_EQ(true, interpreter->exit()) << "Exit flag is wrong";
}

// Function to test setting program counter opcode
TEST_F(Chip8CPU, set_pc_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Set new pc and validate current pc gets updated
    unsigned int temp_pc = interpreter->m_program_counter + 0x100;
    interpreter->execute(set_pc_call(temp_pc));
    ASSERT_EQ(interpreter->m_program_counter, temp_pc);
}

// Function to test load byte into vx opcode 6xkk
TEST_F(Chip8CPU, ld_vx_byte_test)
{
    // For opcode simulators
    using namespace chip8::util;

    unsigned int vx = 5, byte = 10;
    unsigned int ld_val = interpreter->m_registers[vx] + byte;

    interpreter->execute(set_reg_call(vx, byte));

    ASSERT_NE(0, interpreter->m_registers[vx]);

    ASSERT_EQ(ld_val, interpreter->m_registers[vx]);
}

// Function to test load byte into vx opcode 6xkk
TEST_F(Chip8CPU, add_vx_byte_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Test 1
    unsigned int vx = 9, add = 100;
    unsigned int org_val = interpreter->m_registers[9];

    interpreter->execute(add_to_reg_call(vx, add));

    ASSERT_NE(0, interpreter->m_registers[vx]);

    ASSERT_EQ(org_val+add, interpreter->m_registers[vx]); 
}

// Function to test load vy into vx opcode 8xy0
TEST_F(Chip8CPU, ld_vx_vy_test)
{
    // For opcode simulators
    using namespace chip8::util;

    unsigned int vx = 2, vy = 4;

    // First load a non-zero value
    interpreter->execute(set_reg_call(vy, 10));
    unsigned int org_vy = (interpreter->m_registers[vy]);    // Make sure vy is a non zero before updating vx

    interpreter->execute(set_reg_equal_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);

    ASSERT_EQ(org_vy, interpreter->m_registers[vx]);   
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]); 
}

// Function to test load vx | vy into vx opcode 8xy1
TEST_F(Chip8CPU, or_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 2, vy = 4;

    // Load non-zero bytes for operation testing
    interpreter->execute(set_reg_call(vx, 12));
    unsigned int org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 15));
    unsigned int org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(or_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);

    ASSERT_EQ((org_vx | org_vy), interpreter->m_registers[vx]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);
}

// Function to test load vx & vy into vx opcode 8xy2
TEST_F(Chip8CPU, and_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 3, vy = 5;

    interpreter->execute(set_reg_call(vx, 12));
    unsigned int org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 12));
    unsigned int org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(and_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);

    ASSERT_EQ((org_vx & org_vy), interpreter->m_registers[vx]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);
}

// Function to test load vx ^ vy into vx opcode 8xy3
TEST_F(Chip8CPU, xor_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 4, vy = 6;

    interpreter->execute(set_reg_call(vx, 2));
    unsigned int org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 99));
    unsigned int org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(xor_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx ^ org_vy), interpreter->m_registers[vx]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);
}

// Function to test load vx + vy into vx opcode 8xy4
// 1. Regular addition (carry not set)
// 2. Overflow addition (carry set)
TEST_F(Chip8CPU, add_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12, vy = 13;

    interpreter->execute(set_reg_call(vx, 50));
    unsigned int org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 25));
    unsigned int org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(add_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx + org_vy) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(0, interpreter->m_registers[15]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);

    interpreter->execute(set_reg_call(vx, 100));
    org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 200));
    org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(add_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx + org_vy) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(1, interpreter->m_registers[15]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);
}

// Function to test load vx - vy into vx opcode 8xy5
// 1. Regular subtraction (carry not set)
// 2. Underflow subtraction (carry set)
TEST_F(Chip8CPU, sub_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 0, vy = 1;

    // Vx set to 60, Vy set to 30
    interpreter->execute(set_reg_call(vx, 60));
    unsigned int org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 30));
    unsigned int org_vy = (interpreter->m_registers[vy]);

    // Execute sub VX, VY
    interpreter->execute(sub_reg_call(vx, vy));

    // Vx should be equal to Vx - Vy and carry flag should be set cause no borrow
    // Vy unchanged
    ASSERT_EQ((org_vx - org_vy) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(1, interpreter->m_registers[15]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);

    // Vx set to 100, Vy to 105
    interpreter->execute(set_reg_call(vx, 100));
    org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 105));
    org_vy = (interpreter->m_registers[vy]);
    interpreter->execute(sub_reg_call(vx, vy));

    // Vx should be Vx-Vy and carry flag should not be set cause borrow
    // Vy unchanged
    ASSERT_EQ( (org_vx - org_vy) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(0, interpreter->m_registers[15]);
    ASSERT_EQ((org_vy), interpreter->m_registers[vy]);
}

// Function to test load vx >> 1 into vx opcode 8xy6
TEST_F(Chip8CPU, shr_vx_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12;
    interpreter->execute(set_reg_call(vx, 6));
    unsigned int org_vx = (interpreter->m_registers[vx]);

    interpreter->execute(shr_reg_call(vx));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx >> 1), interpreter->m_registers[vx]);
    ASSERT_EQ(0, interpreter->m_registers[15]);

    interpreter->execute(set_reg_call(vx, 5));
    org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(shr_reg_call(vx));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx >> 1), interpreter->m_registers[vx]);
    ASSERT_EQ(1, interpreter->m_registers[15]);
}

// Function to test load vy - vx into vx opcode 8xy7
// 1. Regular subtraction (carry not set)
// 2. Underflow subtraction (carry set)
TEST_F(Chip8CPU, subn_vx_vy_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 0, vy = 1;

    interpreter->execute(set_reg_call(vx, 30));
    unsigned int org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 60));
    unsigned int org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(subn_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vy-org_vx) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(1, interpreter->m_registers[15]);

    interpreter->execute(set_reg_call(vx, 60));
    org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(set_reg_call(vy, 30));
    org_vy = (interpreter->m_registers[vy]);

    interpreter->execute(subn_reg_call(vx, vy));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vy-org_vx) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(0, interpreter->m_registers[15]);
}

// Function to test load vx << 1 into vx opcode 8xyE
TEST_F(Chip8CPU, shl_vx_test)
{
    // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12;
    interpreter->execute(set_reg_call(vx, 127));
    unsigned int org_vx = (interpreter->m_registers[vx]);

    interpreter->execute(shl_reg_call(vx));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx << 1) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(0, interpreter->m_registers[15]);

    interpreter->execute(set_reg_call(vx, 129));
    org_vx = (interpreter->m_registers[vx]);
    interpreter->execute(shl_reg_call(vx));

    ASSERT_NE(0, interpreter->m_registers[vx]);
    ASSERT_EQ((org_vx << 1) & 0xFF, interpreter->m_registers[vx]);
    ASSERT_EQ(1, interpreter->m_registers[15]);
}

// Functions to test skip instruction if Vx == kk 3xkk
TEST_F(Chip8CPU, skip_instruct_eq_test)
{
        // For opcode generator
    using namespace chip8::util;

    unsigned int vx = 12;
    unsigned int pc = interpreter->m_program_counter;

    // Execute does not change program counter
    // So skip should inc pc by 2
    interpreter->execute(set_reg_call(vx, 127));
    interpreter->execute(skip_instr_ifeq_call(vx,127));

    ASSERT_EQ(pc+2, interpreter->m_program_counter);
}

// Functions to test skip instruction if Vx == kk 4xkk
TEST_F(Chip8CPU, skip_instruct_neq_test)
{
    // For opcode generators
    using namespace chip8::util;

    unsigned int vx = 12;
    unsigned int pc = interpreter->m_program_counter;

    // Execute does not change program counter
    // So skip should inc pc by 2
    interpreter->execute(set_reg_call(vx, 127));
    interpreter->execute(skip_instr_ifneq_call(vx,126));

    ASSERT_EQ(pc+2, interpreter->m_program_counter);
}

// Functions to test skip instruction if Vx == Vy 5xy0
TEST_F(Chip8CPU, skip_instruct_reg_eq_test)
{
    // For opcode generators
    using namespace chip8::util;

    unsigned int vx = 1, vy = 2;
    unsigned int pc = interpreter->m_program_counter;

    interpreter->execute(set_reg_call(vx, 5));
    interpreter->execute(set_reg_call(vy, 5));

    interpreter->execute(skip_instr_ifeq_reg_call(vx,vy));

    ASSERT_EQ(pc+2, interpreter->m_program_counter);
}


// Functions to test skip instruction if Vx != Vy 9xy0
TEST_F(Chip8CPU, skip_instruct_reg_neq_test)
{
    // For opcode generators
    using namespace chip8::util;

    unsigned int vx = 1, vy = 2;
    unsigned int pc = interpreter->m_program_counter;

    interpreter->execute(set_reg_call(vx, 5));
    interpreter->execute(set_reg_call(vy, 4));

    interpreter->execute(skip_instr_ifneq_reg_call(vx,vy));

    ASSERT_EQ(pc+2, interpreter->m_program_counter);
}

// Function to test setting index register opcode Annn
TEST_F(Chip8CPU, set_ir_test)
{
    // For opcode simulators
    using namespace chip8::util;

    // Set new pc and validate current pc gets updated
    unsigned int temp_ir = interpreter->m_index_register + 0x100;
    interpreter->execute(set_i_call(0x100));
    ASSERT_EQ(interpreter->m_index_register, temp_ir);
}

// Function to test setting index register opcode Annn
TEST_F(Chip8CPU, jump_ir_test)
{
    // For opcode simulators
    using namespace chip8::util;

    interpreter->execute(set_reg_call(0, 10));

    // Set new pc and validate current pc gets updated
    unsigned int temp_pc = interpreter->m_program_counter;

    interpreter->execute(jump_pc_call(temp_pc+0x100));

    ASSERT_NE(interpreter->m_program_counter, temp_pc+10);
    ASSERT_EQ(interpreter->m_program_counter, temp_pc+10+0x100);
}

// Function to test random setting of vx AND kk Cxkk
// TODO: How do I test this better
TEST_F(Chip8CPU, rand_vx_test)
{
    // For opcode generators
    using namespace chip8::util;

    unsigned int vx = 6;

    interpreter->execute(rand_reg_call(vx, 100));

    unsigned int a = interpreter->m_registers[vx];

    ASSERT_TRUE((a >= 0) && (a <= 255));
}

// Function to test sound and delay timer opcodes Fx07, Fx15, Fx18
TEST_F(Chip8CPU, timer_test)
{
    // For opcode generators
    using namespace chip8::util;

    // Set delay timer to vx value
    // Set different vx to delay timer
    // Set that new vx as sound timer

    unsigned int vx = 5;
    unsigned int org_vx_val = 10;
    unsigned int org_delay = interpreter->delay();

    interpreter->execute(set_reg_call(vx, org_vx_val));
    interpreter->execute(delay_eq_vx_call(vx));
    ASSERT_NE(interpreter->delay(), org_delay);
    ASSERT_EQ(interpreter->delay(), org_vx_val);

    vx = 8;
    unsigned org_val = interpreter->m_registers[vx];
    interpreter->execute(vx_eq_delay_call(vx));
    ASSERT_NE(interpreter->m_registers[vx], org_val);
    ASSERT_EQ(interpreter->m_registers[vx], org_vx_val);

    unsigned org_sound = interpreter->sound();
    interpreter->execute(sound_eq_vx_call(vx));
    ASSERT_NE(interpreter->sound(), org_sound);
    ASSERT_EQ(interpreter->sound(), org_vx_val);
} 

// Function to test opcode that adds vx to index register Fx1E
TEST_F(Chip8CPU, add_ir_vx_test)
{
    // Using opcode generator
    using namespace chip8::util;

    unsigned int vx = 9;
    interpreter->execute(set_reg_call(vx, 100));

    unsigned int org_ir = interpreter->m_index_register;
    interpreter->execute(index_add_reg_call(vx));

    ASSERT_NE(interpreter->m_index_register, org_ir);
    ASSERT_EQ(interpreter->m_index_register, org_ir+100);
}

// TODO: Skipped D and E opcode tests
// TODO: Skipped Fx0A, Fx29, Fx33, Fx55, Fx65