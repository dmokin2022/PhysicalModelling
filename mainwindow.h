#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QMainWindow>

#include "Controller.h"
#include "View.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  View *view;
  Controller *controller;
  QColorDialog colorDialog;

  // Обработчика нажатия на кнопку Пуск/Стоп
  void runPushButtonHandler();
  void constPushButtonHandler();
  void setConnections();

  void onChooseColor();
  void setLabelColor(QColor color);
  void showParticleProperties(Particle *p);

  qreal moveOffsetX;
  qreal moveOffsetY;

private slots:
  void onMouseClickedOverView(qreal x, qreal y);
  void onMouseMovedOverView(qreal x, qreal y);
  void onPropertiesChanged();
  void onFillColorChecked();
  void onAngleChanged(int value);
  void on_toolButton_2_clicked();
  void on_toolButton_4_clicked();
  void on_toolButton_6_clicked();
  void on_toolButton_5_clicked();
  void on_toolButton_3_clicked();
  void on_toolButton_clicked();
  void on_toolButton_7_clicked();
  void on_toolButtonPlay_clicked();
  void on_toolButtonOneStep_clicked();
  void on_toolButtonRestart_clicked();
  void on_toolButtonAddParticle_clicked();
  void on_toolButtonAddSpring_clicked();

  void on_toolButtonDelete_clicked();

signals:
  void runSimulation();
  void stepSimulation();
};
#endif  // MAINWINDOW_H
