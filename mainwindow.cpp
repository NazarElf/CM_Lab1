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
    RR *PU1 = new RR(mu,lambda);
    double b[1];
    double iter = 0.0001;
    for(int i = 0; i < 1; ++i, iter /= 2.0)
    {
        PU1->tau = iter;
        PU1->Start(inVec,poisson);


        b[i] = PU1->getMx();

        //double dd1 = PU1->getDx();
    }

    ui->Results->showMessage(QString::number(b[1]), 0);
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
/*
    for(auto val: inVec)
    {
        std::cout << val << "\t";
    }
*/
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
