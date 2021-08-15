# -*- coding: utf-8 -*-
import sys
import os
import platform
import warnings
from setuptools import setup, Extension, find_packages
from pyquicksetup import read_version, read_readme, default_cmdclass

#########
# settings
#########

project_var_name = "cpyquickhelper"
versionPython = "%s.%s" % (sys.version_info.major, sys.version_info.minor)
path = "Lib/site-packages/" + project_var_name
readme = 'README.rst'
history = "HISTORY.rst"
requirements = None

KEYWORDS = project_var_name + ', Xavier Dupré'
DESCRIPTION = """Module with C functions. No precise purpose yet."""
CLASSIFIERS = [
    'Programming Language :: Python :: 3',
    'Intended Audience :: Developers',
    'Topic :: Scientific/Engineering',
    'Topic :: Education',
    'License :: OSI Approved :: MIT License',
    'Development Status :: 5 - Production/Stable'
]


#######
# data
#######

packages = find_packages()
package_dir = {k: os.path.join('.', k.replace(".", "/")) for k in packages}
package_data = {
    project_var_name + ".algorithms": ["*.cpp", "*.hpp"],
    project_var_name + ".examples": ["*.cpp", "*.hpp", "*.h"],
    project_var_name + ".io": ["*.cpp", "*.hpp"],
    project_var_name + ".numbers": ["*.cpp", "*.h", "*.pyx"],
    project_var_name + ".parallel": ["*.cpp", "*.hpp"],
}


class get_pybind_include(object):
    """
    Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked.
    `Source <https://github.com/pybind/python_example/blob/master/setup.py>`_.
    """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


def get_compile_args():

    if sys.platform.startswith("win"):
        libraries_thread = ['kernel32']
        extra_compile_args_thread = None
        extra_compile_args_numbers = ['/EHsc', '/O2', '/Gy', '/openmp']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
        extra_link_args = None
        define_macros = [('USE_OPENMP', None)]
    elif sys.platform.startswith("darwin"):
        libraries_thread = None
        extra_compile_args_thread = ['-lpthread', '-stdlib=libc++', '-std=c++11',
                                     '-mmacosx-version-min=10.7', '-Xpreprocessor',
                                     '-fopenmp']
        extra_compile_args_numbers = ['-stdlib=libc++', '-mmacosx-version-min=10.7',
                                      '-std=c++11', '-Xpreprocessor', '-fopenmp']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
        extra_link_args = ["-lomp"]
        define_macros = [('USE_OPENMP', None)]
    else:
        libraries_thread = None
        extra_compile_args_thread = ['-lpthread', '-fopenmp', '-std=c++11']
        # option -mavx512f enable AVX 512 instructions
        # see https://blog.qiqitori.com/?p=390
        # , '-o2', '-mavx512f']
        extra_compile_args_numbers = ['-fopenmp', '-std=c++11']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
        extra_link_args = ['-lgomp']
        define_macros = [('USE_OPENMP', None)]
    return (libraries_thread, extra_compile_args_numbers,
            extra_compile_args_bench, extra_compile_args_thread,
            extra_link_args, define_macros)


def get_extensions():
    import numpy
    root = os.path.abspath(os.path.dirname(__file__))
    (libraries_thread, extra_compile_args_numbers,
     extra_compile_args_bench, extra_compile_args_thread,
     extra_link_args, define_macros) = get_compile_args()

    ext_edit_distance_c = Extension(
        'cpyquickhelper.algorithms.edit_distance_c',
        [os.path.join(root, 'cpyquickhelper/algorithms/edit_distance_c.cpp')],
        extra_compile_args=extra_compile_args_numbers,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/algorithms')
        ],
        language='c++',
        define_macros=define_macros)

    ext_custom_container = Extension(
        'cpyquickhelper.examples.custom_container_python',
        [os.path.join(root, 'cpyquickhelper/examples/custom_container.cpp'),
         os.path.join(root, 'cpyquickhelper/examples/custom_container_python.cpp')],
        extra_compile_args=extra_compile_args_numbers,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/examples')
        ],
        language='c++',
        define_macros=define_macros)

    ext_thread = Extension(
        'cpyquickhelper.parallel.threader',
        [os.path.join(root, 'cpyquickhelper/parallel/threaderc.cpp'),
         os.path.join(root, 'cpyquickhelper/parallel/threader.cpp')],
        extra_compile_args=extra_compile_args_thread,
        extra_link_args=extra_link_args,
        include_dirs=[os.path.join(root, 'cpyquickhelper/parallel')],
        libraries=libraries_thread,
        define_macros=define_macros)

    ext_stdhelper = Extension(
        'cpyquickhelper.io.stdchelper',
        [os.path.join(root, 'cpyquickhelper/io/stdchelper.cpp'),
         os.path.join(root, 'cpyquickhelper/io/stdcapture.cpp')],
        extra_compile_args=extra_compile_args_thread,
        extra_link_args=extra_link_args,
        include_dirs=[os.path.join(root, 'cpyquickhelper/io')],
        define_macros=define_macros)

    ext_numbers = Extension(
        'cpyquickhelper.numbers.weighted_number',
        [os.path.join(root, 'cpyquickhelper/numbers/weighted_number.cpp'),
         os.path.join(root, 'cpyquickhelper/numbers/weighted_number_python.cpp')],
        extra_compile_args=extra_compile_args_numbers,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/numbers')
        ],
        language='c++',
        define_macros=define_macros)

    ext_benchmark = Extension(
        'cpyquickhelper.numbers.cbenchmark',
        [os.path.join(root, 'cpyquickhelper/numbers/cbenchmark.cpp')],
        extra_compile_args=extra_compile_args_numbers,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/numbers')
        ],
        language='c++',
        define_macros=define_macros)

    ext_benchmark_dot = Extension(
        'cpyquickhelper.numbers.cbenchmark_dot',
        [os.path.join(root, 'cpyquickhelper/numbers/cbenchmark_dot.cpp')],
        extra_compile_args=extra_compile_args_bench,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/numbers')
        ],
        language='c++',
        define_macros=define_macros)

    ext_benchmark_sum_type = Extension(
        'cpyquickhelper.numbers.cbenchmark_sum_type',
        [os.path.join(
            root, 'cpyquickhelper/numbers/cbenchmark_sum_type.cpp')],
        extra_compile_args=extra_compile_args_bench,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/numbers')
        ],
        language='c++',
        define_macros=define_macros)

    ext_slowcode = Extension(
        'cpyquickhelper.numbers.slowcode',
        [os.path.join(root, 'cpyquickhelper/numbers/slowcode.cpp')],
        extra_compile_args=extra_compile_args_numbers,
        extra_link_args=extra_link_args,
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            os.path.join(root, 'cpyquickhelper/numbers')
        ],
        language='c++',
        define_macros=define_macros)

    pattern1 = "cpyquickhelper.numbers.%s"
    name = 'direct_blas_lapack'
    ext_blas = Extension(
        pattern1 % name,
        ['cpyquickhelper/numbers/%s.pyx' % name],
        include_dirs=[numpy.get_include()],
        extra_compile_args=["-O3"],
        define_macros=define_macros)

    # cythonize

    opts = dict(boundscheck=False, cdivision=True,
                wraparound=False, language_level=3,
                cdivision_warnings=True)

    try:
        from Cython.Build import cythonize
        ext_modules = cythonize([ext_blas], compiler_directives=opts)
    except ImportError:
        # Cython is not installed.
        warnings.warn(
            "cython is not installed. Only pure python subpckages will be available.")
        ext_modules = None

    # setup
    if ext_modules is not None:
        ext_modules.extend([
            ext_edit_distance_c,
            ext_slowcode,
            ext_custom_container,
            ext_thread, ext_stdhelper,
            ext_numbers, ext_benchmark,
            ext_benchmark_dot,
            ext_benchmark_sum_type
        ])
    return ext_modules


try:
    ext_modules = get_extensions()
except ImportError as e:
    warnings.warn(
        "Unable to build C++ extension with missing dependencies %r." % e)
    ext_modules = None


setup(
    name=project_var_name,
    ext_modules=ext_modules,
    version=read_version(__file__, project_var_name),
    author='Xavier Dupré',
    author_email='xavier.dupre@gmail.com',
    license="MIT",
    url="http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html",
    download_url="https://github.com/sdpython/cpyquickhelper/",
    description=DESCRIPTION,
    long_description=read_readme(__file__),
    cmdclass=default_cmdclass(),
    keywords=KEYWORDS,
    classifiers=CLASSIFIERS,
    packages=packages,
    package_dir=package_dir,
    package_data=package_data,
    setup_requires=["pybind11", "cython", "scipy", "numpy", "pyquicksetup"],
    install_requires=["pybind11", "numpy>=1.16",
                      "cython", 'scipy', 'pandas>=1.0'],
)
