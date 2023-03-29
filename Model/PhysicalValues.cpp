#include "PhysicalValues.hpp"

#include <QColor>

// Определение физических констант для моделирования
typedef double physvalue;

//namespace phys {

//#define dt_ 0.01
//#define dt ((physvalue)0.01)
physvalue dt_ = 0.01;

//#define almostZero 0.0001
//#define  almostZero ((physvalue)0.0001)
physvalue almostZero = 0.0001;

QColor defaultColor_(Qt::red);

physvalue framesPerSecond = 100;  // Количество кадров моделирования в секунду

//}  // namespace phys
