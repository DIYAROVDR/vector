#ifndef WELLS_H
#define WELLS_H

#include "../Core/well.h"

class Wells {
public:
    Wells();
    void addWell(const Well& well);
    void update();
private:
    std::vector<Well> wells;
};

#endif // WELLS_H
