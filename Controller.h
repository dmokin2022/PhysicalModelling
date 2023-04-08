#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "View.hpp"
#include "Model/Space.hpp"

#include <QObject>


enum Operation {
    NOP,
    ADDING_PARTICLE,
    DELETING_PARTICLE,
    CHANGING_PARTICLE,
    ADDING_SPRING,
    DELETING_SPRING,
};

class Controller: public QObject {

    Q_OBJECT
public:
    Controller(Space& space, View& view);

    // Построение готовых тестовых структур
    void testWithSprings();
    void testParticlesOnly();

    // Переключение режимов редактирования
    void toggleAddingParticleMode();
    void toggleAddingSpringMode();
    void toggleDeleteMode();

    // Команды редактирования
    Particle* addParticleAtAlloc(qreal x, qreal y);
    void deleteSelectedParticle();
    Spring* addSpringBetweenParticles(Particle &p1, Particle &p2);
    void deleteSpringAtCursorPosition();
    void editOperationAtAlloc(qreal x, qreal y);

private:
    int lastOperation;

    struct {
        bool addingParticle;
        bool addingSpring;
        bool deleting;
    } mode;

    View *view;
    Space *space;
};

#endif // CONTROLLER_H
