#pragma once

//#include <QObject>
#include "A_Particle.hpp"

// Объект пружина для создания упругих тел
class Spring {
//class Spring : public QObject {
//    Q_OBJECT
public:
    Particle *p1;
    Particle *p2;
    physvalue k;
    physvalue dx;
    physvalue dy;
    physvalue l;    // длина пружины. Опрделяется по начальному расстоянию между частицами


    Spring(Particle &p1, Particle &p2, physvalue k = 1);

    virtual ~Spring() {}

    void computeHookForce();
};
