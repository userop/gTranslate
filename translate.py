# coding: UTF-8
# cython: language_level=3
"""
  date: 6/13/21
  
"""
import json

from PySide2.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QApplication, \
    QShortcut, QComboBox, QMenu, QSystemTrayIcon
from PySide2.QtGui import QClipboard, QCursor, QCloseEvent
from PySide2.QtCore import Slot, Qt

from google_trans_new import LANGUAGES
from google_trans_new.google_trans_new import google_translator, URLS_SUFFIX


transer = google_translator()
CONFIG = "info.js"


class Info:
    def __init__(self, js_file):
        self.conf = js_file
        with open(js_file, 'r') as file:
            data = json.load(file)
        for key in data:
            setattr(self, key, data[key])

    def close(self):
        with open(self.conf, 'w') as file:
            delattr(self, "conf")
            json.dump(self.__dict__, file, indent=4)


class GoogleTrans(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.info = Info(CONFIG)

        self.setWindowFlags(Qt.FramelessWindowHint)
        self.label = QLabel(self)

        self._off = 0
        self._trans_on = True
        self._action = QShortcut(self)
        self._action.setKey(self.info.action_)
        self._quit = QShortcut(self)
        self._quit.setKey(self.info.quit_)

        self.t_box = QComboBox()
        self.f_box = QComboBox()
        self.domain = QComboBox()
        self.menu = QMenu()
        # layout
        lay = QVBoxLayout(self)
        self.setLayout(lay)
        lay.addWidget(self.label)

        # clipboard
        self.clip = QApplication.clipboard()
        # self.desktop = QApplication.desktop()
        self.setContextMenuPolicy(Qt.CustomContextMenu)

        # context menu and tools
        self.tray = QSystemTrayIcon()

        self.clip.selectionChanged.connect(self._translate)
        self._action.activated.connect(self._on_off)
        self._quit.activated.connect(self.quit)
        self.customContextMenuRequested.connect(self.context)
        self.tray.activated.connect(self.tray_active)

    @Slot()
    def context(self, pos):
        """菜单右键"""
        line = QHBoxLayout()
        line.addWidget(QLabel("Do translate<%s>: %s" % (self.info.action_, str(self._trans_on))))

        line1 = QHBoxLayout()
        line1.addWidget(QLabel("host: www.translate.google."))
        line1.addWidget(self.domain)

        line2 = QHBoxLayout()
        line2.addWidget(QLabel("To:"))
        line2.addWidget(self.t_box)

        line3 = QHBoxLayout()
        line3.addWidget(QLabel("From: "))
        line3.addWidget(self.f_box)

        lay = QVBoxLayout()
        lay.addLayout(line)
        lay.addLayout(line1)
        lay.addLayout(line2)
        lay.addLayout(line3)
        self.menu.setLayout(lay)

        lang = list(LANGUAGES.keys())
        self.domain.addItems(URLS_SUFFIX)

        self.t_box.addItems(lang)
        self.f_box.addItems(lang)

        self.menu.exec_(pos)

    @Slot()
    def _translate(self):
        if not self._trans_on:
            return
        text = self.clip.text(QClipboard.Selection)
        result = transer.translate(text, self.info.to_, self.info.from_)
        self.label.setText(result)
        self.adjustSize()

        self.move(QCursor.pos())
        self.setVisible(True)
        self.activateWindow()
        self.setWindowState((self.windowState() & ~Qt.WindowMinimized) | Qt.WindowActive)
        self.raise_()

    @Slot()
    def tray_active(self):
        # todo double click show that widget
        self._translate(QCursor.pos())

    @Slot()
    def quit(self):
        self._off = 1
        self.close()

    @Slot()
    def _on_off(self):
        self._trans_on = not self._trans_on

    def closeEvent(self, event: QCloseEvent) -> None:
        if self._off:
            self.info.close()
            event.accept()
            return
        self.setVisible(False)
        self.hide()
        event.ignore()

# todo window hide when clicked something
#   window flow the mouse At pos
#   try to change