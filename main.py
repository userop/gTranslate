#!/usr/bin/python3
# coding: UTF-8
# language_level: python3
import sys
from PySide2.QtWidgets import QApplication

from translate import GoogleTrans


if __name__ == '__main__':
    app = QApplication([])
    window = GoogleTrans()
    window.show()
    sys.exit(app.exec_())
