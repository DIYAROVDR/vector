#ifndef SOLVER_H
#define SOLVER_H

#include <chrono>
#include <thread>

#include <mkl.h>
#include <Eigen/PardisoSupport>

#include "h5filemanager.h"
#include "grid.h"
#include "well.h"

#include <iostream>

#include <QObject>

#include <QElapsedTimer>
#include <QDebug>

class Solver : public QObject {
    Q_OBJECT
public:
    Solver(Grid* grid, Well* well);
    void setup(double totalTime, double initialDt);
    void stop();  // Остановка расчётов
signals:
    void finished();  // Сигнал об окончании работы
    void progressUpdated(double currentTime, double totalTime);  // Сигнал для обновления прогресса
    void errorOccurred(const QString& error);  // Сигнал об ошибке
public slots:
    void run();  // Слот для запуска расчётов
private:
    Grid* grid;
    Well* well;
    H5FileManager& h5filemanager;
    double tolnewt = 1E-3; // Величина невязки, при которой происходит выход из Ньютоновских итераций
    double dtmin = 1E-3; // Минимальный допустимый временной шаг.
    double tollin = 1E-3; // Точность решения линейной системы
    int maxnewtit = 100; // Максимально допустимое число Ньютоновских итераций.
    double totalTime;
    double initialDt;
    bool isStopped;  // Флаг для остановки расчётов


    Eigen::VectorXd R;
    Eigen::VectorXd Pnew;
    Eigen::VectorXd Pold;
    Eigen::SparseMatrix<double> jacobian;
    Eigen::PardisoLDLT <Eigen::SparseMatrix<double>> solver;
    bool applyNewtonRaphson(double dt);
    void assembleSystem(double dt);
    void solveAllSteps();
};

#endif // SOLVER_H
