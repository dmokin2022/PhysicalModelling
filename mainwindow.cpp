#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new View();

    // Установка окна для прорисовки графического вида физической модели
    ui->graphicsView->setScene(view->scene);
    view->test();

    // Привязка обработчиков нажатий к кнопкам
    connect(ui->pushButton, &QPushButton::clicked, view, &View::togleSimulation);
    connect(ui->pushButton_2, &QPushButton::clicked, view, &View::oneStepSimulation);
    //ui->push
//    connect(&ui->buttonRunSimulation, &QPushButton::clicked, view, &[this](){
//        view->space.computeTimeFrame();
//        view->drawModel();
//    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


// Обработчика нажатия на кнопку Пуск/Стоп
void MainWindow::runPushButtonHandler() {
    view->space.computeTimeFrame();
    view->drawModel();
}

void MainWindow::constPushButtonHandler() {
    view->simulationIsStarted = !view->simulationIsStarted;
}

