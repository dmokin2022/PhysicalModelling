//import math
////include

//#include <QObject>

#include "Spring.hpp"

#include <cmath>

Spring::Spring(Particle &p1, Particle &p2, physvalue k) {
  this->p1 = &p1;
  this->p2 = &p2;
  this->k  = k;
  this->hookDecay = 0.99;
  dx       = p2.x - p1.x;
  dy       = p2.y - p1.y;
  // длина пружины. Опрделяется по начальному расстоянию между частицами
  l = sqrt(dx * dx + dy * dy);
}

void Spring::computeHookForce() {
  // Определяем текущее расстояние между частицами
  dx             = p2->x - p1->x;
  dy             = p2->y - p1->y;
  physvalue lnew = sqrt(dx * dx + dy * dy);

  // Вычисляем разницу между текущим расстоянием и тем, что было при создании пружины
  // если пружина сжалась, то dl отрицательное, если растянулась, то dl положительное
  physvalue dl = lnew - l;

  physvalue Fh = k * dl;  // Вычисляем силу упругости Гука по закону Гука

  // вычисление составляющих силы Гука через единичный вектор расстояния между двумя частицами
  physvalue Fhx = dx / lnew * Fh;
  physvalue Fhy = dy / lnew * Fh;

  Fhx *= hookDecay;
  Fhy *= hookDecay;

  // прибавляем составляющие силы Гука к сумме всех сил, действующих на частицы, соединяемые пружиной
  p1->Fx += Fhx;  // к одной частице со знаком +
  p1->Fy += Fhy;

  p2->Fx -= Fhx;  // к другой частице со знаком -
  p2->Fy -= Fhy;
}
