#pragma once
#include <QObject>

#include "A_PhysicalValues.hpp"

class Box: public QObject {
    Q_OBJECT
public:
    physvalue top = -10;
    physvalue bottom = 20;
    physvalue left = -10;
    physvalue right = 40;
};


