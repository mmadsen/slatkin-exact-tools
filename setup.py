from distutils.core import setup, Extension

se = Extension('slatkinexact',
				 sources = ['slatkin-common.c', 'mersenne.c', 'slatkinexact_wrap.c']
				)

setup(name = "SlatkinExact",
	  version = '1.0',
	  description = 'Python extension wrapping a modernized version of Montgomery Slatkin\'s exact test',
	  author = 'Mark E. Madsen (and Montgomery Slatkin)',
	  author_email = "mark@madsenlab.org",
	  url = "https://github.com/mmadsen/slatkin-exact-tools",
	  ext_modules = [se],
      py_modules = ["slatkinexact"],
	)

