#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qobjectdefs.h"

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void functionEvents(bool value);
    void pointEvents(QLineEdit* line, int graphNum);
    void drawPoint(double axisPoint, int graphNum, QPen color);
    void drawPlot();
    void eraseGraph(int graphNum);
    void calculate();
    void clear();
    bool noLines();

private:
    Ui::MainWindow* ui;
    QRegularExpressionValidator* validFn;
    QRegularExpressionValidator* validDouble;
    bool isInteraction;
};
#endif // MAINWINDOW_H
