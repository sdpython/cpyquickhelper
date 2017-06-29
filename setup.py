

import os
from setuptools import setup, Extension

root = os.path.abspath(os.path.dirname(__file__))

setup(name='cpyquickhelper', version='0.1',
      ext_modules=[
                Extension('src.cpyquickhelper.io.stdchelper', 
                            [os.path.join(root, 'src/cpyquickhelper/io/stdchelper.cpp'),
                             os.path.join(root, 'src/cpyquickhelper/io/stdcapture.cpp')])
                ])