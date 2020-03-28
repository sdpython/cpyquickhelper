# -*- coding: utf-8 -*-
import sys
import os
import warnings
from setuptools import setup, Extension
from setuptools import find_packages

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
    project_var_name + ".examples": ["*.cpp", "*.hpp", "*.h"],
    project_var_name + ".io": ["*.cpp", "*.hpp"],
    project_var_name + ".numbers": ["*.cpp", "*.h", "*.pyx"],
    project_var_name + ".parallel": ["*.cpp", "*.hpp"],
}

############
# functions
############


def ask_help():
    return "--help" in sys.argv or "--help-commands" in sys.argv


def is_local():
    file = os.path.abspath(__file__).replace("\\", "/").lower()
    if "/temp/" in file and "pip-" in file:
        return False
    try:
        from pyquickhelper.pycode.setup_helper import available_commands_list
    except ImportError:
        return False
    return available_commands_list(sys.argv)


def verbose():
    print("---------------------------------")
    print("package_dir =", package_dir)
    print("packages    =", packages)
    print("package_data=", package_data)
    print("current     =", os.path.abspath(os.getcwd()))
    print("---------------------------------")

##########
# version
##########


if is_local() and not ask_help():
    def write_version():
        from pyquickhelper.pycode import write_version_for_setup
        return write_version_for_setup(__file__)

    try:
        write_version()
        subversion = None
    except Exception:
        subversion = ""

    if subversion is None:
        versiontxt = os.path.join(os.path.dirname(__file__), "version.txt")
        if os.path.exists(versiontxt):
            with open(versiontxt, "r") as f:
                lines = f.readlines()
            subversion = "." + lines[0].strip("\r\n ")
            if subversion == ".0":
                raise Exception(
                    "Git version is wrong: '{0}'.".format(subversion))
        else:
            subversion = ""
else:
    # when the module is installed, no commit number is displayed
    subversion = ""

if "upload" in sys.argv and not subversion and not ask_help():
    # avoid uploading with a wrong subversion number
    raise Exception(
        "Git version is empty, cannot upload, is_local()={0}".format(is_local()))

########
# pybind11
########


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


##############
# common part
##############

if os.path.exists(readme):
    with open(readme, "r", encoding='utf-8-sig') as f:
        long_description = f.read()
else:
    long_description = ""
if os.path.exists(history):
    with open(history, "r", encoding='utf-8-sig') as f:
        long_description += f.read()

if "--verbose" in sys.argv:
    verbose()

if is_local():
    import pyquickhelper
    logging_function = pyquickhelper.get_fLOG()
    logging_function(OutputPrint=True)
    must_build, run_build_ext = pyquickhelper.get_insetup_functions()

    if must_build():
        out = run_build_ext(__file__)
        print(out)

    if "build_sphinx" in sys.argv and not sys.platform.startswith("win"):
        # There is an issue with matplotlib and notebook gallery on linux
        # _tkinter.TclError: no display name and no $DISPLAY environment variable
        import matplotlib
        matplotlib.use('agg')

    from pyquickhelper.pycode import process_standard_options_for_setup
    r = process_standard_options_for_setup(
        sys.argv, __file__, project_var_name,
        unittest_modules=["pyquickhelper"],
        additional_notebook_path=["pyquickhelper", "jyquickhelper"],
        additional_local_path=["pyquickhelper", "jyquickhelper"],
        requirements=["pyquickhelper", "jyquickhelper", "pybind11"],
        layout=["html"], github_owner='sdpython',
        add_htmlhelp=sys.platform.startswith("win"),
        coverage_options=dict(omit=["*exclude*.py"]),
        fLOG=logging_function, covtoken=("5091a257-f079-417e-af3b-dc425ddda031", "'_UT_37_std' in outfile"))
    if not r and not ({"bdist_msi", "sdist",
                       "bdist_wheel", "publish", "publish_doc", "register",
                       "upload_docs", "bdist_wininst", "build_ext"} & set(sys.argv)):
        raise Exception("unable to interpret command line: " + str(sys.argv))
else:
    r = False

if ask_help():
    from pyquickhelper.pycode import process_standard_options_for_setup_help
    process_standard_options_for_setup_help(sys.argv)

if not r:
    if len(sys.argv) in (1, 2) and sys.argv[-1] in ("--help-commands",):
        from pyquickhelper.pycode import process_standard_options_for_setup_help
        process_standard_options_for_setup_help(sys.argv)
    try:
        from pyquickhelper.pycode import clean_readme
        long_description = clean_readme(long_description)
    except ImportError:
        long_description = ""
    from cpyquickhelper import __version__ as sversion
    root = os.path.abspath(os.path.dirname(__file__))

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
        extra_compile_args_thread = ['-lpthread', '-fopenmp']
        # option -mavx512f enable AVX 512 instructions
        # see https://blog.qiqitori.com/?p=390
        # , '-o2', '-mavx512f']
        extra_compile_args_numbers = ['-fopenmp']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
        extra_link_args = ['-lgomp']
        define_macros = [('USE_OPENMP', None)]

    # extensions

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
        [os.path.join(root, 'cpyquickhelper/numbers/cbenchmark_sum_type.cpp')],
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

    # cython and numbers
    import numpy
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
            ext_slowcode,
            ext_custom_container,
            ext_thread, ext_stdhelper,
            ext_numbers, ext_benchmark,
            ext_benchmark_dot,
            ext_benchmark_sum_type
        ])

    setup(
        name=project_var_name,
        ext_modules=ext_modules,
        version=sversion,
        author='Xavier Dupré',
        author_email='xavier.dupre@gmail.com',
        license="MIT",
        url="http://www.xavierdupre.fr/app/cpyquickhelper/helpsphinx/index.html",
        download_url="https://github.com/sdpython/cpyquickhelper/",
        description=DESCRIPTION,
        long_description=long_description,
        keywords=KEYWORDS,
        classifiers=CLASSIFIERS,
        packages=packages,
        package_dir=package_dir,
        package_data=package_data,
        setup_requires=["pybind11", "cython", "scipy", "numpy"],
        install_requires=["pybind11", "numpy>=1.16",
                          "cython", 'scipy'],
    )
