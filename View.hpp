
#include "A_Particle.hpp"
#include "B_Spring.hpp"
#include "C_Box.hpp"
#include "D_Space.hpp"

#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QTimer>

class View: public QObject {
    Q_OBJECT
public:
    physvalue scale; // Масштабирующий коэффициент. Сколько точек экрана приходится на единичный отрезок в пространстве модели

    physvalue offsetX = 0;    // Точка пространства модели, отображаемая в левой верхней части экрана (смщещение по x и y)
    physvalue offsetY = 0;
    physvalue framesPerSecond = 100;   // Количество кадров моделирования в секунду

    bool simulationIsStarted = false;    // Флаг признака того, что процесс симуляции запущен

    Space space;

    // Создаём объект для графической сцены
    QGraphicsScene *scene;
    QGraphicsItemGroup *boxGroup;
    QGraphicsItemGroup *particlesGroup;

    // Настройка таймера для генерации кадров
    QTimer timer;

    View();

    void initDraw();

    void drawModel();

    void test();

public slots:
    void showFrame();   // обработчик таймера
    // Обработчика нажатия на кнопку Пуск/Стоп
    void oneStepSimulation();
    void togleSimulation();

};

//if __name__ == '__main__':
//    view = View()
//    view.test()
