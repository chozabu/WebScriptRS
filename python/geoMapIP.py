from embeddedpyqt import *
import sys
from PyQt4 import QtGui

from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
import matplotlib.pyplot as plt

from mpl_toolkits.basemap import Basemap

import random

import pygeoip
gi = pygeoip.GeoIP('/opt/geolite/GeoLiteCity.dat', pygeoip.MEMORY_CACHE)

class MainWidget(QtGui.QWidget):
    def __init__(self, parent=None):
        super(MainWidget, self).__init__(parent)

        # a figure instance to plot on
        self.figure = plt.figure()

        # this is the Canvas Widget that displays /embpyqt/python/the `figure`
        # it takes the `figure` instance as a parameter to __init__
        self.canvas = FigureCanvas(self.figure)

        # this is the Navigation widget
        # it takes the Canvas widget and a parent
        self.toolbar = NavigationToolbar(self.canvas, self)

        # Just some button connected to `plot` method
        self.button = QtGui.QPushButton('Plot')
        self.button.clicked.connect(self.plot)

        # set the layout
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.toolbar)
        layout.addWidget(self.canvas)
        layout.addWidget(self.button)
        self.setLayout(layout)

    def plot(self):
        ''' plot some random stuff '''
        # random data
        '''data = [random.random() for i in range(2)]

        # create an axis
        ax = self.figure.add_subplot(111)

        # discards the old graph
        ax.hold(False)

        # plot data
        ax.plot(data, '*-')'''
        m = Basemap(projection='robin',lon_0=0,resolution='c')#,latlon=True)
        m.bluemarble(scale=0.2)
        for friend in bridge.getFriendList():
          print ''
          pd = bridge.getPeerDetails(friend)
          print pd['name']
          print pd['extAddr']
          ld = gi.record_by_addr(pd['extAddr'])
          print ld
          if ld:
            print ld['latitude'],ld['longitude']
            x, y = m(ld['longitude'],ld['latitude'])
            #m.scatter(x, y,30,marker='o',color='k')
            plt.plot(x, y,'ro')
            plt.text(x,y,pd['name'],fontsize=9,
                    ha='center',va='top',color='r',
                    bbox = dict(boxstyle="square",ec='None',fc=(1,1,1,0.5)))
            #plt.text(x,y,pd['name'],fontsize=14,fontweight='bold',
            #        ha='center',va='center',color='r')
        # refresh canvas
        self.canvas.draw()


class Window(QtGui.QDialog):
    def __init__(self, parent=None):
        super(Window, self).__init__(parent)
        layout = QtGui.QVBoxLayout()
        layout.addWidget(MainWidget())
        self.setLayout(layout)
def setParent(incoming):
  incoming.addWidget(MainWidget())
print "hmm"
a=MainWidget()
a.show()
if __name__ == '__main__':
  print "running as main"
  app = QtGui.QApplication(sys.argv)
  main = Window()
  #main.addWidget(Window)
  main.show()
  sys.exit(app.exec_())