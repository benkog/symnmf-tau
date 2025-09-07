from setuptools import Extension, setup

module = Extension("symnmfmodule", sources=['symnmfmodule.c', 'symnmf.c', 'python_utils.c'])
setup(name='symnmfmodule',
     version='1.0',
     description='A module that contains implementations for all parts of the symNMF algorithm.',
     ext_modules=[module])