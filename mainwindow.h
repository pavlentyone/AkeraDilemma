#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <math.h>
#include <random>
#include <QVector>
#include <QDebug>
#include <algorithm>
#include <QtMath>
#include <QStringListModel>
#include <QStringList>
#include <QListView>
#include <QSlider>
#include <QLabel>
#include <QPoint>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbCreateGeneration_clicked();

    void on_pbProcess_clicked();

    void on_pbShowGraph_clicked();

    void on_hsIterationAmount_valueChanged(int value);

    void on_pb5BestGraphShow_clicked();

private:
    Ui::MainWindow *ui;
//    double maxX;
//    double minX;
    int chromosomeAmount;
    int solutionAmount;
    int compAmount;
    int geneAmount;
    int bitArrayToInt32(bool arr[], int count);
    bool Function(QVector<bool> thisChromosome, QVector<bool> apponentChromosome);
    //int IntFromBoolVector(QVector<float> vec);
    //double IntNormalize(int val);
    QVector<QVector<bool>> OnePointMethod(QVector<QVector<bool>> parentGeneration);
    //QVector<QVector<float>> MultyPointMethod(QVector<QVector<float>> parentGeneration);
    QVector<QVector<bool>> UniformMethod(QVector<QVector<bool>> parentGeneration);
    void GetNextGeneration(QVector<QVector<bool>> parentGeneration);
    QVector<QVector<bool>> Mutation(QVector<QVector<bool>> generation);
    void ShowInListView(QVector<QVector<bool>> vec, QListView *lv);
    QVector<QVector<bool>> firstGeneration;
    QVector<QVector<bool>> minGeneration;
    QVector<QVector<bool>> middleGeneration;
    QVector<int> maxFitnessSequence;
    QVector<double> aveFitnessSequence;
    int maxFitnessTemp;
    double aveFitnessTemp;
    int theLargestFitness;
    void AddGraph(QVector<QVector<bool>> thisGeneration, QCustomPlot *Graph);
};

#endif // MAINWINDOW_H
