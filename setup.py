#!/usr/bin/env python

from distutils.core import setup

setup(name='G27_Pedals_and_Shifter_GUI',
      version='0.1',
      description='Calibration GUI for the G27 Arduino adapter.',
      author='functionreturnfunction,NEYS',
      url='https://github.com/n-e-y-s/G27_Pedals_and_Shifter_GUI',
      packages=['g27_pedals_and_shifter_gui'],
      install_requires=['PySide2>=5.15.0', 'pyqtgraph', 'pyserial']
     )
