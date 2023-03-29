#include "Space.hpp"

Space::Space() {
  sumR = 0;  // сумма радиусов двух расчётных частиц
  dx = 0;  // разность по x между двумя расчётными частицами
  dy = 0;  // разность по y между двумя расчётными частицами
  sqrL = 0;  // квадрат расстояния между двумя расчётными частицами
  L    = 0;  // расстояние между двумя расчётными частицами

  this->dt       = dt_;  // время просчёта одного кадра
  framesCount    = 0;    // количество просчитанных кадров
  particlesCount = 0;    // количество частиц

  // Ускорение свободного падения (гравитация) по осям
  gx = 0;
  gy = 9.8;

  decay = 0.99;  // Коэффициент затухания (используется при соударениях)
}

// Добавление одной частицы
void Space::addParticle(physvalue x, physvalue y, physvalue r, physvalue m) {
  //particles.append(Particle(x, y, r, m));
  //Particle p(x, y, r, m);
  Particle *p = new Particle(x, y, r, m);
  particles.append(p);  // в массив помещается копия объекта

  //  p->x = 1000;  //
  //  p->y = 1000;
  //  delete p;
}

// Добавление квадратного массива из одинаковых частиц
void Space::addParticleArray(
    physvalue x, physvalue y, physvalue r, physvalue m, int count, physvalue distance) {
  physvalue squareSide = floor(sqrt(count));

  for (int i = 0; i < squareSide; i++)
    for (int j = 0; j < squareSide; j++) addParticle(x + i * distance, y + j * distance, r, m);
}

void Space::addSpring(Particle p1, Particle p2, physvalue k) {
  //
  springs.append(Spring(p1, p2, k));
}

void Space::addSpringsToParticlesGroup(
    physvalue x1, physvalue y1, physvalue x2, physvalue y2, physvalue r, physvalue k) {
  // создание отдельного списка для частиц, входящих в выделенный прямоугольник
  QVector<Particle *> connectedParticles;

  // Проходим циклом по всем частицам
  for (auto particle : particles) {
    // Если частица попадает в заданный прямоугольник
    if ((particle->x > x1) & (particle->x < x2))
      if ((particle->y > y1) & (particle->y < y2)) connectedParticles.append(particle);
    //for (int i = 0; i < particles.size(); i++) {
    //if ((particles[i].x > x1) & (particles[i].x < x2))
    //if ((particles[i].y > y1) & (particles[i].y < y2))
    // Записываем частицу в специальный список
    //connectedParticles.append(particles[i]);
  }

  // Проходим циклом по всем выбранным частицам
  for (int i = 0; i < connectedParticles.size(); i++) {
    Particle *p1 = connectedParticles[i];
    for (int j = i + 1; j < connectedParticles.size(); j++) {
      Particle *p2 = connectedParticles[j];
      if (!p2->connectedWithNeighbours) {
        // Определяем расстояние между частицами
        dx   = p2->x - p1->x;
        dy   = p2->y - p1->y;
        sqrL = dx * dx + dy * dy;

        if (sqrL < r * r) {  // Если расстояние между частицами меньше заданного r, то...
          //... устанавливаем пружинную связь между частицами
          //Spring spring = Spring(p1, p2, k);
          springs.append(Spring(*p1, *p2, k));  // сохраняем пружину в список пружин
          p1->springLinksCounter += 1;
        }
      }
    }

    // Если были установлены какие либо связи с другими частицами, то устанавливаем признак присутствия связей у частицы
    if (p1->springLinksCounter) p1->connectedWithNeighbours = true;
  }
}

// Расчёт силы гравитации на частицу
void Space::computeGravityForce(Particle &p1) {
  p1.Fx += p1.m * gx;
  p1.Fy += p1.m * gy;
}

void Space::computeAllForces() {
  // Цикл прохода по всем частицам в пространстве
  for (auto particle : particles) {
    // Обнуляем все силы, действующие на частицу для дальнейшего перерасчёта
    particle->Fx = 0;
    particle->Fy = 0;
    computeGravityForce(*particle);  // расчёт силы гравитации, действующей на частицу
  }

  // Цикл прохода по всем пружинам в списке
  for (auto &spring : springs) {
    // вычисляем силу упругости по закону Гука
    spring.computeHookForce();
  }
}

// Функция расчёта столкновения двух частиц
void Space::computeCollisionBetwinParticles(Particle &p1, Particle &p2) {
  // Проверка знаменателя дроби на 0 (чтобы избежать ошибки деления на 0)
  if (dx <= almostZero) { dx = almostZero; }

  // Расчёт тригонометрических функций для вычисления проекций скрости
  physvalue tana = dy / dx;
  physvalue cosa = 1 / sqrt(tana * tana + 1);
  physvalue sina = tana * cosa;

  // Расчёт проекций составляющих скорости на ось взаимодействия
  // (используется для дальнейших расчётов столкновения)
  physvalue v1px = p1.vx * cosa + p1.vy * sina;
  physvalue v2px = p2.vx * cosa + p2.vy * sina;

  // Расчёт проекций составляющих скорости на ось перпендикулярную взаимодействию
  // (не принимает участия в столкновении, скорости не меняются)
  physvalue v1py = p1.vy * cosa - p1.vx * sina;
  physvalue v2py = p2.vy * cosa - p2.vx * sina;

  // Расчёт столкновения
  // Вычислвение промежуточных значений
  physvalue p  = p1.m * v1px + p2.m * v2px;  // Расчёт суммарного импульса частиц до столкновения
  physvalue dv = v1px - v2px;  // Расчёт разности скорости частиц до столкновения

  physvalue v2pxnew = (p + p1.m * dv) / (p1.m + p2.m);
  physvalue v1pxnew = v2pxnew - v1px + v2px;

  // Расчёт проекций новых составляющих скорости на реальные оси x, y
  p1.vx = v1pxnew * cosa - v1py * sina;
  p2.vx = v2pxnew * cosa - v2py * sina;

  p1.vy = v1pxnew * sina + v1py * cosa;
  p2.vy = v2pxnew * sina + v2py * cosa;

  // учёт затухания при соударениях
  // p1.vx *= decay;
  // p1.vy *= decay;
  //
  // p2.vx *= decay;
  // p2.vy *= decay;
}

// Функция "разлепивания" частиц если они наползают друг на друга
// (упрощение вместо "правильного" расчёта столкновения между кадрами)
// TODO Разобраться с правильным расчётом с учётом статических переменных
// ОШИБКА!!!
void Space::unstickPartcles(Particle &p1, Particle &p2) {
  physvalue d   = p1.r + p2.r - L;
  physvalue k   = 0.5 * d / L;
  physvalue dx_ = k * dx;  // насколько вернуть частицы по x
  physvalue dy_ = k * dy;  // насколько вернуть частицы по y

  // Изменяем координаты частицы 1
  p1.x = p1.x + dx_;
  p1.y = p1.y + dy_;

  // Изменяем координаты частицы 2
  p2.x = p2.x - dx_;
  p2.y = p2.y - dy_;
}

void Space::detectCollisionBetwinParticles(Particle &p1, Particle &p2) {
  physvalue sumR = p1.r + p2.r;  // сумма радиусов двух частиц
  physvalue dx   = p1.x - p2.x;  // разность по x
  physvalue dy   = p1.y - p2.y;  // разность по y

  if (abs(dx) <= sumR) {
    if (abs(dy) <= sumR) {
      sqrL = dx * dx + dy * dy;  // расчёт квадрата расстояния между частицами
      L    = sqrt(sqrL);
      if (sqrL <= sumR * sumR) {
        unstickPartcles(p1, p2);
        computeCollisionBetwinParticles(p1, p2);
      }
    }
  }
}

void Space::detectCollisionWithWalls(Particle &p1) {
  // Если частица "сталкивается" с левой стеной
  if ((p1.x - p1.r) <= wall.left) {
    // Расчёт соударения с левой/правой стеной
    p1.vx = -p1.vx;  // Меняем векторор скорости по x на противоположный
    p1.vx *= decay;
    p1.x = wall.left + p1.r;  // "вытаскиваем" из стены частицу если она успела в неё "проникнуть"
  }

  // Если частица "сталкивается" с правой стеной
  if ((p1.x + p1.r) >= wall.right) {
    // Расчёт соударения с правой стеной
    p1.vx = -p1.vx;  // Меняем векторор скорости по x на противоположный
    p1.vx *= decay;
    p1.x = wall.right - p1.r;  // "вытаскиваем" из стены частицу если она успела в неё "проникнуть"
  }

  // Если частица "сталкивается" с верхней стеной
  if ((p1.y - p1.r) <= wall.top) {
    p1.vy = -p1.vy;  // Меняем векторор скорости по y на противоположный
    p1.vy *= decay;
    p1.y = wall.top + p1.r;  // "вытаскиваем" из стены частицу если она успела в неё "проникнуть"
  }

  // Если частица "сталкивается" с нижней стеной
  if ((p1.y + p1.r) >= wall.bottom) {
    // Расчёт соударения с верхней и нижней стеной
    p1.vy = -p1.vy;  // Меняем векторор скорости по y на противоположный
    p1.vy *= decay;
    p1.y = wall.bottom - p1.r;  // "вытаскиваем" из стены частицу если она успела в неё "проникнуть"
  }
}

// def detectCollisionWithForms(self):
//     pass

// def detectAllCollisions(self):
//     // Цикл прохода по всем частицам в списке
//     for i in range(len(particles)):
//         p1 = particles[i]
//         detectCollisionWithWalls(p1)
//         // calculateAllForces()
//
//         // Цикл прохода по всем частицам в списке, кроме тех по которым уже прошли
//         for k in range(i+1, len(particles)):
//             p2 = particles[k]
//
//             detectCollisionBetwinParticles(p1, p2)

void Space::computeTimeFrame() {
  computeAllForces();  // расчёт всех, действующих на частицу сил

  // Цикл прохода по всем частицам в списке
  for (int i = 0; i < particles.size(); i++) {
    // p1 = Particle();
    Particle *p1 = particles[i];

    // Расчёт сил гравитации на частицу (действует на каждую частицу)
    // computeGravityForce(p1)

    // Поиск соударений частиц со стенками пространства
    detectCollisionWithWalls(*p1);

    // Цикл прохода по всем частицам в списке, кроме тех по которым уже прошли
    for (int k = i + 1; k < particles.size(); k++) {
      Particle *p2 = particles[k];

      detectCollisionBetwinParticles(*p1, *p2);
    }

    // Расчёт нового положения частицы исходя из всех учтённых сил и взаимодействий
    p1->computeAcceleration();
    p1->computeVelocity();
    p1->computeMovement();
  }
}

//if __name__ == '__main__':
//    space = Space();
//    space.addParticle(0, 0, 1, 1)
//    space.addParticle(10, 10, 1, 1)
//    space.detectCollision()
