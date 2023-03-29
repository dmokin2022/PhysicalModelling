
#include <QBrush>
#include <QColor>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QPainter>
#include <QTimer>

#include "Model/A_Particle.hpp"
#include "Model/B_Spring.hpp"
#include "Model/Box.hpp"
#include "Model/D_Space.hpp"

class View : public QObject {
  Q_OBJECT
public:
  // Масштабирующий коэффициент. Сколько точек экрана приходится на единичный отрезок в пространстве модели
  physvalue scale;

  // Точка пространства модели, отображаемая в левой верхней части экрана (смщещение по x и y)
  physvalue offsetX = 0;
  physvalue offsetY = 0;
  physvalue framesPerSecond = 100;  // Количество кадров моделирования в секунду

  bool simulationIsStarted = false;  // Флаг признака того, что процесс симуляции запущен

  Space space;

  // Создаём объект для графической сцены
  QGraphicsScene *scene;
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

  void test();

public slots:
  void showFrame();  // обработчик таймера
  // Обработчика нажатия на кнопку Пуск/Стоп
  void oneStepSimulation();
  void togleSimulation();
};

//if __name__ == '__main__':
//    view = View()
//    view.test()
