#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QMainWindow>

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
  QColorDialog colorDialog;

  // Обработчика нажатия на кнопку Пуск/Стоп
  void runPushButtonHandler();
  void constPushButtonHandler();
  void setConnections();

  void onChooseColor();

private slots:
  void mouseClickedOver(qreal x, qreal y);
  void propertiesChanged();
  void onFillColorChecked();

signals:
  void runSimulation();
  void stepSimulation();
};
#endif  // MAINWINDOW_H
