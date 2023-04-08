
#include "View.hpp"

View::View() {
  // Масштабирующий коэффициент. Сколько точек экрана приходится на единичный отрезок в пространстве модели
  scale = 10;
  // Точка пространства модели, отображаемая в левой верхней части экрана (смщещение по x и y)
  offsetX = 0;
  offsetY = 0;
  //framesPerSecond = 20;  // Количество кадров моделирования в секунду

  simulationIsStarted = false;  // Флаг признака того, что процесс симуляции запущен

  templateParticle = new Particle(0, 0, 1, 1, 0, Qt::red);
  selectedParticle = nullptr;
  // Создаём объект для графической сцены
  scene = new GraphicsScene();

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

  // Цикл прохода по всем пружинам в пространстве
  for (auto &spring : space.springs) { drawSpring(spring); }

  // Цикл прохода по всем частицам в пространстве
  for (auto &p : space.particles) {
    drawParticle(p);
    drawVectorAt(*p);
  }

  //if (selectedParticle) drawVectorAt(*selectedParticle);

  // win.graphicsView.setScene(scene)
}

void View::drawParticle(Particle *p) {
  //Particle p = space.particles[i];

  // Расчёт экранных координат и размеров объектов для отображения
  int sx = (p->x - p->r - offsetX) * scale;
  int sy = (p->y - p->r - offsetY) * scale;

  int sr = p->r * scale;
  int sh = 2 * sr;
  int sw = sh;

  // добавляем эллипс/круг в экранную группу изображений
  QPen pen(p->color);
  QBrush brush;

  if (p->isFilledWithColor) { brush = QBrush(p->color); }

  // Рисование рамки вокруг выбранной частцы
  if (p == selectedParticle) {
      //  painter = new QPainter();
      pen = QPen(Qt::gray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
      //pen = QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
      //  painter->setPen(*pen);
  }

  particlesGroup->addToGroup(scene->addEllipse(sx, sy, sw, sh, pen, brush));

  // Выводим координаты частицы для отладки
  // xystr = str(str(p.x) + "," + str(p.y))
  // particlesGroup.addToGroup(scene.addText(xystr))
}

void View::drawSpring(Spring spring) {
  // пружины отображаются как простые отрезки, соединяющие центры частиц
  //Spring spring = space.springs[i];

  // расчёт экранных координат концов отрезка пружины
  int sx1 = (spring.p1->x - offsetX) * scale;
  int sy1 = (spring.p1->y - offsetY) * scale;

  int sx2 = (spring.p2->x - offsetX) * scale;
  int sy2 = (spring.p2->y - offsetY) * scale;

  QPen pen(Qt::red);
  // добавляем линию в экранную группу изображений
  particlesGroup->addToGroup(scene->addLine(sx1, sy1, sx2, sy2, pen));
}

void View::drawVectorAt(Particle &p) {
  QPen pen(Qt::green);
  //QBrush brush;
  qreal vr  = p.r / p.getV();
  qreal vxr = p.vx * vr;
  qreal vyr = p.vy * vr;

  // Прорисовка стенок пространства
  // Расчёт экранных координат и размеров объектов для отображения
  physvalue x1 = (p.x - offsetX) * scale;
  physvalue y1 = (p.y - offsetY) * scale;

  physvalue x2 = (p.x + vxr - offsetX) * scale;
  physvalue y2 = (p.y + vyr - offsetY) * scale;

  particlesGroup->addToGroup(scene->addLine(x1, y1, x2, y2, pen));
}

//physvalue View::

qreal View::physXfromScene(qreal x) { return (x - offsetX) / scale; }

qreal View::physYfromScene(qreal y) { return (y - offsetY) / scale; }

Particle *View::getParticleAtAlloc(qreal x, qreal y) {
  //Particle* result = nullptr;
  for (auto particle : space.particles) {
    if (particle->isIncludingPoint(x, y)) {
      selectedParticle = particle;
      return particle;
    }
  }

  return nullptr;
}

void View::showFrame() {
  if (simulationIsStarted) {
    space.computeTimeFrame();
    drawModel();
  }
}

void View::testWithSprings() {
  scene->addText("Hello, world!");
  scene->addEllipse(0, 0, 20, 20);

  space.addParticleArray(0, 0, 1, 1, 7 * 7, 3);

  space.addSpringsToParticlesGroup(-1, -1, 100, 100, 5, 1000);
  initDraw();
  drawModel();
}

void View::testParticlesOnly() {
  space.addParticleArray(0, 0, 1, 1, 10 * 10, 3);

  initDraw();
  drawModel();
}

// Обработчика нажатия на кнопку Пуск/Стоп
void View::oneStepSimulation() {
  space.computeTimeFrame();
  drawModel();
}

void View::togleSimulation() { simulationIsStarted = !simulationIsStarted; }

void View::restart() {
  this->space.deleteAll();
  this->testParticlesOnly();
}

//if __name__ == '__main__':
//    view = View()
//    view.test()
