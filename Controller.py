import sys

from PyQt5.QtWidgets import QApplication


from PyQt5 import uic

import View


# Обработчика нажатия на кнопку Пуск/Стоп
def runPushButtonHandler():
    view.space.computeTimeFrame()
    view.drawModel()

def constPushButtonHandler():
    view.simulationIsStarted = ~view.simulationIsStarted



if __name__ == '__main__':

    # !!! ОБЯЗАТЕЛЬНО ИДЁТ ПЕРВОЙ СТРОКОЙ В ЛЮБОМ ПРИЛОЖЕНИИ, ИСПОЛЬЗУЮЩЕЙ QtGUI !!!
    app = QApplication(sys.argv)    # Создаёт приложение Qt.
    # или:
    # app = QtWidgets.QApplication([])

    # Загружаем файл интерфейса
    win = uic.loadUi("PhysicalModelling.ui")  # расположение вашего файла .ui


    view = View.View()
    win.graphicsView.setScene(view.scene)
    view.test()

    # Привязка обработчика к кнопке поиска по названию ЗАСТРОЙЩИКА в таблице застройщиков
    win.runPushButton.clicked.connect(runPushButtonHandler)
    win.constPushButton.clicked.connect(constPushButtonHandler)


    # # !!! ОБЯЗАТЕЛЬНО ИДЁТ В КОНЦЕ В ЛЮБОМ ПРИЛОЖЕНИИ, ИСПОЛЬЗУЮЩЕЙ QtGUI !!!
    win.show()
    app.exit()
    sys.exit(app.exec())
