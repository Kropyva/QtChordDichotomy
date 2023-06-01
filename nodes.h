#ifndef NODES_H
#define NODES_H

#include <QString>
#include <cmath>
#include <memory>

class Node {
public:
    virtual double evaluate() const = 0;
};

class NumberNode : public Node {
public:
    NumberNode(double value);

    double evaluate() const override;

private:
    double value;
};

class BinaryOpNode : public Node {
public:
    BinaryOpNode(QChar op, std::shared_ptr<Node> left, std::shared_ptr<Node> right);

    double evaluate() const override;

private:
    QChar op;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class NodeBuilder {
public:
    NodeBuilder(const QString& expression);

    std::shared_ptr<Node> buildAST(const double parameter);

private:
    std::shared_ptr<Node> parseExpression();

    std::shared_ptr<Node> parseTerm();

    std::shared_ptr<Node> parseFactor();

    double parseNumber();

    QChar peek() const;

    QChar advance();

    bool match(QChar expected);

    double parameter;
    QString expression;
    qsizetype index;
};

#endif // NODES_H
