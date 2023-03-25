#include "A_Particle.hpp"
#include "B_Spring.hpp"
#include "C_Box.hpp"

// Описание пространства для физического взаимодействия
class Space: public QObject {
public:
    // Статические переменные для расчётов
    physvalue sumR = 0; // сумма радиусов двух расчётных частиц
    physvalue dx = 0;   // разность по x между двумя расчётными частицами
    physvalue dy = 0;   // разность по y между двумя расчётными частицами
    physvalue sqrL = 0; // квадрат расстояния между двумя расчётными частицами
    physvalue L = 0;    // расстояние между двумя расчётными частицами

    // Ускорение свободного падения (гравитация) по осям
    physvalue gx = 0;
    physvalue gy = 9.8;
    physvalue decay;   // Коэффициент затухания (используется при соударениях)


    physvalue dt;   // время просчёта одного кадра

    int framesCount; // количество просчитанных кадров
    int particlesCount; // количество частиц


    Box wall;

    QVector<Particle> particles;    // Список всех частиц, присутствующих в пространстве
    QVector<Spring> springs;        // Список всех пружин, присутствующих в пространстве

    Space();

    // Добавление одной частицы
    void addParticle(physvalue x, physvalue y, physvalue r, physvalue m);

    // Добавление квадратного массива из одинаковых частиц
    void addParticleArray(physvalue x, physvalue y, physvalue r, physvalue m, int count, physvalue distance);

    void addSpring(Particle p1, Particle p2, physvalue k);

    void addSpringsToParticlesGroup(physvalue x1, physvalue y1,
                                    physvalue x2, physvalue y2,
                                    physvalue r, physvalue k);
    // Расчёт силы гравитации на частицу
    void computeGravityForce(Particle &p1);

    void computeAllForces();

    // Функция расчёта столкновения двух частиц
    void computeCollisionBetwinParticles(Particle &p1, Particle &p2);

    // Функция "разлепивания" частиц если они наползают друг на друга
    // (упрощение вместо "правильного" расчёта столкновения между кадрами)
    void unstickPartcles(Particle &p1, Particle &p2);

    void detectCollisionBetwinParticles(Particle &p1, Particle &p2);

    void detectCollisionWithWalls(Particle &p1);

    void computeTimeFrame();
};

//if __name__ == '__main__':
//    space = Space();
//    space.addParticle(0, 0, 1, 1)
//    space.addParticle(10, 10, 1, 1)
//    space.detectCollision()







