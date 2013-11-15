#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
from PyQt4.QtCore import Qt, QRegExp
from PyQt4.QtGui import (
   QDialog, QPlainTextEdit, QVBoxLayout, QSplitter, QPushButton, QTextCursor,
   QSyntaxHighlighter, QTextCharFormat, QColor, QFont)


class DefaultColorScheme:
    syntax_keyword="#808000" # darkYellow
    syntax_builtin="brown" 
    syntax_magic="magenta"
    syntax_self="darkMagenta"
    syntax_comment="darkGray"
    syntax_string="darkGreen"
    syntax_number="darkBlue"
scheme = DefaultColorScheme

example = """\
from embeddedpyqt import *
import random
#import PyQt4
print ""
print 'names in current namespace', dir()
print ""
print 'names in bridge', dir(bridge)

print ""
print "peers this plugin knows about"
print bridge.getPeers()
print ""
print "bridge.getOnlineList()"
print bridge.getOnlineList()
print ""
print "bridge.getFriendList()"
print bridge.getFriendList()
print ""
print bridge.getPeerDetails(bridge.getFriendList()[0])
#disabled... bridge.broadcastToRS("broadcasting from python in a plugin")
embpyqt_console.WindowTitle = random.random()

#disabled - connecting message push
#def gotMsg(msg):
#    print "1"
#asd = bridge.msgPush.connect(gotMsg)
#bridge.msgPush.signal.disconnect(gotMsg)
#PyQt4.QtCore.QObject.disconnect(bridge, PyQt4.QtCore.SIGNAL('msgPush()'), gotMsg)
"""

class PythonHighlighter(QSyntaxHighlighter):

    keywords = (
        "and",       "del",       "for",       "is",        "raise",
        "assert",    "elif",      "from",      "lambda",    "return",
        "break",     "else",      "global",    "not",       "try",
        "class",     "except",    "if",        "or",        "while",
        "continue",  "exec",      "import",    "pass",      "yield",
        "def",       "finally",   "in",        "print",     "with"
        )
    builtins = ('ArithmeticError', 'AssertionError', 'AttributeError', 'BaseException', 'BufferError', 'BytesWarning', 
    'DeprecationWarning', 'EOFError', 'Ellipsis', 'EnvironmentError', 'Exception', 'False', 'FloatingPointError', 'FutureWarning', 
    'GeneratorExit', 'IOError', 'ImportError', 'ImportWarning', 'IndentationError', 'IndexError', 'KeyError', 'KeyboardInterrupt', 
    'LookupError', 'MemoryError', 'NameError', 'None', 
    'NotImplemented', 'NotImplementedError', 'OSError', 'OverflowError', 'PendingDeprecationWarning', 'ReferenceError', 'RuntimeError', 'RuntimeWarning', 
    'StandardError', 'StopIteration', 'SyntaxError', 'SyntaxWarning', 'SystemError', 'SystemExit', 'TabError', 'True', 'TypeError', 'UnboundLocalError', 
    'UnicodeDecodeError', 'UnicodeEncodeError', 'UnicodeError', 'UnicodeTranslateError', 'UnicodeWarning', 
    'UserWarning', 'ValueError', 'Warning', 'ZeroDivisionError', '__debug__', '__doc__', '__import__', '__name__', '__package__', 
    'abs', 'all', 'any', 'apply', 'basestring', 'bin', 'bool', 'buffer', 'bytearray', 'bytes', 'callable', 'chr', 'classmethod', 
    'cmp', 'coerce', 'compile', 'complex', 'copyright', 'credits', 'delattr', 'dict', 'dir', 'divmod', 
    'enumerate', 'eval', 'execfile', 'exit', 'file', 'filter', 'float', 'format', 'frozenset', 'getattr', 
    'globals', 'hasattr', 'hash', 'help', 'hex', 'id', 'input', 'int', 'intern', 'isinstance', 
    'issubclass', 'iter', 'len', 'license', 'list', 'locals', 'long', 'map', 'max', 'min', 'next', 
    'object', 'oct', 'open', 'ord', 'pow', 'print', 'property', 'quit', 'range', 'raw_input', 'reduce', 'reload', 
    'repr', 'reversed', 'round', 'set', 'setattr', 'slice', 'sorted', 'staticmethod', 'str', 
    'sum', 'super', 'tuple', 'type', 'unichr', 'unicode', 'vars', 'xrange', 'zip') 
    

    def __init__(self, edit):
        self.textedit = edit
        document = edit.document()
        QSyntaxHighlighter.__init__(self, document)

        base_format = QTextCharFormat()
        base_format.setFont(edit.font())
        self.base_format = base_format
        self.document = document
        
        self.updateHighlighter(base_format.font())


    def highlightBlock(self, text):
        self.setCurrentBlockState(0)
        
        if text.trimmed().isEmpty():
            self.setFormat(0, len(text), self.empty_format)
            return
        
        self.setFormat(0, len(text), self.base_format)
        
        startIndex = 0
        if self.previousBlockState() != 1:
            startIndex = self.multiLineStringBegin.indexIn(text)
        
        if startIndex > -1:
            self.highlightRules(text, 0, startIndex)
        else:
            self.highlightRules(text, 0, len(text))
        
        while startIndex >= 0:
            endIndex = self.multiLineStringEnd.indexIn(text, 
                  startIndex + len(self.multiLineStringBegin.pattern()))
            if endIndex == -1:
                self.setCurrentBlockState(1)
                commentLength = text.length() - startIndex
            else:
                commentLength = endIndex - startIndex + \
                                self.multiLineStringEnd.matchedLength()
                self.highlightRules(text, endIndex, len(text))
            
            self.setFormat(startIndex, commentLength, self.multiLineStringFormat)
            startIndex = self.multiLineStringBegin.indexIn(text, 
                                           startIndex + commentLength)
    
    
    def highlightRules(self, text, start, finish):
        for expression, format in self.rules:
            index = expression.indexIn(text, start)
            while index >= start and index < finish:
                length = expression.matchedLength()
                self.setFormat(index, min(length, finish - index), format)
                index = expression.indexIn(text, index + length)
    

    def updateFonts(self, font):    
        self.base_format.setFont(font)
        self.empty_format = QTextCharFormat(self.base_format)
        #self.empty_format.setFontPointSize(font.pointSize()/4.0)
        
        self.keywordFormat = QTextCharFormat(self.base_format)
        self.keywordFormat.setForeground(QColor(scheme.syntax_keyword))
        self.keywordFormat.setFontWeight(QFont.Bold)
        self.builtinFormat = QTextCharFormat(self.base_format)
        self.builtinFormat.setForeground(QColor(scheme.syntax_builtin))
        self.magicFormat = QTextCharFormat(self.base_format)
        self.magicFormat.setForeground(QColor(scheme.syntax_magic))
        #self.qtFormat = QTextCharFormat(self.base_format)
        #self.qtFormat.setForeground(QColor(scheme.syntax_qt))
        ##self.qtFormat.setFontWeight(QFont.Bold)
        self.selfFormat = QTextCharFormat(self.base_format)
        self.selfFormat.setForeground(QColor(scheme.syntax_self))
        #self.selfFormat.setFontItalic(True)
        self.singleLineCommentFormat = QTextCharFormat(self.base_format)
        self.singleLineCommentFormat.setForeground(QColor(scheme.syntax_comment))
        self.singleLineCommentFormat.setFontItalic(True)
        self.multiLineStringFormat = QTextCharFormat(self.base_format)
        self.multiLineStringFormat.setForeground(QColor(scheme.syntax_string))
        #self.multiLineStringFormat.setBackground(QBrush(Qt.green))
        self.quotationFormat1 = QTextCharFormat(self.base_format)
        self.quotationFormat1.setForeground(QColor(scheme.syntax_string))
        self.quotationFormat2 = QTextCharFormat(self.base_format)
        self.quotationFormat2.setForeground(QColor(scheme.syntax_string))
        self.numFormat = QTextCharFormat(self.base_format)
        self.numFormat.setForeground(QColor(scheme.syntax_number))


    def updateRules(self):
        self.rules = []
        self.rules += map(lambda s: (QRegExp(r"\b"+s+r"\b"),
                          self.keywordFormat), self.keywords)
        self.rules += map(lambda s: (QRegExp(r"\b"+s+r"\b"),
                          self.builtinFormat), self.builtins)

        self.rules.append((QRegExp(r"\b__[a-z]+__\b"), self.magicFormat))
        self.rules.append((QRegExp(r"\bself\b"), self.selfFormat))
        self.rules.append((QRegExp(r"\b\d+(\.\d*)?\b"), self.numFormat))
        #self.rules.append((QRegExp(r"\bQ([A-Z][a-z]*)+\b"), self.qtFormat))
        self.rules.append((QRegExp(r"#[^\n]*"), self.singleLineCommentFormat))
        self.multiLineStringBegin = QRegExp(r'\"\"\"')
        self.multiLineStringEnd = QRegExp(r'\"\"\"')
        self.rules.append((QRegExp(r'\"[^\n]*\"'), self.quotationFormat1))
        self.rules.append((QRegExp(r"'[^\n]*'"), self.quotationFormat2))
    

    def updateHighlighter(self, font):    
        self.updateFonts(font)
        self.updateRules()
        self.setDocument(self.document)




class EditorWidget(QPlainTextEdit):

    
    def __init__(self, parent):
        super(EditorWidget, self).__init__(parent)
        self.highlighter = PythonHighlighter(self)

        
    def keyPressEvent(self, event):
        key = event.key()
        if key == Qt.Key_Tab:
            self.textCursor().insertText(" " * 4)
            event.accept()
        elif key in (Qt.Key_Return, Qt.Key_Enter):
            cursor = self.textCursor()
            line = unicode(cursor.block().text())
            indent = (len(line) - len(line.lstrip(" "))) / 4
            cursor.insertText("\n")
            if line.endswith(":"):
                indent += 1
            elif line.strip().startswith(("return", "pass")):
                indent = max(indent - 1, 0)
            cursor.insertText(" " * (indent*4))
            event.accept()
        else:
            return super(EditorWidget, self).keyPressEvent(event)



class OutputWidget(QPlainTextEdit):

    
    def __init__(self, parent, autoshow=False):
        super(OutputWidget, self).__init__(parent)
        self.autoshow = autoshow
        self.setReadOnly(True)
        sys.stdout = self
        sys.stderr = self
        self.buf = u""
        self.startTimer(500)

        
    def write(self, s):
        sys.__stdout__.write(s)
        self.buf += unicode(s)

        
    def writeline(self, s):
        self.write(u"%s\n" % s)

        
    def timerEvent(self, event):
        self.flush()

        
    def flush(self):
        if self.buf:
            if self.autoshow and not self.window().isVisible():
                self.window().show()
            self.moveCursor(QTextCursor.End, QTextCursor.MoveAnchor)
            self.textCursor().insertText(self.buf)          
            self.moveCursor(QTextCursor.End, QTextCursor.MoveAnchor)
            self.ensureCursorVisible()
            self.buf = ""


    
class Console(QDialog):


    def __init__(self, execute_function, example=example, autoshow=False):
        QDialog.__init__(self)
        self.execute = execute_function
        self.vblayout = QVBoxLayout(self)
        self.splitter = QSplitter(Qt.Vertical, self)
        self.layout().addWidget(self.splitter)
        self.edit = EditorWidget(self.splitter)
        self.edit.setPlainText(example)
        self.output = OutputWidget(self.splitter, autoshow=autoshow)
        self.runButton = QPushButton(self, text="&Run")
        self.runButton.clicked.connect(self.run)
        self.layout().addWidget(self.runButton)
        self.edit.setFocus()
        self.resize(800, 600)
        

    def run(self):
        code = unicode(self.edit.toPlainText())
        self.execute(code)
    
        

        

        
if __name__ == "__main__":
    import sys
    from PyQt4.QtGui import QApplication
    app = QApplication(sys.argv)
    win = Console()
    win.show()
    app.exec_()
    