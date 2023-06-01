#include "mainwindow.h"
#include "operations.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , validFn(new QRegularExpressionValidator(QRegularExpression("[+-/*.^()0-9x]+"), this))
    , validDouble(new QRegularExpressionValidator(QRegularExpression("^(-?\\d+(\\.\\d+)?)$"), this))
    , isInteraction(false)
{
    ui->setupUi(this);

    ui->customPlotWidget->xAxis->setRange(-30, 30);
    ui->customPlotWidget->yAxis->setRange(-15, 15);

    ui->customPlotWidget->xAxis->setLabel("x");
    ui->customPlotWidget->yAxis->setLabel("y");

    for (int i = 0; i < 5; i++) {
        ui->customPlotWidget->addGraph();
    }

    ui->customPlotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlotWidget->replot();

    ui->comboBoxMethod->addItem("Chord");
    ui->comboBoxMethod->addItem("Dichotomy");

    ui->lineEditFunction->setValidator(validFn);
    ui->lineEditEpsilon->setValidator(validDouble);
    ui->lineEditPointA->setValidator(validDouble);
    ui->lineEditPointB->setValidator(validDouble);

    QObject::connect(ui->lineEditFunction, &QLineEdit::textChanged, this, [this](){
        functionEvents(false);

        for (int i = 0; i < 5; i ++) {
            eraseGraph(i);
        }
    });
    QObject::connect(ui->lineEditFunction, &QLineEdit::returnPressed, this, [this](){
        drawPlot();
    });
    QObject::connect(ui->lineEditFunction, &QLineEdit::editingFinished, this, [this](){
        drawPlot();
    });
    QObject::connect(ui->customPlotWidget, &QCustomPlot::beforeReplot, this, [this](){
        if (isInteraction) {
            drawPlot();
        }
    });
    QObject::connect(ui->lineEditPointA, &QLineEdit::textChanged, this, [this](){
        pointEvents(ui->lineEditPointA, 2);
    });
    QObject::connect(ui->lineEditPointB, &QLineEdit::textChanged, this, [this](){
        pointEvents(ui->lineEditPointB, 3);
    });
    QObject::connect(ui->buttonCalcucate, &QPushButton::clicked, this, &MainWindow::calculate);
    QObject::connect(ui->buttonClear, &QPushButton::clicked, this, &MainWindow::clear);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete validFn;
    delete validDouble;
}

bool checkExpression(QString& line) {
    if (line.size() == 0) {
        QMessageBox::warning(nullptr, "Warning", "The lenght of the function expression must be greater than zero!");
        return true;
    }
    return false;
}

void MainWindow::pointEvents(QLineEdit* line, int graphNum) {
    if (line->text().size() == 0) {
        eraseGraph(graphNum);
        return;
    }
    double x = line->text().toDouble();
    drawPoint(x, graphNum, QPen(Qt::red));
}

void MainWindow::functionEvents(bool value) {
    isInteraction = value;
    ui->lineEditPointA->setEnabled(value);
    ui->lineEditPointB->setEnabled(value);
    ui->lineEditEpsilon->setEnabled(value);
    ui->comboBoxMethod->setEnabled(value);
}

void MainWindow::drawPlot()
{
    QString line = ui->lineEditFunction->text();

    if (checkExpression(line)) {
        return;
    }

    double lower = ui->customPlotWidget->xAxis->range().lower;
    double upper = ui->customPlotWidget->xAxis->range().upper;
    double step = (upper - lower) / 200;

    QVector<double> x {}, y {};
    QVector<double> a {}, b {};

    try {
        if (upper <= 0 || lower >= 0) {
            appendPoints(line, step, lower, upper, x, y);
        } else {
            appendPoints(line, step, lower, 0, a, b);
            appendPoint(line, -0.0000001, a, b);

            appendPoint(line, 0.00000001, x, y);
            appendPoints(line, step, 0, upper, x, y);
        }
    } catch (const QString& except) {
        QMessageBox::warning(nullptr, "Warning", except);
        return;
    }

    ui->customPlotWidget->graph(1)->setData(a, b);
    ui->customPlotWidget->graph(0)->setData(x, y);
    ui->customPlotWidget->replot();

    functionEvents(true);
}

void MainWindow::drawPoint(double x, int graphNum, QPen color) {
    QString line = ui->lineEditFunction->text();

    if (checkExpression(line)) {
        return;
    }

    double y = fn(line, x);

    auto graph = ui->customPlotWidget->graph(graphNum);
    graph->data()->clear();

    graph->addData(x, y);
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle::ssDisc);
    graph->setPen(color);

    ui->customPlotWidget->replot();
}

void MainWindow::eraseGraph(int graphNum) {
    ui->customPlotWidget->graph(graphNum)->data()->clear();
    ui->customPlotWidget->replot();
}

bool MainWindow::noLines()
{
    bool value = false;

    if (ui->lineEditFunction->text().size() == 0) {
        value = true;
    } else if (ui->lineEditEpsilon->text().size() == 0) {
        value = true;
    } else if (ui->lineEditPointA->text().size() == 0) {
        value = true;
    } else if (ui->lineEditPointB->text().size() == 0) {
        value = true;
    }

    if (value) {
        QMessageBox::warning(nullptr, "Warning", "Some fields are empty");
    }

    return value;
}

void MainWindow::calculate()
{
    if (noLines()) {
        return;
    }

    QString line = ui->lineEditFunction->text();
    double eps = ui->lineEditEpsilon->text().toDouble();
    double a = ui->lineEditPointA->text().toDouble();;
    double b = ui->lineEditPointB->text().toDouble();

    QString method = ui->comboBoxMethod->currentText();
    auto resultText = ui->plainTextEditResult;
    double value = 0;

    try {
        if (method == "Chord") {
            resultText->appendPlainText("Result of Chord Method: ");
            value = chordMethod(line, a, b, eps);
        } else if (method == "Dichotomy") {
            resultText->appendPlainText("Result of Dichotomy Method: ");
            value = dichotomyMethod(line, a, b, eps);
        }

        resultText->insertPlainText(QString::number(value));
        drawPoint(value, 4, QPen(Qt::green));

    } catch (const QString& except) {
        resultText->insertPlainText(except);
        QMessageBox::warning(nullptr, "Warning", except);
        eraseGraph(4);
    }
}

void MainWindow::clear()
{
    ui->lineEditFunction->clear();
    ui->lineEditEpsilon->clear();
    ui->lineEditPointA->clear();
    ui->lineEditPointB->clear();
    ui->plainTextEditResult->clear();

    for (int i = 0; i < 5; i ++) {
        eraseGraph(i);
    }
}
