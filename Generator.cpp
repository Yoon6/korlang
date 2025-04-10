#include <tuple>
#include <list>

#include "Node.h"
#include "Code.h"

static vector<Code> codeList;
static map<wstring, size_t> functionTable;

static list<map<wstring, size_t>> symbolStack;
static vector<size_t> offsetStack;
static size_t localSize = 0;

static vector<vector<size_t>> continueStack;
static vector<vector<size_t>> breakStack;

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

// Function
auto Function::generate() -> void {
    functionTable[name] = codeList.size();

    auto temp = writeCode(Instruction::Alloca);
    initBlock();
    for (auto& name : parameters)
    {
		setLocal(name);
    }

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
    expression->generate();
	writeCode(Instruction::Return);
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
    continueStack.emplace_back();
    breakStack.emplace_back();
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

    auto continueAddress = codeList.size(); // 증감식 주소
	expression->generate();
	writeCode(Instruction::PopOperand);
	writeCode(Instruction::Jump, jumpAddress);
	patchAddress(conditionJump);
	popBlock();

    for (auto& jump : continueStack.back())
    {
		patchOperand(jump, continueAddress);
    }
    for (auto& jump : breakStack.back())
    {
		patchAddress(jump);
    }
    continueStack.pop_back();
	breakStack.pop_back();
}

// Break
auto Break::generate() -> void {
	if (breakStack.empty())
	{
		return;
	}

    auto jumpCode = writeCode(Instruction::Jump);
	breakStack.back().push_back(jumpCode);
}

// Continue
auto Continue::generate() -> void {
    if (continueStack.empty())
    {
        return;
    }

    auto jumpCode = writeCode(Instruction::Jump);
	continueStack.back().push_back(jumpCode);
}

// If
auto If::generate() -> void {
    vector<size_t> jumpList;
    for (int i = 0; i < conditions.size(); i++)
    {
        conditions[i]->generate();
		auto conditionJump = writeCode(Instruction::ConditionJump);
        pushBlock();
		for (auto& node : blocks[i])
		{
			node->generate();
		}
        popBlock();
        jumpList.push_back(writeCode(Instruction::Jump));
		patchAddress(conditionJump);

    }
    if (!elseBlock.empty())
    {
        pushBlock();
        for (auto& node : elseBlock)
        {
            node->generate();
        }
        popBlock();
    }

	for (auto& jump : jumpList)
	{
		patchAddress(jump);
	}
}

// Print
auto Print::generate() -> void {
    for (int i = arguments.size(); i > 0; i--)
    {
        arguments[i - 1]->generate();
    }
    writeCode(Instruction::Print, arguments.size());
    if (isPrintLine)
    {
        writeCode(Instruction::PrintLine);
    }
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
	lhs->generate();
	auto LogicalAnd = writeCode(Instruction::LogicalAnd);
	rhs->generate();
	patchAddress(LogicalAnd);
}

// Relational
auto Relational::generate() -> void {
	map<Kind, Instruction> instructions = {
			{Kind::Equal, Instruction::Equal},
			{Kind::NotEqual, Instruction::NotEqual},
			{Kind::LessThan, Instruction::LessThan},
			{Kind::LessOrEqual, Instruction::LessOrEqual},
			{Kind::GreaterThan, Instruction::GreaterThan},
			{Kind::GreaterOrEqual, Instruction::GreaterOrEqual}
	};

    lhs->generate();
    rhs->generate();
    writeCode(instructions[kind]);
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
    map<Kind, Instruction> instructions = {
        {Kind::Add, Instruction::Absolute},
        {Kind::Subtract, Instruction::ReverseSign},
    };

    sub->generate();
    writeCode(instructions[kind]);
}

// Call
auto Call::generate() -> void {
	for (auto i = parameters.size(); i > 0; i--)
	{
		parameters[i - 1]->generate();
	}
	sub->generate();
	writeCode(Instruction::Call, parameters.size());

}

// GetElement
auto GetElement::generate() -> void {
	sub->generate();
	index->generate();
	writeCode(Instruction::GetElement);
}

// SetElement
auto SetElement::generate() -> void {
    value->generate();
    sub->generate();
	index->generate();
	writeCode(Instruction::SetElement);
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
    writeCode(Instruction::PushNull);
}

// BooleanLiteral
auto BooleanLiteral::generate() -> void {
    writeCode(Instruction::PushBoolean, value);
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
    for (int i = values.size(); i > 0; i--)
    {
		values[i - 1]->generate();
    }
	writeCode(Instruction::PushArray, values.size());
}

// MapLiteral
auto MapLiteral::generate() -> void {
    for (auto& [key, value] : values)
    {
        writeCode(Instruction::PushString, key);
		value->generate();
    }

    writeCode(Instruction::PushMap, values.size());
}


