import shutil
import sys
from PyQt5.QtWidgets import QListWidget, QAbstractItemView, QWidget, QVBoxLayout, QApplication, QPushButton
import serial
import time
import os
from PIL import Image
import sys

messageSize = 64
stripSize = 300
imageSize = 10
# 300 Leds/5M
# Image 300*533: 133s, 0.24s/line
# Distance between 2 leds: 1,66cm
# Maximum speed: ~6.9cm/s, ~0.25km/h

# 144 Leds/m
# Image 144*256: 32s, 0.11s/line
# Distance between 2 leds: 0.7mm
# Maximum speed: ~6.36cm/s, ~0.22km/h


url_list = list()


class MyListWidget(QListWidget):
    def __init__(self, parent):
        super(MyListWidget, self).__init__(parent)
        self.setAcceptDrops(True)
        self.setDragDropMode(QAbstractItemView.InternalMove)

    def dragEnterEvent(self, event):
        if event.mimeData().hasUrls():
            event.acceptProposedAction()
        else:
            super(MyListWidget, self).dragEnterEvent(event)

    def dragMoveEvent(self, event):
        super(MyListWidget, self).dragMoveEvent(event)

    def dropEvent(self, event):
        if event.mimeData().hasUrls():
            for url in event.mimeData().urls():
                self.addItem(url.path())
                url_list.append(url.toLocalFile())
            event.acceptProposedAction()
        else:
            super(MyListWidget, self).dropEvent(event)


class MyWindow(QWidget):
    def __init__(self):
        super(MyWindow, self).__init__()
        self.setGeometry(100, 100, 500, 500)
        self.setWindowTitle("Python image converter")

        self.list = MyListWidget(self)
        self.btn = QPushButton("Convert")

        layout = QVBoxLayout(self)
        layout.addWidget(self.list)
        layout.addWidget(self.btn)
        self.btn.clicked.connect(execConverter)

        self.setLayout(layout)


def execConverter():
    if os.path.exists("ARDUINO"):
        shutil.rmtree("ARDUINO", ignore_errors=False, onerror=None)
        print("ARDUINO folder deleted")

    myImageSequence=0
    for url in url_list:
        myImageSequence += 1
        myImagePath = url

        print(myImagePath)
        ################# Loading image

        myImage = Image.open(myImagePath)
        myPixels = list(myImage.getdata())
        imageX = myImage.size[0]
        imageY = myImage.size[1]
        pix = myImage.load()
        imageSize = imageX

        ################# Preparing image formatted path
        formattedPath = "ARDUINO/0"

        if (myImageSequence < 10):
            formattedPath = formattedPath + "0" + str(myImageSequence) + "/"
        else:
            formattedPath = formattedPath + str(myImageSequence) + "/"

        ################# For each column of pixel, creating a file
        columnCount = 0;
        deci = 0; # used to add a 0 before /0.dat when the float is equal to a tenth
        while columnCount < imageSize:

            fileName = formattedPath
            if columnCount / 100 < 10:
                fileName = fileName + '0'
            if deci == 10:
                fileName = fileName + str(columnCount / 100) + "0/"
                deci=0;
            else :
                fileName = fileName + str(columnCount / 100) + "/"

            if not os.path.exists(fileName):
                os.makedirs(fileName)
            deci += 1;

            fileName = fileName + "0.dat"
            # print ("Creating file " + str(fileName))

            with open(fileName, 'w', encoding="utf-8") as myFile:

                ################# Sending a pixel column
                ################# Pixel columns are sent as series of pixels in a message. Each message has messageSize pixel in order to not overload the arduino

                # print "writing a pixel column in file " + str(fileName)
                y = 0
                while y < imageY:
                    myFile.write(str(chr(pix[columnCount, y][0])))
                    myFile.write(str(chr(pix[columnCount, y][1])))
                    myFile.write(str(chr(pix[columnCount, y][2])))

                    y = y + 1
                ################# Y is the height of each pixel,

                columnCount = columnCount + 1

        print("Estimated image size:" + str(imageSize / float(144)) + "m")
        print("Estimated exposition time:" + str(imageSize / float(144) * 9) + "s")


if __name__ == '__main__':
    app = QApplication(sys.argv)

    window = MyWindow()
    window.show()
    sys.exit(app.exec_())
