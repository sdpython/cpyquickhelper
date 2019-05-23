# -*- coding: utf-8 -*-
import sys
import os
from setuptools import setup, Extension
from setuptools import find_packages
import numpy
from Cython.Build import cythonize

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

here = os.path.dirname(__file__)
packages = find_packages(where=here)
package_dir = {k: os.path.join(here, k.replace(".", "/")) for k in packages}
package_data = {
    project_var_name + ".js": ["*.js", "*.css"],
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

    write_version()

    versiontxt = os.path.join(os.path.dirname(__file__), "version.txt")
    if os.path.exists(versiontxt):
        with open(versiontxt, "r") as f:
            lines = f.readlines()
        subversion = "." + lines[0].strip("\r\n ")
        if subversion == ".0":
            raise Exception("Git version is wrong: '{0}'.".format(subversion))
    else:
        raise FileNotFoundError(versiontxt)
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
    from pyquickhelper.pycode import clean_readme
    from cpyquickhelper import __version__ as sversion
    long_description = clean_readme(long_description)
    root = os.path.abspath(os.path.dirname(__file__))

    if sys.platform.startswith("win"):
        libraries_thread = ['kernel32']
        extra_compile_args_thread = None
        extra_compile_args_numbers = ['/EHsc', '/O2', '/Gy']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
    elif sys.platform.startswith("darwin"):
        libraries_thread = None
        extra_compile_args_thread = ['-lpthread', '-stdlib=libc++', '-std=c++11',
                                     '-mmacosx-version-min=10.7']
        extra_compile_args_numbers = ['-stdlib=libc++', '-mmacosx-version-min=10.7',
                                      '-std=c++11']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
        extra_compile_args_bench.append('-fpermissive')
    else:
        libraries_thread = None
        extra_compile_args_thread = ['-lpthread', '-std=c++11']
        # option -mavx512f enable AVX 512 instructions
        # see https://blog.qiqitori.com/?p=390
        extra_compile_args_numbers = ['-std=c++11']  # , '-o2', '-mavx512f']
        extra_compile_args_bench = extra_compile_args_numbers.copy()
        extra_compile_args_bench.append('-fpermissive')

    # extensions

    ext_thread = Extension('cpyquickhelper.parallel.threader',
                           [os.path.join(root, 'cpyquickhelper/parallel/threaderc.cpp'),
                            os.path.join(root, 'cpyquickhelper/parallel/threader.cpp')],
                           extra_compile_args=extra_compile_args_thread,
                           include_dirs=[os.path.join(
                               root, 'cpyquickhelper/parallel')],
                           libraries=libraries_thread)

    ext_stdhelper = Extension('cpyquickhelper.io.stdchelper',
                              [os.path.join(root, 'cpyquickhelper/io/stdchelper.cpp'),
                               os.path.join(root, 'cpyquickhelper/io/stdcapture.cpp')],
                              extra_compile_args=extra_compile_args_thread,
                              include_dirs=[os.path.join(root, 'cpyquickhelper/io')])

    ext_numbers = Extension('cpyquickhelper.numbers.weighted_number',
                            [os.path.join(root, 'cpyquickhelper/numbers/weighted_number.cpp'),
                             os.path.join(root, 'cpyquickhelper/numbers/weighted_number_python.cpp')],
                            extra_compile_args=extra_compile_args_numbers,
                            include_dirs=[
                                # Path to pybind11 headers
                                get_pybind_include(),
                                get_pybind_include(user=True),
                                os.path.join(
                                    root, 'cpyquickhelper/numbers')
                            ],
                            language='c++')

    ext_benchmark = Extension('cpyquickhelper.numbers.cbenchmark',
                              [os.path.join(
                                  root, 'cpyquickhelper/numbers/cbenchmark.cpp')],
                              extra_compile_args=extra_compile_args_numbers,
                              include_dirs=[
                                  # Path to pybind11 headers
                                  get_pybind_include(),
                                  get_pybind_include(user=True),
                                  os.path.join(
                                      root, 'cpyquickhelper/numbers')
                              ],
                              language='c++')

    ext_benchmark_dot = Extension('cpyquickhelper.numbers.cbenchmark_dot',
                                  [os.path.join(
                                      root, 'cpyquickhelper/numbers/cbenchmark_dot.cpp')],
                                  extra_compile_args=extra_compile_args_bench,
                                  include_dirs=[
                                      # Path to pybind11 headers
                                      get_pybind_include(),
                                      get_pybind_include(user=True),
                                      os.path.join(
                                          root, 'cpyquickhelper/numbers')
                                  ],
                                  language='c++')

    ext_benchmark_sum_type = Extension('cpyquickhelper.numbers.cbenchmark_sum_type',
                                       [os.path.join(
                                           root, 'cpyquickhelper/numbers/cbenchmark_sum_type.cpp')],
                                       extra_compile_args=extra_compile_args_bench,
                                       include_dirs=[
                                           # Path to pybind11 headers
                                           get_pybind_include(),
                                           get_pybind_include(user=True),
                                           os.path.join(
                                               root, 'cpyquickhelper/numbers')
                                       ],
                                       language='c++')

    # cython and numbers

    pattern1 = "cpyquickhelper.numbers.%s"
    name = 'direct_blas_lapack'
    ext_blas = Extension(pattern1 % name,
                         ['cpyquickhelper/numbers/%s.pyx' % name],
                         include_dirs=[numpy.get_include()],
                         extra_compile_args=["-O3"])

    # cythonize

    opts = dict(boundscheck=False, cdivision=True,
                wraparound=False, language_level=3,
                cdivision_warnings=True)
    ext_modules = cythonize([ext_blas], compiler_directives=opts)

    # setup
    ext_modules.extend([
        ext_thread, ext_stdhelper,
        ext_numbers, ext_benchmark,
        ext_benchmark_dot,
        ext_benchmark_sum_type
    ])

    setup(
        name=project_var_name,
        ext_modules=ext_modules,
        version='%s%s' % (sversion, subversion),
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
        setup_requires=["pyquickhelper", "pybind11"],
        install_requires=["pybind11", "numpy",
                          "cython", 'scipy'],
    )
