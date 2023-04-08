#include "Controller.h"

Controller::Controller(Space& space, View& view)
{
    this->space = &space;
    this->view = &view;
    mode.addingSpring = false;
    mode.addingParticle = false;
    mode.deleting = false;
    lastOperation = Operation::NOP;
}


void Controller::testWithSprings() {
//  scene->addText("Hello, world!");
//  scene->addEllipse(0, 0, 20, 20);

  space->addParticleArray(0, 0, 1, 1, 10 * 10, 3);

  space->addSpringsToParticlesGroup(-1, -1, 100, 100, 5, 1000);
  view->initDraw();
  view->drawModel();
}

void Controller::testParticlesOnly() {
  space->addParticleArray(0, 0, 1, 1, 10 * 10, 3);

  view->initDraw();
  view->drawModel();
}


void Controller::toggleAddingParticleMode() {
    mode.addingParticle = !mode.addingParticle;
}

void Controller::toggleAddingSpringMode() {
    mode.addingSpring = !mode.addingSpring;
}

void Controller::toggleDeleteMode() {
    mode.deleting = !mode.deleting;
}

Particle* Controller::addParticleAtAlloc(qreal x, qreal y) {
    Particle *particleAtAlloc = view->getParticleAtAlloc(x, y);
    // Если в нужной позиции есть частица
    if (particleAtAlloc) {
        // меняем её свойства на свойства образца
        *particleAtAlloc = *(view->templateParticle);

        particleAtAlloc->x = view->physXfromScene(x);
        particleAtAlloc->y = view->physYfromScene(y);
        lastOperation = Operation::CHANGING_PARTICLE;
    } else {
        // если частицы нет, то добавляем её по шаблону
        particleAtAlloc = space->addParticle(*view->templateParticle);
        // установка правильных координат
        particleAtAlloc->x = view->physXfromScene(x);
        particleAtAlloc->y = view->physYfromScene(y);
        lastOperation = Operation::ADDING_PARTICLE;
    }

    // TODO операция "расталкивания" частиц
    // если они помещены слишком близко друг к другу
    // ...

    view->selectedParticle = particleAtAlloc;
    view->drawModel();

    return particleAtAlloc;
}

void Controller::editOperationAtAlloc(qreal x, qreal y) {
    if (mode.addingParticle) {
        addParticleAtAlloc(x, y);
    }

    if (mode.addingSpring) {
        //addSpringBetweenParticles();
    }

    if (mode.deleting) {
        view->getParticleAtAlloc(x, y);
        deleteSelectedParticle();
    }
}

void Controller::deleteSelectedParticle() {
    int i = 0;
    if (view->selectedParticle) {
        for (auto p : space->particles) {
            if (p == view->selectedParticle) {
                delete p;
                space->particles.remove(i);
                view->selectedParticle = nullptr;
                lastOperation = Operation::DELETING_PARTICLE;
                break;
            }
            i++;
        }
    }

    view->drawModel();
}

Spring* addSpringBetweenParticles(Particle &p1, Particle &p2) {
    Q_UNUSED(p1)
    Q_UNUSED(p2)
    return nullptr;
}

void deleteSpringAtCursorPosition() {

}

