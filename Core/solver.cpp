#include "solver.h"

Solver::Solver(Grid* grid, Well* well) :
    grid(grid),
    well(well),
    h5filemanager(H5FileManager::instance()) {
    Pnew = grid->getP();  // Инициализируем давления из сетки
    h5filemanager.saveDynamicCube(Pnew.array(),"PRESSURE");
    Eigen::initParallel();
    int numThreads = std::thread::hardware_concurrency();  // Количество потоков, поддерживаемых машиной
    Eigen::setNbThreads(numThreads);
    //mkl_set_num_threads(numThreads);
}

void Solver::setup(double totalTime, double initialDt) {
    this->totalTime = totalTime;
    this->initialDt = initialDt;

    grid->update();
    jacobian = grid->getT() + (grid->getD() / initialDt);
    solver.analyzePattern(jacobian);
}

void Solver::run() {
    try {
        solveAllSteps();
        if (!isStopped) {
            emit finished();
        }
    } catch (const std::exception& ex) {
        emit errorOccurred(QString("Error: %1").arg(ex.what()));
    } catch (...) {
        emit errorOccurred("Unknown error occurred.");
    }
}


void Solver::stop() {
    isStopped = true;
}

void Solver::assembleSystem(double dt) {
    // Обновляем матрицы проводимости T и ёмкости D в сетке
    grid->update();

    // Обновляем правую часть (вектор Q) из объекта скважины
    well->update();

    // Вычисляем матрицу Якоби: J = T + (D / dt)
    jacobian = grid->getT() + (grid->getD() / dt);

    // Вычисляем вектор невязки:
    // R = J * P_new - (D / dt) * P_old  + Q
    R = jacobian * Pnew - (grid->getD() / dt) * Pold + grid->getQ();
}


bool Solver::applyNewtonRaphson(double dt) {


    for (int iter = 0; iter < maxnewtit; ++iter) {
        assembleSystem(dt);

        // Решаем линейную систему: J * delta_P = -R
        solver.factorize(jacobian);

        if (solver.info() != Eigen::Success) {
            std::cout << "Failed to decompose Jacobian matrix.\n";
            return false;
        }

        Eigen::VectorXd deltaP = solver.solve((-R).eval());

        if (solver.info() != Eigen::Success) {
            std::cout << "Failed to solve linear system, info: " << solver.info() << std::endl;
            return false;
        }

        // Обновляем давления
        Pnew += deltaP;
        grid->setPressure(Pnew);
        // Проверяем условие сходимости после обновления давления

        if (R.lpNorm<Eigen::Infinity>() < tolnewt) {
            std::cout << "Converged at step with dt = " << dt << " in " << iter << " iterations.\n";

            // Обновляем давления в сетке только после сходимости

            grid->addDt(dt);
            h5filemanager.saveDynamicCube(Pnew.array(), "PRESSURE");

            return true;
        }

        // Если сходимость не достигнута, продолжаем с следующей итерации
    }

    std::cout << "Newton-Raphson did not converge with dt = " << dt << ".\n";
    return false;
}


void Solver::solveAllSteps() {
    double currentTime = grid->currentTime();
    double dt = initialDt;

    while (currentTime < totalTime) {
        bool converged = false;
        Pold = Pnew;

        while (!converged && dt > dtmin) {
            converged = applyNewtonRaphson(dt);
            if (!converged) {
                dt /= 5.0; // Уменьшаем шаг времени
                std::cout << "Reducing dt to " << dt << " and retrying.\n";
            }
        }

        if (!converged) {
            std::cout << "Failed to converge after 5 attempts. Aborting simulation.\n";
            return;
        }

        currentTime += dt;

        // Увеличиваем шаг времени, если сходимость хорошая
        if (dt < initialDt && R.lpNorm<Eigen::Infinity>() < tolnewt) {
            dt *= 5.0;
            std::cout << "Increasing dt to " << dt << ".\n";
        }
        emit progressUpdated(currentTime, totalTime);
        std::cout << "Step completed. Current time: " << currentTime << " / " << totalTime << "\n";
    }

    std::cout << "Simulation completed successfully.\n";
}
