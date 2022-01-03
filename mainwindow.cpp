#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //maxX;
    //minX;
    chromosomeAmount = 50;
    solutionAmount = 6;
    compAmount = solutionAmount / 2 + 9;
    geneAmount = int(pow(2, solutionAmount));

    theLargestFitness = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::bitArrayToInt32(bool arr[], int count)
{
    int ret = 0;
    int tmp;
    for (int i = 0; i < count; i++) {
        tmp = arr[i];
        ret |= tmp << (count - i - 1);
    }
    return ret;
}

bool MainWindow::Function(QVector<bool> thisChromosome, QVector<bool> apponentChromosome){
    //generate solutions
    QVector<bool> thisSolutions;
    thisSolutions.resize(compAmount);
    QVector<bool> apponentSolutions;
    apponentSolutions.resize(compAmount);
    for(int i = 0; i < solutionAmount; i++){
        if(i % 2 == 0)
            thisSolutions[i / 2] = bool(rand() % 2);
        else
            apponentSolutions[i / 2] = bool(rand() % 2);
    }
    bool arr[solutionAmount];
    int solutionNum;
    bool solution;
    for(int i = solutionAmount; i < (compAmount * 2); i++){
        for(int j = 0 ;j < solutionAmount; j++){
            if((i - j - 1) % 2 == 0)
                arr[j] = thisSolutions[(i - j - 1)/2];
            else
                arr[j] = apponentSolutions[(i - j - 1)/2];
        }
        solutionNum = bitArrayToInt32(arr, solutionAmount);
        if(i % 2 == 0){
            solution = thisChromosome[solutionNum];
            thisSolutions[i / 2] = solution;
        }
        else{
            solution = apponentChromosome[solutionNum];
            apponentSolutions[i / 2] = solution;
        }
    }

    //compute fitnesses;
    int thisFitness = 0;
    int apponentFitness = 0;
    for(int i = 0; i < compAmount; i++){
        if(thisSolutions[i] == true && apponentSolutions[i] == true){
            thisFitness += 1;
            apponentFitness += 1;
        }
        else if(thisSolutions[i] == true && apponentSolutions[i] == false){
            thisFitness += 5;
        }
        else if(thisSolutions[i] == false && apponentSolutions[i] == true){
            apponentFitness += 5;
        }
        else if(thisSolutions[i] == false && apponentSolutions[i] == false){
            thisFitness += 3;
            apponentFitness += 3;
        }
    }
    if(thisFitness < apponentFitness)
        return true;
    else
        return false;
}

//int MainWindow::IntFromBoolVector(QVector<float> vec);
//double MinaWindow::IntNormalize(int val);
QVector<QVector<bool>> MainWindow::OnePointMethod(QVector<QVector<bool>> parentGeneration){
    QVector<QVector<bool>> childGeneration;
    childGeneration.resize(parentGeneration.length());

    for(int i = 0; i < parentGeneration.length(); i++){
        int fatherNum = rand() % parentGeneration.length();
        int motherNum = rand() % parentGeneration.length();

        int crossoverPlace = rand() % parentGeneration[i].length();

        childGeneration[i].resize(parentGeneration[i].length());
        for(int j = 0; j < crossoverPlace; j++)
            childGeneration[i][j] = parentGeneration[fatherNum][j];
        for(int j = crossoverPlace; j < parentGeneration[i].length(); j++)
            childGeneration[i][j] = parentGeneration[motherNum][j];

    }
    return childGeneration;
}

//QVector<QVector<float>> MainWindow::MultyPointMethod(QVector<QVector<float>> parentGeneration);

QVector<QVector<bool>> MainWindow::UniformMethod(QVector<QVector<bool>> parentGeneration){
    QVector<QVector<bool>> childGeneration;
    childGeneration.resize(parentGeneration.length());

    for(int i = 0; i < parentGeneration.length(); i++){
        int fatherNum = rand() % parentGeneration.length();
        int motherNum = rand() % parentGeneration.length();

        childGeneration[i].resize(parentGeneration[i].length());
        for(int j = 0; j < parentGeneration[i].length(); j++)
            if(rand() % 2)
                childGeneration[i][j] = parentGeneration[fatherNum][j];
            else
                childGeneration[i][j] = parentGeneration[motherNum][j];
    }
    return childGeneration;
}

void MainWindow::GetNextGeneration(QVector<QVector<bool>> parentGeneration){
    QVector<QVector<bool>> twoGenerations;
    twoGenerations.resize(parentGeneration.length());

    //copy from parent generation
    for(int i = 0; i < parentGeneration.length(); i++){
        twoGenerations[i].resize(parentGeneration[i].length());
        for(int j = 0; j < parentGeneration[i].length(); j++)
            twoGenerations[i][j] = parentGeneration[i][j];
    }

    QVector<int> twoGenerationsFitness;
    twoGenerationsFitness.resize(twoGenerations.length());

    for(int i = 0 ; i < twoGenerationsFitness.length(); i++)
        twoGenerationsFitness[i] = 0;

    int apponentAmount = 10;
    int apponentNum;
    bool thisWon;
    for(int i = 0; i < parentGeneration.length(); i++){
        for(int j = 0; j < apponentAmount; j++){
            apponentNum = rand() % twoGenerations.length();
            thisWon = Function(twoGenerations[i], twoGenerations[apponentNum]);
            if(thisWon)
                twoGenerationsFitness[i]++;
            /*else
                twoGenerationsFitness[apponentNum]++;*/
        }
    }

    //sorting
    for(int i = 0; i < twoGenerations.length(); i++){
        bool check = true;
        for(int j = 1; j < twoGenerations.length(); j++)
            if(twoGenerationsFitness[j] >
               twoGenerationsFitness[j - 1]){
                QVector<bool> temp = twoGenerations[j];
                twoGenerations[j] = twoGenerations[j - 1];
                twoGenerations[j -1] = temp;
                int tempf = twoGenerationsFitness[j];
                twoGenerationsFitness[j] = twoGenerationsFitness[j - 1];
                twoGenerationsFitness[j - 1] = tempf;
                check = false;
            }
        if(check)
            break;
    }
    if(twoGenerationsFitness.length()){
        maxFitnessTemp = twoGenerationsFitness[0];
        aveFitnessTemp = 0;
        for(int i = 0; i < twoGenerationsFitness.length(); i++)
            aveFitnessTemp += twoGenerationsFitness[i];
        aveFitnessTemp /= double(twoGenerationsFitness.length());
    }
    if(theLargestFitness < maxFitnessTemp)
        theLargestFitness = maxFitnessTemp;
}

QVector<QVector<bool>> MainWindow::Mutation(QVector<QVector<bool>> generation){
    QVector<QVector<bool>> mutatedGeneration;
    mutatedGeneration.resize(generation.length());
    int maxChance = 1000;
    int chance = 100;
    mutatedGeneration.resize(generation.length());
    for(int i = 0; i < generation.length(); i++){
        mutatedGeneration[i].resize(generation[i].length());
        mutatedGeneration[i].resize(generation[i].length());
        for(int j = 0; j < generation[i].length(); j++)
            if(chance >= (rand() % maxChance))
                mutatedGeneration[i][j] = bool(rand() % 2);
            else
                mutatedGeneration[i][j] = generation[i][j];
    }
    return mutatedGeneration;
}

void MainWindow::ShowInListView(QVector<QVector<bool>> vec, QListView *lv){
    QStringList stringList;
    int jMax = /*vec.length() >= 1? 1 :*/vec.length();
    for(int i = 0 ; i < jMax; i++){
        QString string;
        for(int j = vec[i].length() -1; j > -1; j--)
            string += QString::number(vec[i][j]) + ",";
        stringList << QString::number(i+1) + ". " + string;
    }

    QStringListModel *stringListModel = new QStringListModel(this);
    stringListModel->setStringList(stringList);
    lv->setModel(stringListModel);
    lv->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_pbCreateGeneration_clicked()
{
    firstGeneration.resize(chromosomeAmount);
    for(int i = 0; i < firstGeneration.length(); i++){
        firstGeneration[i].resize(geneAmount);
        for(int j = 0; j < firstGeneration[i].length(); j++)
            firstGeneration[i][j] = bool(rand() % 2);
    }

    //show first generation
    ShowInListView(firstGeneration, ui->lvFirstGeneration);
}

void MainWindow::on_pbProcess_clicked()
{
    minGeneration.resize(firstGeneration.length());
    int iterationAmount = ui->lIterationAmount->text().toInt();
    QVector<QVector<bool>> childGeneration;
    if(iterationAmount > 0){
        //first process
        GetNextGeneration(firstGeneration);
        childGeneration = OnePointMethod(firstGeneration);
        for(int i = 0; i < minGeneration.length(); i++)
            minGeneration[i] = childGeneration[i];
        maxFitnessSequence.resize(iterationAmount);
        aveFitnessSequence.resize(iterationAmount);
        //write max fitness value
        maxFitnessSequence[0] = maxFitnessTemp;
        //write average fitness value
        aveFitnessSequence[0] = aveFitnessTemp;
        minGeneration = Mutation(minGeneration);
    }

    ui->statusBar->showMessage("process: " + QString::number(0) + " %");
    this->setCursor(Qt::WaitCursor);
    for(int i = 1 ; i < iterationAmount; i++){
        GetNextGeneration(minGeneration);
        childGeneration = OnePointMethod(minGeneration);
        for(int i = 0; i < minGeneration.length(); i++)
            minGeneration[i] = childGeneration[i];
        //write max fitness value
        maxFitnessSequence[i] = maxFitnessTemp;
        //write average fitness value
        aveFitnessSequence[i] = aveFitnessTemp;

        minGeneration = Mutation(minGeneration);
        if(i == iterationAmount / 2){
            middleGeneration.resize(minGeneration.length());
            for(int i = 0; i < minGeneration.length(); i++){
                middleGeneration[i].resize(minGeneration[i].length());
                for(int j = 0; j < minGeneration[i].length(); j++)
                    middleGeneration[i][j] = minGeneration[i][j];
            }
        }

        ui->statusBar->showMessage("process: " + QString::number(double(i)/double(iterationAmount) * double(100)) + " %");
    }

    this->setCursor(Qt::ArrowCursor);
    //show child generation
    ShowInListView(middleGeneration, ui->lvChildGeneration);

    ShowInListView(minGeneration, ui->lvNextGeneration);
    ui->statusBar->showMessage("");
}

void MainWindow::on_pbShowGraph_clicked()
{
    QVector<double> X;
    X.resize(maxFitnessSequence.length());
    for(int i = 0; i < X.length(); i++)
        X[i] = double(i);

    QVector<double> maxYSequence;
    maxYSequence.resize(maxFitnessSequence.length());
    for(int i = 0; i < maxFitnessSequence.length(); i++)
        maxYSequence[i] = double(maxFitnessSequence[i]);

    QVector<double> aveYSequence;
    aveYSequence.resize(aveFitnessSequence.length());
    for(int i = 0; i < aveFitnessSequence.length(); i++)
        aveYSequence[i] = double(aveFitnessSequence[i]);

    ui->cpGraph->addGraph();
    ui->cpGraph->graph(0)->setData(X, maxYSequence);
    ui->cpGraph->graph(0)->setPen(QColor(20,20,200,255));
    ui->cpGraph->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->cpGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    ui->cpGraph->addGraph();
    ui->cpGraph->graph(1)->setData(X, aveYSequence);
    ui->cpGraph->graph(1)->setPen(QColor(200,150,0,255));
    ui->cpGraph->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->cpGraph->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 3));

    ui->cpGraph->xAxis->setLabel("Generation number");
    ui->cpGraph->yAxis->setLabel("Fitness value");
    ui->cpGraph->xAxis->setRange(0, double(X.length()) + 1);//Для оси Ox
    ui->cpGraph->yAxis->setRange(0, theLargestFitness + 2);//Для оси Oy
    ui->cpGraph->replot();
}

void MainWindow::on_hsIterationAmount_valueChanged(int value)
{
    ui->lIterationAmount->setText(QString::number(value));
}


void MainWindow::on_pb5BestGraphShow_clicked()
{
    ui->cp5BestInFirstGeneration->clearGraphs();
    ui->cp5BestInMiddleGeneration->clearGraphs();
    ui->cp5BestInLastGeneration->clearGraphs();
    AddGraph(firstGeneration, ui->cp5BestInFirstGeneration);
    AddGraph(middleGeneration,  ui->cp5BestInMiddleGeneration);
    AddGraph(minGeneration, ui->cp5BestInLastGeneration);
}

void MainWindow::AddGraph(QVector<QVector<bool>> thisGeneration, QCustomPlot *Graph){

    Graph->addGraph();
    QVector<double> XPositions(2);
    QVector<double> YPositions(2);
    XPositions[0] = double(500);
    YPositions[0] = double(500);
    XPositions[1] = double(200);
    YPositions[1] = double(800);
    Graph->graph(0)->setData(XPositions, YPositions);
    Graph->graph(0)->setPen(QColor(230,0,0,255));
    Graph->graph(0)->setLineStyle(QCPGraph::lsNone);
    Graph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));

    Graph->addGraph();
    XPositions.resize(5);
    YPositions.resize(5);
    XPositions[0] = double(300 - 50);
    YPositions[0] = double(700 - 50);
    XPositions[1] = double(300 + 50);
    YPositions[1] = double(700 - 50);
    XPositions[2] = double(300 + 50);
    YPositions[2] = double(700 + 50);
    XPositions[3] = double(300 - 50);
    YPositions[3] = double(700 + 50);
    XPositions[4] = double(300 - 50);
    YPositions[4] = double(700 - 50);
    Graph->graph(1)->setData(XPositions, YPositions);
    Graph->graph(1)->setPen(QColor(0,0,240,255));
    Graph->graph(1)->setLineStyle(QCPGraph::lsLine);
    Graph->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    int positionMax = thisGeneration.length() >= 2 ? 2 : thisGeneration.length();
    for(int position = 0; position < positionMax; position++){
        int thisGenerPosition = position;
        XPositions.resize(thisGeneration[position].length());
        YPositions.resize(thisGeneration[position].length());
        if(position == 1){
            thisGenerPosition = thisGeneration.length() - 1;
        }
        XPositions[0] = double(500);
        YPositions[0] = double(500);
        for(int i = 1; i < thisGeneration[thisGenerPosition].length(); i++)
            switch (thisGeneration[thisGenerPosition][i]) {
            case 0:
                XPositions[i] = XPositions[i - 1] - 1;
                YPositions[i] = YPositions[i - 1];
                break;
            case 1:
                YPositions[i] = YPositions[i - 1] - 1;
                XPositions[i] = XPositions[i - 1];
                break;
            case 2:
                XPositions[i] = XPositions[i - 1] + 1;
                YPositions[i] = YPositions[i - 1];
                break;
            case 3:
                YPositions[i] = YPositions[i - 1] + 1;
                XPositions[i] = XPositions[i - 1];
                break;
            default:
                break;
            }

        Graph->addGraph();
        Graph->graph(position + 2)->setData(XPositions, YPositions);
        switch (position + 2) {
        case 2:
            Graph->graph(position + 2)->setPen(QColor(0,150,150,100));
            break;
        case 3:
            Graph->graph(position + 2)->setPen(QColor(150,150,0,100));
            break;
        case 4:
            Graph->graph(position + 2)->setPen(QColor(0,0,230,100));
            break;
        case 5:
            Graph->graph(position + 2)->setPen(QColor(50,50,50,100));
            break;
        case 6:
            Graph->graph(position + 2)->setPen(QColor(0,230,0,255));
            break;
        default:
            break;
        }
        Graph->graph(position + 2)->setLineStyle(QCPGraph::lsLine);
        Graph->graph(position + 2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
    }

    Graph->xAxis->setLabel("x");
    Graph->yAxis->setLabel("y");
    Graph->xAxis->setRange(0, 1000);//Для оси Ox
    Graph->yAxis->setRange(0, 1000);//Для оси Oy
    Graph->replot();
}
