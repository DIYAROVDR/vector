#ifndef BLOCKCENTEREDGRID_H
#define BLOCKCENTEREDGRID_H

#include "grid.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>

class BlockCenteredGrid : public Grid {
private:
    void volume() override;
    void trang() override;
    void coords() override;

public:
    BlockCenteredGrid();
    void init() override;
    void update() override;
    void setDimens(int nx, int ny, int nz) override;
    void setDx(Eigen::ArrayXd& dx) override;
    void setDy(Eigen::ArrayXd& dy) override;
    void setDz(Eigen::ArrayXd& dz) override;
    ~BlockCenteredGrid();
};

#endif // BLOCKCENTEREDGRID_H
