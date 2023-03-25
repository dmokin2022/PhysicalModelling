#pragma once

#include "A_PhysicalValues.hpp"
#include <cmath>

class Particle {
//class Particle: public QObject {
//    Q_OBJECT
public:

    physvalue x;    // Координаты
    physvalue y;
    physvalue z;

    physvalue r;    // радиус
    physvalue m;    // масса


    physvalue Fx;   // проекции силы
    physvalue Fy;
    physvalue ax;   // проекции ускорения
    physvalue ay;
    physvalue vx;   // проекции скорости
    physvalue vy;

    int springLinksCounter; // счётчик числа связей частицы с другими частицами
    bool connectedWithNeighbours = false;


    Particle(physvalue x, physvalue y, physvalue r = 1, physvalue m = 1);
    virtual ~Particle() {}

    physvalue getSumForce();
    void computeAcceleration();
    void computeVelocity();
    void computeMovement();
};
