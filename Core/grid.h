#ifndef GRID_H
#define GRID_H

#include <Eigen/Sparse>

#include "pvt.h"
#include "rock.h"

class Grid {
public:
    enum class Initialization {
        EQUILIBRIUM,
        NONEQUILIBRIUM
    };
    enum class Type {
      BLOCKCENTERED,
      CORNERPOINT,
      VERTEX
    };
    Grid();
    virtual ~Grid() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void setDimens(int nx, int ny, int nz) = 0;

    virtual void setDx(Eigen::ArrayXd& dx) = 0;
    virtual void setDy(Eigen::ArrayXd& dy) = 0;
    virtual void setDz(Eigen::ArrayXd& dz) = 0;

    void setPoro(const Eigen::ArrayXd& poro);
    void setPermx(const Eigen::ArrayXd& permx);
    void setPermy(const Eigen::ArrayXd& permy);
    void setPermz(const Eigen::ArrayXd& permz);
    void setPressure(const Eigen::VectorXd& P);
    void setPVT(const PVT& pvt);
    void setRock(const Rock& rock);
    void addDt(double dt);

    int blockIndex(int i, int j, int k) const;

    int NX();
    int NY();
    int NZ();

    Eigen::SparseMatrix<double> getT();
    Eigen::SparseMatrix<double> getD();
    Eigen::VectorXd getP();
    Eigen::VectorXd getQ();

    double currentTime();

    double permxi(int i);
    double permyi(int i);
    double permzi(int i);

    double mu(int i);

    double xi(int i);
    double yi(int i);
    double zi(int i);

    double dxi(int i);
    double dyi(int i);
    double dzi(int i);

    double& Pi(int i);
    double& Qi(int i);

    double radius(int i);
    double perm(int i);

    Eigen::ArrayXd X();
    Eigen::ArrayXd Y();
    Eigen::ArrayXd Z();

protected:
    int nx;
    int ny;
    int nz;
    int size;
    double time = 0.0;

    Eigen::MatrixXi neighbors;
    Eigen::MatrixXd Tg;

    Eigen::VectorXd P;
    Eigen::VectorXd Q;

    Eigen::ArrayXd P0;

    Eigen::ArrayXd x;
    Eigen::ArrayXd y;
    Eigen::ArrayXd z;

    Eigen::ArrayXd dx;
    Eigen::ArrayXd dy;
    Eigen::ArrayXd dz;

    Eigen::ArrayXd Ax;
    Eigen::ArrayXd Ay;
    Eigen::ArrayXd Az;

    Eigen::ArrayXd permx;
    Eigen::ArrayXd permy;
    Eigen::ArrayXd permz;

    Eigen::ArrayXd poro;
    Eigen::ArrayXd poroinit;

    Eigen::ArrayXd V;

    Eigen::SparseMatrix<double> T;
    Eigen::SparseMatrix<double> D;

    PVT pvt;
    Rock rock;

    virtual void volume() = 0;
    virtual void trang() = 0;
    virtual void coords() = 0;
};

#endif // GRID_H
