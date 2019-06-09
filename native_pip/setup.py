from setuptools import Extension, setup

setup(
    name='pypiplib',
    version='1.0',
    description='Python Package with Pip Pattern C++ Extension',
    ext_modules=[
        Extension('pypiplib',
            sources=['pypiplib.cpp', 'piplib.cpp'],
            py_limited_api=True,
            language='c++',
            extra_compile_args=['-std=c++17']
    )]
)

# python setup.py build
