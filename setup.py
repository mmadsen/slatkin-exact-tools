from distutils.core import setup, Extension


setup(name = "slatkintools",
	  version = '1.0',
	  description = 'Python extension wrapping a modernized version of the "exact" test for neutrality by Montgomery Slatkin exact test',
	  author = 'Mark E. Madsen (and Montgomery Slatkin)',
	  author_email = "mark@madsenlab.org",
	  url = "https://github.com/mmadsen/slatkin-exact-tools",
	  py_modules = ['slatkin'],
	  ext_modules = [Extension('_slatkin', sources = ['slatkin.c', 'mersenne.c', 'slatkin.i'])],
	)

