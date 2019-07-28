//
// Created by dotelsaramsha on 5/1/19.
//

#ifndef ORDERMATCHING_FILEREADER_H
#define ORDERMATCHING_FILEREADER_H

#include <fstream>
#include <string>
#include "OrderPoint.h"
#include "OrderTable.h"


class FileReader {
private:
    string filename;
    fstream csvFile;
public:
    FileReader (string);
    void fillOrderTable(OrderTable*);
};


#endif //ORDERMATCHING_FILEREADER_H
