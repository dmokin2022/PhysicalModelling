import math


class Particle:
    dt = 0.01

    def __init__(self, x, y, r=1, m=1):
        self.x: float = x
        self.y: float = y
        self.r: float = r
        self.m: float = m

        self.Fx: float = 0
        self.Fy: float = 0

        self.ax: float = 0
        self.ay: float = 0

        self.vx: float = 0
        self.vy: float = 0

        self.connectedWithNeighbours: bool = False    # признак связанности частицы с соседними частицами
                                                # (нужно для расчёта поведения упругого тела из частиц)
        self.springLinksCounter = 0 # счётчик числа связей частицы с другими частицами

    def getSumForce(self):
        return 1

    def computeAcceleration(self):
        self.ax = self.Fx/self.m
        self.ay = self.Fy/self.m

    def computeVelocity(self):
        self.vx = self.vx + self.ax*self.dt
        self.vy = self.vy + self.ay*self.dt

    def computeMovement(self):
        self.x = self.x + self.vx*self.dt
        self.y = self.y + self.vy*self.dt

# Объект пружина для создания упругих тел
class Spring:
    def __init__(self, p1, p2, k=1):
        self.p1 = p1
        self.p2 = p2
        self.k: float = k
        dx = p2.x - p1.x
        dy = p2.y - p1.y
        self.l: float = math.sqrt(dx*dx + dy*dy)    # длина пружины. Опрделяется по начальному расстоянию между частицами
        pass

    def computeHookForce(self):
        # Определяем текущее расстояние между частицами
        dx = self.p2.x - self.p1.x
        dy = self.p2.y - self.p1.y
        lnew = math.sqrt(dx*dx + dy*dy)

        # Вычисляем разницу между текущим расстоянием и тем, что было при создании пружины
        dl = lnew - self.l  # если пружина сжалась, то dl отрицательное, если растянулась, то dl положительное

        Fh = self.k*dl  # Вычисляем силу упругости Гука по закону Гука

        # вычисление составляющих силы Гука через единичный вектор расстояния между двумя частицами
        Fhx = dx/lnew*Fh
        Fhy = dy/lnew*Fh

        # прибавляем составляющие силы Гука к сумме всех сил, действующих на частицы, соединяемые пружиной
        self.p1.Fx += Fhx   # к одной частице со знаком +
        self.p1.Fy += Fhy

        self.p2.Fx -= Fhx   # к другой частице со знаком -
        self.p2.Fy -= Fhy


class Wall:
    top: float = -10
    bottom: float = 20
    left: float = -10
    right: float = 40

almostZero = 0.0001

# Описание пространства для физического взаимодействия
class Space:

    # Статические переменные для расчётов
    sumR: float = 0 # сумма радиусов двух расчётных частиц
    dx: float = 0   # разность по x между двумя расчётными частицами
    dy: float = 0   # разность по y между двумя расчётными частицами
    sqrL: float = 0 # квадрат расстояния между двумя расчётными частицами
    L: float = 0    # расстояние между двумя расчётными частицами

    wall = Wall

    def __init__(self, dt=0.001):
        self.dt = dt    # время просчёта одного кадра
        self.framesCount = 0 # количество просчитанных кадров
        self.particlesCount = 0 # количество частиц

        # Ускорение свободного падения (гравитация) по осям
        self.gx = 0
        self.gy = 9.8

        self.decay = 0.99   # Коэффициент затухания (используется при соударениях)

        self.particles = list()     # Список всех частиц, присутствующих в пространстве
        self.springs = list()       # Список всех пружин, присутствующих в пространстве

    # Добавление одной частицы
    def addParticle(self, x, y, r, m):
        self.particles.append(Particle(x, y, r, m))

    # Добавление квадратного массива из одинаковых частиц
    def addParticleArray(self, x, y, r, m, count, distance):
        squareSide = math.floor(math.sqrt(count))

        for i in range(squareSide):
            for j in range(squareSide):
                self.addParticle(x + i*distance, y + j*distance, r, m)

    def addSpring(self, p1, p2, k):
        self.springs.append(Spring(p1, p2, k))

    def addSpringsToParticlesGroup(self, x1, y1, x2, y2, r, k):
        # создание отдельного списка для частиц, входящих в выделенный прямоугольник
        connectedParticles = list()

        # Проходим циклом по всем частицам
        for i in range(len(self.particles)):
            # Если частица попадает в заданный прямоугольник
            if (self.particles[i].x > x1) & (self.particles[i].x < x2):
                if (self.particles[i].y > y1) & (self.particles[i].y < y2):
                    # Записываем частицу в специальный список
                    connectedParticles.append(self.particles[i])

        # Проходим циклом по всем выбранным частицам
        for i in range(len(connectedParticles)):
            p1 = connectedParticles[i]
            for j in range(i+1, len(connectedParticles)):
                p2 = connectedParticles[j]
                if not p2.connectedWithNeighbours:
                    # Определяем расстояние между частицами
                    dx = p2.x - p1.x
                    dy = p2.y - p1.y
                    sqrL = dx*dx + dy*dy

                    if (sqrL < r*r):    # Если расстояние между частицами меньше заданного r, то...
                        #... устанавливаем пружинную связь между частицами
                        self.springs.append(Spring(p1, p2, k))  # Сахраняем пружину в список пружин
                        p1.springLinksCounter += 1

            # Если были установлены какие либо связи с другими частицами, то устанавливаем признак присутствия связей у частицы
            if (p1.springLinksCounter):
                p1.connectedWithNeighbours = True

    # Расчёт силы гравитации на частицу
    def computeGravityForce(self, p1):
        p1.Fx += p1.m*self.gx
        p1.Fy += p1.m*self.gy

    def computeAllForces(self):
        # Цикл прохода по всем частицам в пространстве
        for i in range(len(self.particles)):
            p = self.particles[i]
            # Обнуляем все силы, действующие на частицу для дальнейшего перерасчёта
            p.Fx = 0
            p.Fy = 0
            self.computeGravityForce(p)     # расчёт силы гравитации, действующей на частицу

        # Цикл прохода по всем пружинам в списке
        for i in range(len(self.springs)):
            spring = self.springs[i]
            # вычисляем силу упругости по закону Гука
            spring.computeHookForce()



    # Функция расчёта столкновения двух частиц
    def computeCollisionBetwinParticles(self, p1, p2):

        # Проверка знаменателя дроби на 0 (чтобы избежать ошибки деления на 0)
        if (self.dx <= almostZero):
            self.dx = almostZero

        # Расчёт тригонометрических функций для вычисления проекций скрости
        tana = self.dy/self.dx
        cosa: float = 1/math.sqrt(tana*tana + 1)
        sina = tana*cosa

        # Расчёт проекций составляющих скорости на ось взаимодействия
        # (используется для дальнейших расчётов столкновения)
        v1px: float = p1.vx*cosa + p1.vy*sina
        v2px: float = p2.vx*cosa + p2.vy*sina

        # Расчёт проекций составляющих скорости на ось перпендикулярную взаимодействию
        # (не принимает участия в столкновении, скорости не меняются)
        v1py = p1.vy*cosa - p1.vx*sina
        v2py = p2.vy*cosa - p2.vx*sina

        # Расчёт столкновения
        # Вычислвение промежуточных значений
        p = p1.m*v1px + p2.m*v2px   # Расчёт суммарного импульса частиц до столкновения
        dv = v1px - v2px    # Расчёт разности скорости частиц до столкновения

        v2pxnew = (p + p1.m*dv)/(p1.m + p2.m)
        v1pxnew = v2pxnew - v1px + v2px

        # Расчёт проекций новых составляющих скорости на реальные оси x, y
        p1.vx = v1pxnew*cosa - v1py*sina
        p2.vx = v2pxnew*cosa - v2py*sina

        p1.vy = v1pxnew*sina + v1py*cosa
        p2.vy = v2pxnew*sina + v2py*cosa

        # учёт затухания при соударениях
        # p1.vx *= self.decay
        # p1.vy *= self.decay
        #
        # p2.vx *= self.decay
        # p2.vy *= self.decay


    # Функция "разлепивания" частиц если они наползают друг на друга
    # (упрощение вместо "правильного" расчёта столкновения между кадрами)
    def unstickPartcles(self, p1, p2):
        d = p1.r + p2.r - self.L
        k = 0.5*d/self.L
        dx = k*self.dx   # насколько вернуть частицы по x
        dy = k*self.dy   # насколько вернуть частицы по y

        # Изменяем координаты частицы 1
        p1.x = p1.x + dx
        p1.y = p1.y + dy

        # Изменяем координаты частицы 2
        p2.x = p2.x - dx
        p2.y = p2.y - dy


    def detectCollisionBetwinParticles(self, p1, p2):
        self.sumR = p1.r + p2.r  # сумма радиусов двух частиц
        self.dx = p1.x - p2.x  # разность по x
        self.dy = p1.y - p2.y  # разность по y

        if (abs(self.dx) <= self.sumR):
            if (abs(self.dy) <= self.sumR):
                self.sqrL = self.dx*self.dx + self.dy*self.dy  # расчёт квадрата расстояния между частицами
                self.L = math.sqrt(self.sqrL)
                if (self.sqrL <= self.sumR*self.sumR):
                    self.unstickPartcles(p1, p2)
                    self.computeCollisionBetwinParticles(p1, p2)


    def detectCollisionWithWalls(self, p1):
        # Если частица "сталкивается" с левой стеной
        if ((p1.x - p1.r) <= self.wall.left):
            # Расчёт соударения с левой/правой стеной
            p1.vx = - p1.vx # Меняем векторор скорости по x на противоположный
            p1.vx *= self.decay
            p1.x = self.wall.left + p1.r    # "вытаскиваем" из стены частицу если она успела в неё "проникнуть"

        # Если частица "сталкивается" с правой стеной
        if ((p1.x + p1.r) >= self.wall.right):
            # Расчёт соударения с правой стеной
            p1.vx = - p1.vx # Меняем векторор скорости по x на противоположный
            p1.vx *= self.decay
            p1.x = self.wall.right - p1.r    # "вытаскиваем" из стены частицу если она успела в неё "проникнуть"

        # Если частица "сталкивается" с верхней стеной
        if ((p1.y - p1.r) <= self.wall.top):
            p1.vy = - p1.vy # Меняем векторор скорости по y на противоположный
            p1.vy *= self.decay
            p1.y = self.wall.top + p1.r  # "вытаскиваем" из стены частицу если она успела в неё "проникнуть"

        # Если частица "сталкивается" с нижней стеной
        if ((p1.y + p1.r) >= self.wall.bottom):
            # Расчёт соударения с верхней и нижней стеной
            p1.vy = - p1.vy # Меняем векторор скорости по y на противоположный
            p1.vy *= self.decay
            p1.y = self.wall.bottom - p1.r  # "вытаскиваем" из стены частицу если она успела в неё "проникнуть"


    # def detectCollisionWithForms(self):
    #     pass

    # def detectAllCollisions(self):
    #     # Цикл прохода по всем частицам в списке
    #     for i in range(len(self.particles)):
    #         p1 = self.particles[i]
    #         self.detectCollisionWithWalls(p1)
    #         # self.calculateAllForces()
    #
    #         # Цикл прохода по всем частицам в списке, кроме тех по которым уже прошли
    #         for k in range(i+1, len(self.particles)):
    #             p2 = self.particles[k]
    #
    #             self.detectCollisionBetwinParticles(p1, p2)


    def computeTimeFrame(self):

        self.computeAllForces()     # расчёт всех, действующих на частицу сил

        # Цикл прохода по всем частицам в списке
        for i in range(len(self.particles)):
            # p1 = Particle()
            p1 = self.particles[i]

            # Расчёт сил гравитации на частицу (действует на каждую частицу)
            # self.computeGravityForce(p1)

            # Поиск соударений частиц со стенками пространства
            self.detectCollisionWithWalls(p1)

            # Цикл прохода по всем частицам в списке, кроме тех по которым уже прошли
            for k in range(i+1, len(self.particles)):
                p2 = self.particles[k]

                self.detectCollisionBetwinParticles(p1, p2)

            # Расчёт нового положения частицы исходя из всех учтённых сил и взаимодействий
            p1.computeAcceleration()
            p1.computeVelocity()
            p1.computeMovement()



if __name__ == '__main__':
    space = Space();
    space.addParticle(0, 0, 1, 1)
    space.addParticle(10, 10, 1, 1)
    space.detectCollision()







