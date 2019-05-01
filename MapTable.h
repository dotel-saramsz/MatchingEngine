//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_MAPTABLE_H
#define ORDERMATCHING_MAPTABLE_H

#include "OrderTable.h"

class MapTable: public OrderTable {
private:
    //elements only needed in the map implementation
public:
    void insert(OrderPoint*) override;

    void remove(OrderPoint*) override;

    void lookup(OrderPoint*) override;

    void calculateEQprice() override;

    int insert(float);

    MapTable();
};


#endif //ORDERMATCHING_MAPTABLE_H
