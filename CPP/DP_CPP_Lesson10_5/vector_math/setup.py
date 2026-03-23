from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'vector_math',
        ['vector3d.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
]

setup(
    name='vector_math',
    version='1.0.0',
    description='Vector3D module using Pybind11',
    ext_modules=ext_modules,
    zip_safe=False,
    python_requires='>=3.6',
)