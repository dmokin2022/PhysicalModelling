#include "mainwindow.h"

#include <QLineEdit>

#include "graphicsscene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  view = new View();

  // Установка окна для прорисовки графического вида физической модели
  ui->graphicsView->setScene(view->scene);
  view->test();

  setConnections();

  //ui->push
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
  connect(ui->pushButtonPlay, &QPushButton::clicked, view, &View::togleSimulation);
  connect(ui->pushButtonStep, &QPushButton::clicked, view, &View::oneStepSimulation);
  connect(ui->pushButtonRestart, &QPushButton::clicked, view, &View::restart);
  // Подключаем получение сигналов от нажатий мыши в сцене
  connect(view->scene, &GraphicsScene::clicked_, this, &MainWindow::mouseClickedOver);

  connect(ui->lineEditRadius, &QLineEdit::textChanged, this, &MainWindow::propertiesChanged);
  connect(ui->lineEditMass, &QLineEdit::textChanged, this, &MainWindow::propertiesChanged);
  connect(ui->lineEditCharge, &QLineEdit::textChanged, this, &MainWindow::propertiesChanged);

  connect(ui->pushButtonColor, &QPushButton::clicked, this, &MainWindow::onChooseColor);
  connect(ui->checkBoxFill, &QCheckBox::clicked, this, &MainWindow::onFillColorChecked);
  //connect(ui->pushButtonColor, &QPushButton::clicked, this, &ui->colorDialog->open);
}

void MainWindow::onChooseColor() {
  if (view->selectedParticle) {
    QColor color = colorDialog.getColor();

    view->selectedParticle->color = color;

    QPalette palette = ui->labelColor->palette();
    palette.setColor(ui->labelColor->backgroundRole(), color);
    palette.setColor(ui->labelColor->foregroundRole(), color);
    ui->labelColor->setPalette(palette);
  }

  view->drawModel();
}

void MainWindow::mouseClickedOver(qreal x, qreal y) {
  auto px = view->physXfromScene(x);
  auto py = view->physXfromScene(y);

  QString sx = QString::number(px);
  QString sy = QString::number(py);
  ui->labelXY->setText(sx + ", " + sy);

  Particle *particle = view->getParticleAtAlloc(px, py);
  if (particle != nullptr) {
    //ui->labelXY->selectedText();
    ui->lineEditRadius->setText(QString::number(particle->r));
    ui->lineEditMass->setText(QString::number(particle->m));
    ui->lineEditCharge->setText(QString::number(particle->q));
    //ui->labelColor->setStyleSheet("background-color: RoyalBlue ");
    //ui->labelColor->setStyle();
    if (particle->isFilledWithColor) {
      ui->checkBoxFill->setCheckState(Qt::CheckState::Checked);
    } else {
      ui->checkBoxFill->setCheckState(Qt::CheckState::Unchecked);
    }
  } else {
    ui->lineEditRadius->setText("");
    ui->lineEditMass->setText("");
    ui->lineEditCharge->setText("");
    ui->labelColor->setStyleSheet("");
    ui->checkBoxFill->setCheckState(Qt::CheckState::Unchecked);
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
