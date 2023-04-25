#pragma once

#include <QSettings>
#include <QString>
#include <QVariant>

#define SAVE_VAR(x) sap.saveVar(#x, x)
#define LOAD_VAR(x) sap.loadVar(#x, x)

class SimpleAppSettings {
public:
  SimpleAppSettings();

  void setOrgApp(const QString &org, const QString &app);

  void startSection(const QString &sectionName);

  void saveVar(const QString &name, const bool &var);
  void saveVar(const QString &name, const int &var);
  void saveVar(const QString &name, const double &var);
  void saveVar(const QString &name, const QString &var);

  void loadVar(const QString &name, bool &var);
  void loadVar(const QString &name, int &var);
  void loadVar(const QString &name, double &var);
  void loadVar(const QString &name, QString &var);

  void saveAll();
  void loadAll();

private:
  QMap<QString, QVariant> storage;
  QSettings *settings;
  QString currentSection;
};

extern SimpleAppSettings sap;
extern void SimpleAppSettings_test();
