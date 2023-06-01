#include "operations.h"
#include "nodes.h"
#include <QVector>

double fn(QString& expression, double value = 0)
{
    try {
        NodeBuilder builder(expression);
        return builder.buildAST(value)->evaluate();
    } catch (const QString& e) {
        throw e;
    }
}

double chordMethod(QString& line, double a, double b, double eps)
{
    double fnA = fn(line, a);
    double fnB = fn(line, b);
    double x = 0;
    double y = 0;

    do {
        x = b - (fnB * (b - a)) / (fnB - fnA);
        y = fn(line, x);

        if (fnB * y <= 0) {
            a = b;
            fnA = fnB;
        }

        b = x;
        fnB = y;

    } while (std::abs(y) > eps);

    return x;
}

double dichotomyMethod(QString& line, double a, double b, double eps)
{
    double fnA = fn(line, a);
    double fnB = fn(line, b);

    if (fnA * fnB > 0) {
        throw QString("No root exists in the given interval.");
    }

    double x = 0;

    while (std::abs(b - a) > eps) {
        x = (a + b) / 2;
        double y = fn(line, x);

        if (fnA * y > 0) {
            a = x;
            fnA = y;
        } else {
            b = x;
            fnB = y;
        }

    }

    return (a + b) / 2;
}

void appendPoints(QString& line, double step, double start, double end, QVector<double>& x, QVector<double>& y) {
    while (start < end) {
        try {
            appendPoint(line, start, x, y);
        } catch (const QString& e) {
            throw e;
        }

        start += step;
    }
}

void appendPoint(QString& line, double xAxis, QVector<double>& x, QVector<double>& y) {
    x.append(xAxis);

    try {
        y.append(fn(line, x.back()));
    } catch (const QString& e) {
        throw e;
    }
}

