# coding: UTF-8
# cython: language_level=3
"""
  date: 6/13/21
  
"""
import json

from PySide2.QtWidgets import QWidget, QLabel, QVBoxLayout, QHBoxLayout, QApplication, \
    QShortcut, QComboBox, QMenu, QSystemTrayIcon
from PySide2.QtGui import QClipboard, QCursor, QCloseEvent, QKeyEvent
from PySide2.QtCore import Slot, Qt

from google_trans_new import LANGUAGES
from google_trans_new.google_trans_new import google_translator, URLS_SUFFIX


transer = google_translator()
CONFIG = "info"


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

        # build shortcut
        self._action = QShortcut(self)
        self._action.setKey(self.info.action_)
        self._quit = QShortcut(self)
        self._quit.setKey(self.info.quit_)
        self._exchange = QShortcut(self)
        self._exchange.setKey(self.info.exchange)

        self.t_box = QComboBox()
        self.f_box = QComboBox()
        self.domain = QComboBox()
        # 加载语言简拼
        lang = list(LANGUAGES.keys())
        self.domain.addItems(URLS_SUFFIX)
        self.t_box.addItems(lang)
        self.f_box.addItems(lang)

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
        self._exchange.activated.connect(self.ex)

        # 小图标、隐藏图标选项
        self.customContextMenuRequested.connect(self.context)
        self.tray.activated.connect(self.tray_active)
        self.menu.aboutToHide.connect(self.setdown)

    @Slot()
    def setdown(self):
        self.info.to_ = self.t_box.currentText()
        self.info.from_ = self.f_box.currentText()
        transer.url_suffix = self.domain.currentText()

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

        self.domain.setCurrentText(transer.url_suffix)
        self.t_box.setCurrentText(self.info.to_)
        self.f_box.setCurrentText(self.info.from_)

        self.menu.exec_(self.mapToGlobal(pos))

    @Slot()
    def _translate(self):
        if not self._trans_on:
            return
        text = self.clip.text(QClipboard.Selection)
        if text:
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
    def ex(self):
        tgt = self.info.to_
        self.info.to_ = self.info.from_
        self.info.from_ = tgt

    @Slot()
    def quit(self):
        self._off = 1
        self.close()

    @Slot()
    def _on_off(self):
        self._trans_on = not self._trans_on

    def keyPressEvent(self, event: QKeyEvent) -> None:
        if event.key() == Qt.Key_Escape:
            self.hide()

    def closeEvent(self, event: QCloseEvent) -> None:
        if self._off:
            self.info.close()
            event.accept()
            return
        self.setVisible(False)
        self.hide()
        event.ignore()
