#include "wells.h"

Wells::Wells() {}

void Wells::addWell(const Well& well) {
    wells.push_back(well);
}

void Wells::update() {
    for(auto& w : wells){
        w.update();
    }
}
