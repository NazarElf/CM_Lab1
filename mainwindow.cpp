#include "mainwindow.h"
#include "fifo.h"
#include "rr.h"
#include "sf.h"
#include "processingunit.h"
#include "ui_mainwindow.h"
#include <random>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_clicked()
{
    //inputing data
    int count = ui->Count->value();
    double mu = ui->Mu->value();
    double lambda = ui->Lambda->value();
    //generation of times of tasks and time to next task
    size_t m_seed = std::random_device()();
    std::mt19937_64 gen(m_seed);
    std::normal_distribution<double> d(1.0/mu, 1.0/mu);
    
    std::vector<double> inVec;
    for(int i = 1; i < count + 1; ++i)
    {
        double temp;
        do{
            temp = (d(gen));
        } while(temp < 0);
        inVec.push_back(temp);
    }
    genPoisson(count, lambda);
    
    //starting processor
    ProcessingUnit *PU1 = new FIFO(mu,lambda);
    ProcessingUnit *PU2 = new SF(mu,lambda);
    ProcessingUnit *PU3 = new RR(mu,lambda);
    PU3->tau = 0.0001;
    PU1->Start(inVec,poisson);
    PU2->Start(inVec,poisson);
    PU3->Start(inVec,poisson);

    //counting functions
    double FIFO_mx = PU1->getMx();
    double SF_mx = PU2->getMx();
    double RR_mx = PU3->getMx();

    double FIFO_Dx = PU1->getDx();
    double SF_Dx = PU2->getDx();
    double RR_Dx = PU3->getDx();

    double FIFO_f = -5.0 * FIFO_mx - 2.0 * FIFO_Dx;
    double SF_f = -5.0 * SF_mx - 2.0 * SF_Dx;
    double RR_f = -5.0 * RR_mx - 2.0 * RR_Dx;

    //organizing output
    ui->FIFO_Dx->setText(QString::number(FIFO_Dx));
    ui->FIFO_mx->setText(QString::number(FIFO_mx));
    ui->FIFO_func->setText(QString::number(FIFO_f));

    ui->SF_Dx->setText(QString::number(SF_Dx));
    ui->SF_mx->setText(QString::number(SF_mx));
    ui->SF_func->setText(QString::number(SF_f));

    ui->RR_Dx->setText(QString::number(RR_Dx));
    ui->RR_mx->setText(QString::number(RR_mx));
    ui->RR_func->setText(QString::number(RR_f));

    //ui->Results->showMessage(QString::number(b), 0);
    //delete PU;
}

void MainWindow::on_Unusual_clicked()
{
    int count = ui->Count->value();
    double mu = ui->Mu->value();
    double lambda = ui->Lambda->value();
    int number = ui->Number->value();
    double utime = ui->Time->value();


    size_t m_seed = std::random_device()();
    std::mt19937_64 gen(m_seed);
    std::normal_distribution<double> d(1.0/mu, mu);

    std::vector<double> inVec;
    for(int i = 1; i < count + 1; ++i)
    {
        if(i % number == 0 )
            inVec.push_back(utime);
        else
        {
            double temp;
            do{
                temp = (d(gen));
            } while(temp < 0);
            inVec.push_back(temp);
        }
    }

    genPoisson(count, lambda);

    FIFO *PU = new FIFO(mu,lambda);
    PU->Start(inVec,poisson);

    //starting processor
    ProcessingUnit *PU1 = new FIFO(mu,lambda);
    ProcessingUnit *PU2 = new SF(mu,lambda);
    ProcessingUnit *PU3 = new RR(mu,lambda);
    PU3->tau = 0.0001;
    PU1->Start(inVec,poisson);
    PU2->Start(inVec,poisson);
    PU3->Start(inVec,poisson);

    //counting functions
    double FIFO_mx = PU1->getMx();
    double SF_mx = PU2->getMx();
    double RR_mx = PU3->getMx();

    double FIFO_Dx = PU1->getDx();
    double SF_Dx = PU2->getDx();
    double RR_Dx = PU3->getDx();

    double FIFO_f = -5.0 * FIFO_mx - 2.0 * FIFO_Dx;
    double SF_f = -5.0 * SF_mx - 2.0 * SF_Dx;
    double RR_f = -5.0 * RR_mx - 2.0 * RR_Dx;

    //organizing output
    ui->FIFO_Dx->setText(QString::number(FIFO_Dx));
    ui->FIFO_mx->setText(QString::number(FIFO_mx));
    ui->FIFO_func->setText(QString::number(FIFO_f));

    ui->SF_Dx->setText(QString::number(SF_Dx));
    ui->SF_mx->setText(QString::number(SF_mx));
    ui->SF_func->setText(QString::number(SF_f));

    ui->RR_Dx->setText(QString::number(RR_Dx));
    ui->RR_mx->setText(QString::number(RR_mx));
    ui->RR_func->setText(QString::number(RR_f));

}

void MainWindow::genPoisson(int count, double lambda)
{
    this->poisson.clear();
    size_t m_seed = std::random_device()();
    std::mt19937_64 gen(m_seed);
    std::poisson_distribution<int> p(lambda);

    for(int i = 0; i < count; )
    {
        int in = p(gen);
        double t = 0;
        while(in == 0)
        {
            t += 1;
            in = p(gen);
        }

        i += in;
        this->poisson.push_back(t + 1.0/in);
        for(int j = 0; j < in - 1; ++j)
            this->poisson.push_back(1.0/in);
    }
}
