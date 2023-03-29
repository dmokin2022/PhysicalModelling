#pragma once

#include <QPen>
#include <cmath>

#include "PhysicalValues.hpp"

class Particle {
  //class Particle: public QObject {
  //    Q_OBJECT
public:
  physvalue x;  // Координаты
  physvalue y;
  physvalue z;

  physvalue r;  // радиус
  physvalue m;  // масса

  physvalue Fx;  // проекции силы
  physvalue Fy;
  physvalue ax;  // проекции ускорения
  physvalue ay;
  physvalue vx;  // проекции скорости
  physvalue vy;

  QColor color;
  //  QPen pen;
  //  QBrush brush;
  //  QPen(Qt::black),
  //  QBrush (Qt::green)

  int springLinksCounter;  // счётчик числа связей частицы с другими частицами
  bool connectedWithNeighbours = false;

  Particle(physvalue x, physvalue y, physvalue r = 1, physvalue m = 1);
  virtual ~Particle() {}

  physvalue getSumForce();
  void computeAcceleration();
  void computeVelocity();
  void computeMovement();
};
