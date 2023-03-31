#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphicsscene.h"

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
    connect(ui->pushButtonPlay, &QPushButton::clicked, view, &View::togleSimulation);
    connect(ui->pushButtonStep, &QPushButton::clicked, view, &View::oneStepSimulation);
    connect(ui->pushButtonRestart, &QPushButton::clicked, view, &View::restart);
    // Подключаем получение сигналов от нажатий мыши в сцене
    connect(view->scene, &GraphicsScene::clicked_, this, &MainWindow::mouseClickedOver);

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

void MainWindow::mouseClickedOver(qreal x, qreal y)
{
    auto px = view->physXfromScene(x);
    auto py = view->physXfromScene(y);

    QString sx = QString::number(px);
    QString sy = QString::number(py);
    ui->labelXY->setText(sx + ", " + sy);

    Particle* particle = view->getParticleAtAlloc(px, py);
    if (particle != nullptr) {
        //ui->labelXY->selectedText();
        ui->lineEditRadius->setText(QString::number(particle->r));
        ui->lineEditMass->setText(QString::number(particle->m));
        ui->lineEditCharge->setText(QString::number(particle->q));
        ui->labelColor->setStyleSheet("background-color: RoyalBlue ");
        //ui->labelColor->setStyle();
    } else {
        ui->lineEditRadius->setText("");
        ui->lineEditMass->setText("");
        ui->lineEditCharge->setText("");
        ui->labelColor->setStyleSheet("");
    }

}

