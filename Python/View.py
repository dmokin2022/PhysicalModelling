from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QGraphicsScene, QGraphicsItemGroup

import Model

class View:

    def __init__(self):
        self.scale: float = 10 # Масштабирующий коэффициент. Сколько точек экрана приходится на единичный отрезок в пространстве модели

        self.offsetX: float = 0    # Точка пространства модели, отображаемая в левой верхней части экрана (смщещение по x и y)
        self.offsetY: float = 0
        self.framesPerSecond = 100   # Количество кадров моделирования в секунду

        self.simulationIsStarted = False    # Флаг признака того, что процесс симуляции запущен

        self.space = Model.Space()

        # Создаём объект для графической сцены
        self.scene = QGraphicsScene()

        # Настройка таймера для генерации кадров
        self.timer = QTimer()
        self.timer.timeout.connect(self.showFrame)  # Подключаем функцию, которая будет запускаться по таймеру
        self.timer.start(1000/self.framesPerSecond)   # повторяется каждые 1000/frames милисекунд


    def initDraw(self):
        # Создание графической группы для стенок пространства
        self.wallsGroup = QGraphicsItemGroup()
        self.scene.addItem(self.wallsGroup)  # Добавляем группу к сцене

        # Прорисовка стенок пространства
        # Расчёт экранных координат и размеров объектов для отображения
        sx = (self.space.wall.left - self.offsetX) * self.scale
        sy = (self.space.wall.top - self.offsetY) * self.scale

        sw = (self.space.wall.right - self.space.wall.left) * self.scale
        sh = (self.space.wall.bottom - self.space.wall.top) * self.scale

        # self.scene.addRect(sx, sy, sw, sh)
        self.wallsGroup.addToGroup(self.scene.addRect(sx, sy, sw, sh))

        # Создаём группу для отрисовки частиц
        self.particlesGroup = QGraphicsItemGroup()


    def drawModel(self):
        self.scene.clear()  # Очистка сцены для перерисовки
        self.initDraw()

        # Создание новой группы для перерисовки объектов
        self.particlesGroup = QGraphicsItemGroup()
        self.particlesGroup.scene()

        # # Создаём графическую сцену
        self.scene.addItem(self.particlesGroup)  # Добавляем группу к сцене

        # Цикл прохода по всем частицам в пространстве
        for i in range(len(self.space.particles)):
            p: object = self.space.particles[i]

            # Расчёт экранных координат и размеров объектов для отображения
            sx = (p.x - p.r - self.offsetX)*self.scale
            sy = (p.y - p.r - self.offsetY)*self.scale

            sr = p.r*self.scale
            sw = sh = 2*sr

            # добавляем эллипс/круг в экранную группу изображений
            self.particlesGroup.addToGroup(self.scene.addEllipse(sx, sy, sw, sh))

            # Выводим координаты частицы для отладки
            # xystr = str(str(p.x) + "," + str(p.y))
            # self.particlesGroup.addToGroup(self.scene.addText(xystr))

        # Цикл прохода по всем пружинам в пространстве
        for i in range(len(self.space.springs)):
            # пружины отображаются как простые отрезки, соединяющие центры частиц
            spring = self.space.springs[i]

            # расчёт экранных координат концов отрезка пружины
            sx1 = (spring.p1.x - self.offsetX)*self.scale
            sy1 = (spring.p1.y - self.offsetY)*self.scale

            sx2 = (spring.p2.x - self.offsetX)*self.scale
            sy2 = (spring.p2.y - self.offsetY)*self.scale

            # добавляем линию в экранную группу изображений
            self.particlesGroup.addToGroup(self.scene.addLine(sx1, sy1, sx2, sy2))


        # win.graphicsView.setScene(scene)

    def showFrame(self):
        if self.simulationIsStarted:
            self.space.computeTimeFrame()
            self.drawModel()

    def test(self):
        self.scene.addText("Hello, world!")
        self.scene.addEllipse(0, 0, 20, 20)

        self.space.addParticleArray(0, 0, 1, 1, 7*7, 3)

        self.space.addSpringsToParticlesGroup(-1, -1, 100, 100, 5, 1000)
        self.initDraw()
        self.drawModel()


if __name__ == '__main__':
    view = View()
    view.test()
