#include <nodes.h>

NumberNode::NumberNode(double value)
    : value(value)
{
}

double NumberNode::evaluate() const
{
    return value;
}

BinaryOpNode::BinaryOpNode(QChar op, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
    : op(op)
    , left(left)
    , right(right)
{
}

double BinaryOpNode::evaluate() const
{
    switch (op.toLatin1()) {
    case '+':
        return left->evaluate() + right->evaluate();
    case '-':
        return left->evaluate() - right->evaluate();
    case '*':
        return left->evaluate() * right->evaluate();
    case '/':
        return left->evaluate() / right->evaluate();
    case '^':
        return std::pow(left->evaluate(), right->evaluate());
    default:
        throw QString("Invalid operator");
    }
}

NodeBuilder::NodeBuilder(const QString& expression)
    : parameter(0)
    , expression(expression)
    , index(0)
{
}

std::shared_ptr<Node> NodeBuilder::buildAST(const double parameter)
{
    this->parameter = parameter;
    return parseExpression();
}

std::shared_ptr<Node> NodeBuilder::parseExpression()
{
    auto left = parseTerm();
    while (match('+') || match('-')) {
        QChar op = expression[index - 1];
        auto right = parseTerm();
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::shared_ptr<Node> NodeBuilder::parseTerm()
{
    auto left = parseFactor();
    while (match('*') || match('/') || match('^')) {
        QChar op = expression[index - 1];
        auto right = parseFactor();
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::shared_ptr<Node> NodeBuilder::parseFactor()
{
    if (peek().isDigit()) {
        double value = parseNumber();
        return std::make_shared<NumberNode>(value);
    } else if (match('x')) {
        return std::make_shared<NumberNode>(parameter);
    } else if (match('(')) {
        auto node = parseExpression();
        if (!match(')')) {
            throw QString("Missing closing parenthesis.");
        }
        return node;
    } else {
        throw QString("Invalid expression.");
    }
}

double NodeBuilder::parseNumber()
{
    QString number;
    while (peek().isDigit() || peek() == '.') {
        number += advance();
    }
    return number.toDouble();
}

QChar NodeBuilder::peek() const
{
    if (index < expression.size()) {
        return expression[index];
    }
    return '\0';
}

QChar NodeBuilder::advance()
{
    if (index < expression.size()) {
        return expression[index++];
    }
    return '\0';
}

bool NodeBuilder::match(QChar expected)
{
    if (peek() == expected) {
        advance();
        return true;
    }
    return false;
}
