#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QString>

double fn(QString& expression, double value);
double chordMethod(QString& str, double a, double b, double eps);
double dichotomyMethod(QString& str, double a, double b, double eps);
void appendPoints(QString& str, double step, double start, double end, QVector<double>& x, QVector<double>& y);
void appendPoint(QString& str, double xAxis, QVector<double>& x, QVector<double>& y);

#endif // OPERATIONS_H
