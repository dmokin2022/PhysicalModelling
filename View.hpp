#pragma once
#include <QBrush>
#include <QColor>
#include <QGraphicsItemGroup>
#include <QPainter>
#include <QTimer>

#include "Model/Box.hpp"
#include "Model/Particle.hpp"
#include "Model/Space.hpp"
#include "Model/Spring.hpp"
#include "graphicsscene.h"

class View : public QObject {
  Q_OBJECT
public:
  // Масштабирующий коэффициент. Сколько точек экрана приходится на единичный отрезок в пространстве модели
  physvalue scale;

  // Точка пространства модели, отображаемая в левой верхней части экрана (смщещение по x и y)
  physvalue offsetX = 0;
  physvalue offsetY = 0;
  //physvalue framesPerSecond = 100;  // Количество кадров моделирования в секунду

  bool simulationIsStarted = false;  // Флаг признака того, что процесс симуляции запущен

  Space space;
  Particle *selectedParticle;
  Particle *templateParticle;  // шаблонная частица с типовыми значениями
  Spring *selectedSpring;
  Spring *templateSpring;  // шаблонная пружина

  // Oбъект для графической сцены
  GraphicsScene *scene;
  QGraphicsItemGroup *boxGroup;
  QGraphicsItemGroup *particlesGroup;

  // Настройка таймера для генерации кадров
  QTimer timer;

  View();
  ~View() {};

  void initDraw();

  void drawModel();

  qreal physXfromScene(qreal x);
  qreal physYfromScene(qreal y);
  qreal sceneXfromPhys(qreal x);
  qreal sceneYfromPhys(qreal y);

  Particle *getParticleAtAlloc(qreal x, qreal y);

  void testWithSprings();
  void testParticlesOnly();

public slots:
  void showFrame();  // обработчик таймера
  // Обработчика нажатия на кнопку Пуск/Стоп
  void oneStepSimulation();
  void togleSimulation();
  void restart();

private:
  void drawParticle(Particle *p);
  void drawSpring(Spring spring);
  void drawVectorAt(Particle &particle);
};
