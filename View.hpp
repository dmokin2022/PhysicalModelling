#pragma once
#include <QBrush>
#include <QColor>
#include <QGraphicsItemGroup>
#include "graphicsscene.h"
#include <QPainter>
#include <QTimer>

#include "Model/Box.hpp"
#include "Model/Particle.hpp"
#include "Model/Space.hpp"
#include "Model/Spring.hpp"

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

  // Создаём объект для графической сцены
  GraphicsScene *scene;
  QGraphicsItemGroup *boxGroup;
  QGraphicsItemGroup *particlesGroup;

  //  QPainter *painter;
  //  QPen *pen;
  //  QColor &color;
  //  QBrush &brush;

  // Настройка таймера для генерации кадров
  QTimer timer;

  View();
  ~View() {};

  void initDraw();

  void drawModel();
  qreal physXfromScene(qreal x);
  qreal physYfromScene(qreal y);
  Particle* getParticleAtAlloc(qreal x, qreal y);
  void test();

public slots:
  void showFrame();  // обработчик таймера
  // Обработчика нажатия на кнопку Пуск/Стоп
  void oneStepSimulation();
  void togleSimulation();
  void restart();

};

