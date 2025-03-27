//
// Created by yoon on 3/28/25.
//

#include <vector>
#include <map>
#include <any>

#include "Code.h"

using namespace std;

struct StackFrame {
    vector<any> variables;
    vector<any> operandStack;
    size_t instructionPointer = 0;
};

static vector<StackFrame> callStack;

void execute(vector<Code> codeList, map<wstring, size_t> functionTables)
{
    callStack.emplace_back();
    while (true)
    {
        auto code = codeList[callStack.back().instructionPointer];
        switch (code.instruction)
        {
            case Instruction::Exit:
                callStack.pop_back();
                return;
            case Instruction::Call:
                break;
            case Instruction::Alloca:
                break;
            case Instruction::Return:
                break;
            case Instruction::Jump:
                break;
            case Instruction::ConditionJump:
                break;
            case Instruction::Print:
                break;
            case Instruction::LogicalOr:
                break;
            case Instruction::LogicalAnd:
                break;
            case Instruction::Add:
                break;
            case Instruction::Subtract:
                break;
            case Instruction::Multiply:
                break;
            case Instruction::Divide:
                break;
            case Instruction::Modulo:
                break;
            case Instruction::Equal:
                break;
            case Instruction::NotEqual:
                break;
            case Instruction::LessThan:
                break;
            case Instruction::GreaterThan:
                break;
            case Instruction::LessOrEqual:
                break;
            case Instruction::GreaterOrEqual:
                break;
            case Instruction::Absolute:
                break;
            case Instruction::ReverseSign:
                break;
            case Instruction::GetElement:
                break;
            case Instruction::SetElement:
                break;
            case Instruction::GetGlobal:
                break;
            case Instruction::SetGlobal:
                break;
            case Instruction::GetLocal:
                break;
            case Instruction::SetLocal:
                break;
            case Instruction::PushNull:
                break;
            case Instruction::PushBoolean:
                break;
            case Instruction::PushNumber:
                break;
            case Instruction::PushString:
                break;
            case Instruction::PushArray:
                break;
            case Instruction::PushMap:
                break;
            case Instruction::PopOperand:
                break;
        }
        callStack.back().instructionPointer += 1;
    }
}
