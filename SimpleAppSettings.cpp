#include "SimpleAppSettings.hpp"

#include <QApplication>

#include "qdebug.h"

SimpleAppSettings::SimpleAppSettings() {
  //QSettings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr)
  settings = new QSettings("SimpleAppSettings.ini", QSettings::IniFormat);
}

void SimpleAppSettings::setOrgApp(const QString &org, const QString &app) {
  // эти настройки используются (неявно) классом QSettings для
  // определения имени и местоположения конфига
  QCoreApplication::setOrganizationName(org);
  QCoreApplication::setApplicationName(app);
}

void SimpleAppSettings::startSection(const QString &sectionName) {}

// Запись в конфиг
//-------------------------
void SimpleAppSettings::saveVar(const QString &name, const bool &var) {
  settings->setValue(name, var);
}

void SimpleAppSettings::saveVar(const QString &name, const int &var) {
  settings->setValue(name, var);
}

void SimpleAppSettings::saveVar(const QString &name, const double &var) {
  settings->setValue(name, var);
}

void SimpleAppSettings::saveVar(const QString &name, const QString &var) {
  settings->setValue(name, var);
}

void SimpleAppSettings::saveAll() {
  QList<QString> keys = storage.keys();

  for (auto key : keys) {
    //
    settings->setValue(key, storage[key]);
  }
}

// Чтение из конфига
//------------------------------
void SimpleAppSettings::loadVar(const QString &name, bool &var) {
  var = settings->value(name).toBool();
}

void SimpleAppSettings::loadVar(const QString &name, int &var) {
  var = settings->value(name).toInt();
}

void SimpleAppSettings::loadVar(const QString &name, double &var) {
  var = settings->value(name).toDouble();
}

void SimpleAppSettings::loadVar(const QString &name, QString &var) {
  var = settings->value(name).toString();
}

void SimpleAppSettings::loadAll() {
  QList<QString> keys = storage.keys();

  for (auto key : keys) {
    //
    settings->value(key, storage[key]);
  }
}

SimpleAppSettings sap;

#define STR(x) #x

void SimpleAppSettings_test() {
  sap.setOrgApp("MyCompany", "SSaver");

  bool Bool      = true;
  int Int1       = 100;
  int Int2       = 200;
  double Double  = 333.444;
  QString String = "My name is...";

  bool _Bool      = Bool;
  int _Int1       = Int1;
  int _Int2       = Int2;
  double _Double  = Double;
  QString _String = String;

  SAVE_VAR(Bool);
  SAVE_VAR(Int1);
  SAVE_VAR(Int2);
  SAVE_VAR(Double);
  SAVE_VAR(String);

  Bool   = false;
  Int1   = 0;
  Int2   = 0;
  Double = 0.0;
  String = "";

  LOAD_VAR(Bool);
  LOAD_VAR(Int1);
  LOAD_VAR(Int2);
  LOAD_VAR(Double);
  LOAD_VAR(String);

  qDebug() << "Переменная"
           << "Была"
           << "Стала";
  qDebug() << "---------------------------------------------------------------------------------";
  qDebug() << STR(Bool) << _Bool << Bool;
  qDebug() << STR(Int1) << _Int1 << Int1;
  qDebug() << STR(Int2) << _Int2 << Int2;
  qDebug() << STR(Double) << _Double << Double;
  qDebug() << STR(String) << _String << String;
}
