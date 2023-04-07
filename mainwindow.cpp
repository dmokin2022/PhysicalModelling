#include "mainwindow.h"

#include <QLineEdit>

#include "graphicsscene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  view = new View();

  // Установка окна для прорисовки графического вида физической модели
  ui->graphicsView->setScene(view->scene);

  view->testParticlesOnly();
  //view->testWithSprings();

  setConnections();

  //    connect(&ui->buttonRunSimulation, &QPushButton::clicked, view, &[this](){
  //        view->space.computeTimeFrame();
  //        view->drawModel();
  //    });
}

MainWindow::~MainWindow() { delete ui; }

// Обработчика нажатия на кнопку Пуск/Стоп
void MainWindow::runPushButtonHandler() {
  view->space.computeTimeFrame();
  view->drawModel();
}

void MainWindow::constPushButtonHandler() {
  view->simulationIsStarted = !view->simulationIsStarted;
}

void MainWindow::setConnections() {
  // Привязка обработчиков нажатий к кнопкам
//  connect(ui->pushButtonPlay, &QPushButton::clicked, view, &View::togleSimulation);
//  connect(ui->pushButtonStep, &QPushButton::clicked, view, &View::oneStepSimulation);
//  connect(ui->pushButtonRestart, &QPushButton::clicked, view, &View::restart);
  // Подключаем получение сигналов от нажатий мыши в сцене
  connect(view->scene, &GraphicsScene::clicked_, this, &MainWindow::mouseClickedOver);

  connect(ui->lineEditRadius, &QLineEdit::textChanged, this, &MainWindow::propertiesChanged);
  connect(ui->lineEditMass, &QLineEdit::textChanged, this, &MainWindow::propertiesChanged);
  connect(ui->lineEditCharge, &QLineEdit::textChanged, this, &MainWindow::propertiesChanged);

  connect(ui->pushButtonColor, &QPushButton::clicked, this, &MainWindow::onChooseColor);
  connect(ui->checkBoxFill, &QCheckBox::clicked, this, &MainWindow::onFillColorChecked);
  //connect(ui->dial, &QDial::valueChanged, this, &MainWindow::onAngleChanged);
}

void MainWindow::onChooseColor() {
  if (view->selectedParticle) {
    QColor color = colorDialog.getColor();

    view->selectedParticle->color = color;

    setLabelColor(color);
//    QPalette palette = ui->labelColor->palette();
//    palette.setColor(ui->labelColor->backgroundRole(), color);
//    palette.setColor(ui->labelColor->foregroundRole(), color);
//    ui->labelColor->setPalette(palette);
  }

  view->drawModel();
}

void MainWindow::setLabelColor(QColor color) {
    //ui->labelColor->setStyleSheet("background-color: RoyalBlue ");
    //ui->labelColor->setStyle();

    QPalette palette = ui->labelColor->palette();
    palette.setColor(ui->labelColor->backgroundRole(), color);
    palette.setColor(ui->labelColor->foregroundRole(), color);
    ui->labelColor->setPalette(palette);
}

void MainWindow::mouseClickedOver(qreal x, qreal y) {
  auto px = view->physXfromScene(x);
  auto py = view->physXfromScene(y);

  QString sx = QString::number(px);
  QString sy = QString::number(py);
  ui->labelXY->setText(sx + ", " + sy);

  Particle *particle = view->getParticleAtAlloc(px, py);
  if (particle != nullptr) {

    ui->lineEditRadius->setText(QString::number(particle->r));
    ui->lineEditMass->setText(QString::number(particle->m));
    ui->lineEditCharge->setText(QString::number(particle->q));
    if (particle->isFilledWithColor) {
      ui->checkBoxFill->setCheckState(Qt::CheckState::Checked);
    } else {
      ui->checkBoxFill->setCheckState(Qt::CheckState::Unchecked);
    }

    setLabelColor(particle->color);

    // Отображение угла направления скорости
    physvalue angle = particle->getVAngle();
    physvalue v     = particle->getV();
    ui->dial->setValue(360 - angle);
    ui->lineEditAngle->setText(QString::number(angle));
    ui->lineEditVelocity->setText(QString::number(v));

    view->drawModel();

  } else {
    ui->lineEditRadius->setText("");
    ui->lineEditMass->setText("");
    ui->lineEditCharge->setText("");
    ui->labelColor->setStyleSheet("");
    ui->checkBoxFill->setCheckState(Qt::CheckState::Unchecked);
    ui->dial->setValue(0);
  }
}

void MainWindow::propertiesChanged() {
  // Проверка правильности ввода данных в поля
  bool isValid    = false;
  physvalue value = 0;

  value = ui->lineEditRadius->text().toFloat(&isValid);
  if (isValid) { view->selectedParticle->r = value; }

  value = ui->lineEditMass->text().toFloat(&isValid);
  if (isValid) { view->selectedParticle->m = value; }

  value = ui->lineEditCharge->text().toFloat(&isValid);
  if (isValid) { view->selectedParticle->q = value; }

  physvalue angle = view->selectedParticle->getVAngle();
  ui->dial->setValue(angle);

  view->space.pushApartParticles();
  view->drawModel();
}

void MainWindow::onFillColorChecked() {
  if (view->selectedParticle) {
    if (ui->checkBoxFill->isChecked()) {
      view->selectedParticle->isFilledWithColor = true;
    } else {
      view->selectedParticle->isFilledWithColor = false;
    }
  }
  view->drawModel();
}

void MainWindow::onAngleChanged(int value) {
  // Отображение угла направления скорости
  //  physvalue angle = view->selectedParticle->getVAngle();
  //  physvalue v     = view->selectedParticle->getV();
  ui->dial->setValue(360 - value);
  ui->lineEditAngle->setText(QString::number(value));
  //ui->labelVelocity->setText(QString::number(v));
}

void MainWindow::on_toolButton_2_clicked()
{

}

void MainWindow::on_toolButton_4_clicked()
{

}

void MainWindow::on_toolButton_6_clicked()
{

}

void MainWindow::on_toolButton_5_clicked()
{

}

void MainWindow::on_toolButton_3_clicked()
{

}

void MainWindow::on_toolButton_clicked()
{

}

void MainWindow::on_toolButton_7_clicked()
{

}

void MainWindow::on_toolButtonPlay_clicked()
{
    //view->simulationIsStarted = !view->simulationIsStarted;
    view->togleSimulation();
}

void MainWindow::on_toolButtonOneStep_clicked()
{
    view->oneStepSimulation();
}

void MainWindow::on_toolButtonRestart_clicked()
{
    view->restart();
}
