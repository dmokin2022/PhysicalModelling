//import math
////include

//#include <QObject>

#include <cmath>
#include "B_Spring.hpp"


Spring::Spring(Particle &p1, Particle &p2, physvalue k) {
        this->p1 = &p1;
        this->p2 = &p2;
        this->k = k;
        dx = p2.x - p1.x;
        dy = p2.y - p1.y;
        l = sqrt(dx*dx + dy*dy);    // длина пружины. Опрделяется по начальному расстоянию между частицами
}

void Spring::computeHookForce() {
        // Определяем текущее расстояние между частицами
        dx = p2->x - p1->x;
        dy = p2->y - p1->y;
        physvalue lnew = sqrt(dx*dx + dy*dy);

        // Вычисляем разницу между текущим расстоянием и тем, что было при создании пружины
        physvalue dl = lnew - l;  // если пружина сжалась, то dl отрицательное, если растянулась, то dl положительное

        physvalue Fh = k*dl;  // Вычисляем силу упругости Гука по закону Гука

        // вычисление составляющих силы Гука через единичный вектор расстояния между двумя частицами
        physvalue Fhx = dx/lnew*Fh;
        physvalue Fhy = dy/lnew*Fh;

        // прибавляем составляющие силы Гука к сумме всех сил, действующих на частицы, соединяемые пружиной
        p1->Fx += Fhx;   // к одной частице со знаком +
        p1->Fy += Fhy;

        p2->Fx -= Fhx;   // к другой частице со знаком -
        p2->Fy -= Fhy;
}
