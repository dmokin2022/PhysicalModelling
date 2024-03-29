#include "mainwindow.h"

#include <QLineEdit>

#include "graphicsscene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  view       = new View();
  controller = new Controller(view->space, *view);

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
  connect(view->scene, &GraphicsScene::clicked_, this, &MainWindow::onMouseClickedOverView);
  connect(view->scene, &GraphicsScene::moved_, this, &MainWindow::onMouseMovedOverView);

  connect(ui->lineEditRadius, &QLineEdit::textChanged, this, &MainWindow::onPropertiesChanged);
  connect(ui->lineEditMass, &QLineEdit::textChanged, this, &MainWindow::onPropertiesChanged);
  connect(ui->lineEditCharge, &QLineEdit::textChanged, this, &MainWindow::onPropertiesChanged);

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

void MainWindow::showParticleProperties(Particle *p) {
  // Если курсор попал на частицу
  if (p != nullptr) {
    QString sx = QString::number(p->x);
    QString sy = QString::number(p->y);
    ui->labelXY->setText(sx + ", " + sy);

    ui->lineEditRadius->setText(QString::number(p->r));
    ui->lineEditMass->setText(QString::number(p->m));
    ui->lineEditCharge->setText(QString::number(p->q));
    if (p->isFilledWithColor) {
      ui->checkBoxFill->setCheckState(Qt::CheckState::Checked);
    } else {
      ui->checkBoxFill->setCheckState(Qt::CheckState::Unchecked);
    }

    setLabelColor(p->color);

    // Отображение угла направления скорости
    physvalue angle = p->getVAngle();
    physvalue v     = p->getV();
    ui->dial->setValue(360 - angle);
    ui->lineEditAngle->setText(QString::number(angle));
    ui->lineEditVelocity->setText(QString::number(v));

    view->drawModel();

  } else {
    // Если курсор мыши не попал на частицу
    ui->lineEditRadius->setText("");
    ui->lineEditMass->setText("");
    ui->lineEditCharge->setText("");
    ui->labelColor->setStyleSheet("");
    ui->checkBoxFill->setCheckState(Qt::CheckState::Unchecked);
    ui->dial->setValue(0);
  }
}

void MainWindow::onMouseClickedOverView(qreal x, qreal y) {
  auto px = view->physXfromScene(x);
  auto py = view->physXfromScene(y);

  //  QString sx = QString::number(px);
  //  QString sy = QString::number(py);
  //  ui->labelXY->setText(sx + ", " + sy);

  Particle *particle = view->getParticleAtAlloc(px, py);

  // Запоминаем смещение курсора мыши относительно центра окружности (частицы)
  if (particle) {
    moveOffsetX = view->physXfromScene(x) - particle->x;
    moveOffsetY = view->physYfromScene(y) - particle->y;
  }

  controller->editOperationAtAlloc(x, y);

  showParticleProperties(particle);
}

void MainWindow::onMouseMovedOverView(qreal x, qreal y) {
  // Если курсор мыши пападает внутрь частицы
  if (view->selectedParticle) {
    auto px = view->physXfromScene(x);
    auto py = view->physYfromScene(y);

    view->selectedParticle->x = px - moveOffsetX;
    view->selectedParticle->y = py - moveOffsetY;
  }

  view->drawModel();
}

void MainWindow::onPropertiesChanged() {
  if (view->selectedParticle) {
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

void MainWindow::on_toolButton_2_clicked() {}

void MainWindow::on_toolButton_4_clicked() {}

void MainWindow::on_toolButton_6_clicked() {}

void MainWindow::on_toolButton_5_clicked() {}

void MainWindow::on_toolButton_3_clicked() {}

void MainWindow::on_toolButton_clicked() {}

void MainWindow::on_toolButton_7_clicked() {}

void MainWindow::on_toolButtonPlay_clicked() {
  //view->simulationIsStarted = !view->simulationIsStarted;
  view->togleSimulation();
}

void MainWindow::on_toolButtonOneStep_clicked() { view->oneStepSimulation(); }

void MainWindow::on_toolButtonRestart_clicked() { view->restart(); }

void MainWindow::on_toolButtonAddParticle_clicked() { controller->toggleAddingParticleMode(); }

void MainWindow::on_toolButtonAddSpring_clicked() { controller->toggleAddingSpringMode(); }

void MainWindow::on_toolButtonDelete_clicked() { controller->toggleDeleteMode(); }
