#include <tuple>
#include <list>

#include "Node.h"
#include "Code.h"

static vector<Code> codeList;
static map<wstring, size_t> functionTable;

static list<map<wstring, size_t>> symbolStack;
static vector<size_t> offsetStack;
static size_t localSize = 0;

auto writeCode(Instruction instruction) -> size_t {

    codeList.push_back({ instruction });
    return codeList.size() - 1;

}

auto writeCode(Instruction instruction, any operand) -> size_t {

    codeList.push_back({ instruction, operand });
    return codeList.size() - 1;

}

auto generate(Program* program) -> tuple<vector<Code>, map<wstring, size_t>>
{
    wstring main = L"시작";
    writeCode(Instruction::GetGlobal, main);
    writeCode(Instruction::Call, static_cast<size_t>(0));
    writeCode(Instruction::Exit);

    for (auto& node : program->functions)
    {
        node->generate();
    }

    return {codeList, functionTable};
}

auto patchAddress(size_t codeIndex) -> void 
{
	codeList[codeIndex].operand = codeList.size();
}

auto patchOperand(size_t codeIndex, size_t operand) -> void
{
	codeList[codeIndex].operand = operand;
}

auto initBlock() -> void
{
    localSize = 0;
	offsetStack.push_back(0);
    symbolStack.emplace_front();
}

auto popBlock() -> void
{
	offsetStack.pop_back();
	symbolStack.pop_front();
}

// Function
auto Function::generate() -> void {
    functionTable[name] = codeList.size();

    auto temp = writeCode(Instruction::Alloca);
    initBlock();
    for (auto& node : blocks)
    {
        node->generate();
    }
    popBlock();
    patchOperand(temp, localSize);

    writeCode(Instruction::Return);
}

// Return
auto Return::generate() -> void {
    // TODO: Implement Return generation logic
}

auto setLocal(wstring name) -> void
{
    symbolStack.front()[name] = offsetStack.back();
    offsetStack.back() += 1;
	localSize = max(localSize, offsetStack.back());
}

auto getLocal(wstring name) -> size_t
{
	for (auto& symbolTable : symbolStack)
	{
        if (symbolTable.count(name))
        {
            return symbolTable[name];
        }
	}
	return SIZE_MAX;
}

// Variable
auto Variable::generate() -> void {
    setLocal(name);
    expression->generate();
	writeCode(Instruction::SetLocal, getLocal(name));
    writeCode(Instruction::PopOperand);
}

auto pushBlock() -> void
{
	symbolStack.emplace_front();
	offsetStack.push_back(offsetStack.back());
}

// For
auto For::generate() -> void {
    pushBlock();
    variable->generate();
    auto jumpAddress = codeList.size();
	condition->generate();

    auto conditionJump = writeCode(Instruction::ConditionJump);

    // 참인 경우에 실행할 코드
    for (auto& node : blocks)
    {
		node->generate();
    }

	expression->generate();
	writeCode(Instruction::PopOperand);
	writeCode(Instruction::Jump, jumpAddress);
	patchAddress(conditionJump);
	popBlock();
}

// Break
auto Break::generate() -> void {
    // TODO: Implement Break generation logic
}

// Continue
auto Continue::generate() -> void {
    // TODO: Implement Continue generation logic
}

// If
auto If::generate() -> void {
    // TODO: Implement If generation logic
}

// Print
auto Print::generate() -> void {
    for (int i = arguments.size(); i >= 0; i--)
    {
        arguments[i - 1]->generate();
    }
    writeCode(Instruction::Print, arguments.size());
}

// ExpressionStatement
auto ExpressionStatement::generate() -> void {
    expression->generate();
    writeCode(Instruction::PopOperand);
}

// Or
auto Or::generate() -> void {
    lhs->generate();
    auto LogicalOr = writeCode(Instruction::LogicalOr);
    rhs->generate();
    patchAddress(LogicalOr);
}

// And
auto And::generate() -> void {
    // TODO: Implement And generation logic
}

// Relational
auto Relational::generate() -> void {
    // TODO: Implement Relational generation logic
}

// Arithmetic
auto Arithmetic::generate() -> void {
    map<Kind, Instruction> instructions = {
            {Kind::Add, Instruction::Add},
            {Kind::Subtract, Instruction::Subtract},
            {Kind::Multiply, Instruction::Multiply},
            {Kind::Divide, Instruction::Divide},
            {Kind::Modulo, Instruction::Modulo}
    };

    lhs->generate();
    rhs->generate();
    writeCode(instructions[kind]);
}

// Unary
auto Unary::generate() -> void {
    // TODO: Implement Unary generation logic
}

// Call
auto Call::generate() -> void {
    // TODO: Implement Call generation logic
}

// GetElement
auto GetElement::generate() -> void {
    // TODO: Implement GetElement generation logic
}

// SetElement
auto SetElement::generate() -> void {
    // TODO: Implement SetElement generation logic
}

// GetVariable
auto GetVariable::generate() -> void {
	if (getLocal(name) != SIZE_MAX)
	{
		writeCode(Instruction::GetLocal, getLocal(name));
	}
	else
	{
		writeCode(Instruction::GetGlobal, name);
	}
}

// SetVariable
auto SetVariable::generate() -> void {
    value->generate();
    if (getLocal(name) != SIZE_MAX)
    {
		writeCode(Instruction::SetLocal, getLocal(name));
    }
    else
    {
        writeCode(Instruction::SetGlobal, name);
    }
}

// NullLiteral
auto NullLiteral::generate() -> void {
    // TODO: Implement NullLiteral generation logic
}

// BooleanLiteral
auto BooleanLiteral::generate() -> void {
    // TODO: Implement BooleanLiteral generation logic
}

// NumberLiteral
auto NumberLiteral::generate() -> void {
    writeCode(Instruction::PushNumber, value);
}

// StringLiteral
auto StringLiteral::generate() -> void {
    writeCode(Instruction::PushString, value);
}

// ArrayLiteral
auto ArrayLiteral::generate() -> void {
    // TODO: Implement ArrayLiteral generation logic
}

// MapLiteral
auto MapLiteral::generate() -> void {
    // TODO: Implement MapLiteral generation logic
}


