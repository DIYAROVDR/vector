#include "blockcenteredgrid.h"


BlockCenteredGrid::BlockCenteredGrid() {

}


void BlockCenteredGrid::init() {
    volume();
    trang();
    coords();
    this->P0 = P;
    Q.resize(size);
    Q.setConstant(0);
}


BlockCenteredGrid::~BlockCenteredGrid() {

}


void BlockCenteredGrid::volume() {
    V = dx*dy*dz;
}


void BlockCenteredGrid::trang() {
    Tg = Eigen::MatrixXd::Zero(size, 6);
    for (int n = 0; n < 6; ++n) {
        Eigen::Array<bool, Eigen::Dynamic, 1> valid = (neighbors.col(n).array() != -1);
        Eigen::ArrayXd k = Eigen::ArrayXd::Zero(size);
        Eigen::ArrayXd permn = Eigen::ArrayXd::Zero(size);
        Eigen::ArrayXd dn = Eigen::ArrayXd::Zero(size);

        if(n == 0 || n == 1) {
            permn = valid.select(permx(neighbors.col(n)), permn);
            dn = valid.select(dx(neighbors.col(n)), dn);
            k = (dx + dn)/(dx/permx + dn/permn);
            Tg.col(n) = valid.select(k*(dy*dz)/dx,0.0);
        }

        if(n == 2 || n == 3) {
            permn = valid.select(permy(neighbors.col(n)), permn);
            dn = valid.select(dy(neighbors.col(n)), dn);
            k = (dy + dn)/(dy/permy + dn/permn);
            Tg.col(n) = valid.select(k*(dx*dz)/dy,0.0);
        }

        if(n == 4 || n == 5) {
            permn = valid.select(permz(neighbors.col(n)), permn);
            dn = valid.select(dz(neighbors.col(n)), dn);
            k = (dz + dn)/(dz/permz + dn/permn);
            Tg.col(n) = valid.select(k*(dx*dy)/dz,0.0);
        }
    }
}

void BlockCenteredGrid::coords() {
    Eigen::ArrayXi i = Eigen::ArrayXi::LinSpaced(nx, 0, nx - 1).replicate(ny * nz, 1);
    Eigen::ArrayXi j = Eigen::ArrayXi::LinSpaced(nx * ny, 0, ny - 1).replicate(nz, 1);
    Eigen::ArrayXi k = Eigen::ArrayXi::LinSpaced(nx * ny * nz, 0, nz - 1);

    Eigen::ArrayXi index = (i < nx).select(i, i - 1) + nx * ((j < ny).select(j, j - 1) + ny * (k < nz).select(k, k - 1));

    x = Eigen::ArrayXd::Zero(nx * ny * nz);
    y = Eigen::ArrayXd::Zero(nx * ny * nz);
    z = Eigen::ArrayXd::Zero(nx * ny * nz);

    double x_old = 0.0, y_old = 0.0, z_old = 0.0;
    int index_old = 0, j_old = -1, k_old = -1;

    std::transform(i.data(), i.data() + i.size(), index.data(), x.data(),[&](int i, int idx) {
        if (i == 0) return x_old = 0.0;
        if (idx == index_old) return x_old += dx[idx];
        index_old = idx;
        return x_old = dx[idx] + x_old;
    });

    std::transform(j.data(), j.data() + j.size(), index.data(), y.data(),[&](int j, int idx) {
        if (j == j_old) return y_old;
        j_old = j;
        return y_old = (j == 0) ? 0.0 : dy[idx] + y_old;
    });

    std::transform(k.data(), k.data() + k.size(), index.data(), z.data(), [&](int k, int idx) {
        if (k == k_old) return z_old;
        k_old = k;
        return z_old = (k == 0) ? 0.0 : dz[idx] + z_old;
    });
}


void BlockCenteredGrid::update() {
    T.setZero();
    D.setZero();
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(size * 7);

    Eigen::MatrixXd Tm = Eigen::MatrixXd::Zero(size, 6);
    Eigen::MatrixXd F = Eigen::MatrixXd::Zero(size, 6);

    for (int n = 0; n < 6; ++n) {
        Eigen::VectorXd Pn = Eigen::VectorXd::Zero(size);
        Eigen::ArrayXd Fi = Eigen::ArrayXd::Zero(size);
        Eigen::Array<bool, Eigen::Dynamic, 1> valid = (neighbors.col(n).array() != -1);

        Pn = valid.select(P(neighbors.col(n)), Pn);
        Fi = valid.select(Pn - P, 0.0);

        F.col(n) = (Fi > 0).select(
            1/(pvt.Mu(Pn)*pvt.B(Pn)),
            1/(pvt.Mu(P)*pvt.B(P))
        );
    }

    Tm = Tg.array() * F.array();

    for(int i = 0; i < size; ++i) {
        for(int n = 0; n < 6; ++n) {
            if (neighbors(i, n) != -1){
                triplets.emplace_back(i, neighbors(i, n), -Tm.col(n)[i]);
            }
        }
        triplets.emplace_back(i, i, Tm.row(i).sum());
    }

    T.setFromTriplets(triplets.begin(), triplets.end());

    poro = poroinit*(rock.C()*(P.array()-P0)).exp();

    D.setIdentity();
    D.diagonal() = V*(poro*pvt.C() + rock.C()*poroinit/pvt.B(P));
}


void BlockCenteredGrid::setDimens(int nx, int ny, int nz) {
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;
    size = nx*ny*nz;
    T.resize(size,size);
    D.resize(size,size);

    neighbors.resize(size, 6);
    neighbors.setConstant(-1);

    Eigen::ArrayXi i = Eigen::ArrayXi::LinSpaced(nx, 0, nx - 1).replicate(ny * nz, 1);
    Eigen::ArrayXi j = Eigen::ArrayXi::LinSpaced(nx * ny, 0, ny - 1).replicate(nz, 1);
    Eigen::ArrayXi k = Eigen::ArrayXi::LinSpaced(nx * ny * nz, 0, nz - 1);

    Eigen::ArrayXi index = (i < nx).select(i, i - 1) + nx * ((j < ny).select(j, j - 1) + ny * (k < nz).select(k, k - 1));

    neighbors.col(0) = (i > 0).select(index - 1, -1);             // Левый сосед
    neighbors.col(1) = (i < nx - 1).select(index + 1, -1);        // Правый сосед
    neighbors.col(2) = (j > 0).select(index - nx, -1);            // Сосед сверху
    neighbors.col(3) = (j < ny - 1).select(index + nx, -1);       // Сосед снизу
    neighbors.col(4) = (k > 0).select(index - nx * ny, -1);       // Сосед впереди
    neighbors.col(5) = (k < nz - 1).select(index + nx * ny, -1);  // Сосед сзади
}

void BlockCenteredGrid::setDx(Eigen::ArrayXd& dx) {
    this->dx = dx;
}

void BlockCenteredGrid::setDy(Eigen::ArrayXd& dy) {
    this->dy = dy;
}

void BlockCenteredGrid::setDz(Eigen::ArrayXd& dz) {
    this->dz = dz;
}


