#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установка окна для прорисовки графического вида физической модели
    ui->graphicsView->setScene(view->scene);
    view->test();

    // Привязка обработчиков нажатий к кнопкам
    connect(ui->buttonRunSimulation, &QPushButton::clicked, view, &View::togleSimulation);
    connect(ui->buttonStepSimulation, &QPushButton::clicked, view, &View::oneStepSimulation);
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

