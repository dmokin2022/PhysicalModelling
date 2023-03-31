
#include "Particle.hpp"

#include <QObject>
#include <cmath>

Particle::Particle(physvalue x, physvalue y, physvalue r, physvalue m, physvalue q) {
  this->x     = x;
  this->y     = y;
  this->z     = 0;
  this->r     = r;
  this->m     = m;
  this->q     = q;
  this->color = Qt::red;

  Fx = 0;
  Fy = 0;
  ax = 0;
  ay = 0;

  vx = 0;
  vy = 0;

  connectedWithNeighbours = false;  // признак связанности частицы с соседними частицами
      // (нужно для расчёта поведения упругого тела из частиц)
  springLinksCounter = 0;  // счётчик числа связей частицы с другими частицами
}

physvalue Particle::getSumForce() { return 1; }

void Particle::computeAcceleration() {
  ax = Fx / m;
  ay = Fy / m;
}

void Particle::computeVelocity() {
  vx = vx + ax * dt_;
  vy = vy + ay * dt_;
}

void Particle::computeMovement() {
  x = x + vx * dt_;
  y = y + vy * dt_;
}

bool Particle::isIncludingPoint(physvalue px, physvalue py)
{
    bool result = false;
    if (((x - r) < px) && (px < (x + r))) {
        if (((y - r) < py) && (py < (y + r))) {
            result = true;
        }
    }
    return result;
}
