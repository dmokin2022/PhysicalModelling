//import sys

//from PyQt5.QtWidgets import QApplication


//from PyQt5 import uic

//import View
#include "View.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QWindow>


View *view;

int main2(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow w;
    w.show();
    return a.exec();

    // !!! ОБЯЗАТЕЛЬНО ИДЁТ ПЕРВОЙ СТРОКОЙ В ЛЮБОМ ПРИЛОЖЕНИИ, ИСПОЛЬЗУЮЩЕЙ QtGUI !!!
    QApplication *app = new QApplication(argc, argv);    // Создаёт приложение Qt.
    // или:
    // app = QtWidgets.QApplication([])

    // Загружаем файл интерфейса

    //QWindow win = uic.loadUi("PhysicalModelling.ui");  // расположение вашего файла .ui

    view = new View();
    //view = View.View();

    w.graphicsView.setScene(view.scene);
    view.test();

    // Привязка обработчика к кнопке поиска по названию ЗАСТРОЙЩИКА в таблице застройщиков
    win.runPushButton.clicked.connect(runPushButtonHandler);
    win.constPushButton.clicked.connect(constPushButtonHandler);


    // // !!! ОБЯЗАТЕЛЬНО ИДЁТ В КОНЦЕ В ЛЮБОМ ПРИЛОЖЕНИИ, ИСПОЛЬЗУЮЩЕЙ QtGUI !!!
    win.show();
    app.exit();
    sys.exit(app.exec());
}

// Обработчика нажатия на кнопку Пуск/Стоп
void runPushButtonHandler() {
    view->space.computeTimeFrame();
    view->drawModel();
}

void constPushButtonHandler() {
    view->simulationIsStarted = !view->simulationIsStarted;
}
