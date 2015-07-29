from distutils.core import setup, Extension
from Cython.Build import cythonize



ext = Extension(name="slatkin", sources=["slatkin_impl.c", "mersenne.c", "slatkin.pyx"])


setup(name = "slatkin",
	  version = '1.0',
	  description = 'Python extension wrapping a modernized version of the "exact" test for neutrality by Montgomery Slatkin exact test',
	  author = 'Mark E. Madsen (and Montgomery Slatkin)',
	  author_email = "mark@madsenlab.org",
	  url = "https://github.com/mmadsen/slatkin-exact-tools",
      ext_modules=cythonize(ext)
      )

