//
// Created by dotelsaramsha on 5/1/19.
//

#ifndef ORDERMATCHING_CSVREADER_H
#define ORDERMATCHING_CSVREADER_H

#include <fstream>
#include "OrderPoint.h"
#include "OrderTable.h"


class FileReader {
private:
    fstream csvFile;
public:
    FileReader (const char*);
    void fillOrderTable(OrderTable*);
};


#endif //ORDERMATCHING_CSVREADER_H
