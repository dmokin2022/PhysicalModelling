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
  void setLabelColor(QColor color);

private slots:
  void mouseClickedOver(qreal x, qreal y);
  void propertiesChanged();
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

signals:
  void runSimulation();
  void stepSimulation();
};
#endif  // MAINWINDOW_H
