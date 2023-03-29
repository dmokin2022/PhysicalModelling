
#include "View.hpp"

View::View() {
  // Масштабирующий коэффициент. Сколько точек экрана приходится на единичный отрезок в пространстве модели
  scale = 10;
  // Точка пространства модели, отображаемая в левой верхней части экрана (смщещение по x и y)
  offsetX         = 0;
  offsetY         = 0;
  framesPerSecond = 20;  // Количество кадров моделирования в секунду

  simulationIsStarted = false;  // Флаг признака того, что процесс симуляции запущен

  //space = Space();

  // Создаём объект для графической сцены
  scene = new QGraphicsScene();

  //  painter = new QPainter();
  //  pen     = new QPen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
  //  painter->setPen(*pen);

  // Настройка таймера для генерации кадров
  // Подключаем функцию, которая будет запускаться по таймеру
  connect(&this->timer, &QTimer::timeout, this, &View::showFrame);
  timer.start(1000 / framesPerSecond);  // повторяется каждые 1000/frames милисекунд
}

void View::initDraw() {
  // Создание графической группы для стенок пространства
  boxGroup = new QGraphicsItemGroup();
  scene->addItem(boxGroup);  // Добавляем группу к сцене

  // Прорисовка стенок пространства
  // Расчёт экранных координат и размеров объектов для отображения
  physvalue sx = (space.wall.left - offsetX) * scale;
  physvalue sy = (space.wall.top - offsetY) * scale;

  physvalue sw = (space.wall.right - space.wall.left) * scale;
  physvalue sh = (space.wall.bottom - space.wall.top) * scale;

  // scene.addRect(sx, sy, sw, sh)
  boxGroup->addToGroup(scene->addRect(sx, sy, sw, sh));

  // Создаём группу для отрисовки частиц
  //particlesGroup = new QGraphicsItemGroup();
}

void View::drawModel() {
  scene->clear();  // Очистка сцены для перерисовки
  initDraw();

  // Создание новой группы для перерисовки объектов
  particlesGroup = new QGraphicsItemGroup();
  particlesGroup->scene();

  // Создаём графическую сцену
  scene->addItem(particlesGroup);  // Добавляем группу к сцене

  // Цикл прохода по всем частицам в пространстве
  for (auto &p : space.particles) {
    //Particle p = space.particles[i];

    // Расчёт экранных координат и размеров объектов для отображения
    int sx = (p->x - p->r - offsetX) * scale;
    int sy = (p->y - p->r - offsetY) * scale;

    int sr = p->r * scale;
    int sh = 2 * sr;
    int sw = sh;

    // добавляем эллипс/круг в экранную группу изображений
    particlesGroup->addToGroup(scene->addEllipse(sx, sy, sw, sh));

    // Выводим координаты частицы для отладки
    // xystr = str(str(p.x) + "," + str(p.y))
    // particlesGroup.addToGroup(scene.addText(xystr))
  }

  // Цикл прохода по всем пружинам в пространстве
  for (auto &spring : space.springs) {
    // пружины отображаются как простые отрезки, соединяющие центры частиц
    //Spring spring = space.springs[i];

    // расчёт экранных координат концов отрезка пружины
    int sx1 = (spring.p1->x - offsetX) * scale;
    int sy1 = (spring.p1->y - offsetY) * scale;

    int sx2 = (spring.p2->x - offsetX) * scale;
    int sy2 = (spring.p2->y - offsetY) * scale;

    // добавляем линию в экранную группу изображений
    particlesGroup->addToGroup(scene->addLine(sx1, sy1, sx2, sy2));
  }

  // win.graphicsView.setScene(scene)
}

void View::showFrame() {
  if (simulationIsStarted) {
    space.computeTimeFrame();
    drawModel();
  }
}

void View::test() {
  scene->addText("Hello, world!");
  scene->addEllipse(0, 0, 20, 20);

  space.addParticleArray(0, 0, 1, 1, 7 * 7, 3);

  space.addSpringsToParticlesGroup(-1, -1, 100, 100, 5, 1000);
  initDraw();
  drawModel();
}

// Обработчика нажатия на кнопку Пуск/Стоп
void View::oneStepSimulation() {
  space.computeTimeFrame();
  drawModel();
}

void View::togleSimulation() { simulationIsStarted = !simulationIsStarted; }

//if __name__ == '__main__':
//    view = View()
//    view.test()
