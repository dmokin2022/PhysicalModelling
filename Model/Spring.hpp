#pragma once

//#include <QObject>
#include "Particle.hpp"

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

    // Условно вязкость пружины
    physvalue hookDecay;

    Spring(Particle &p1, Particle &p2, physvalue k = 1);

    virtual ~Spring() {}

    void computeHookForce();
};

