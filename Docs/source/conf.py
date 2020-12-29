# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'Cinder'
copyright = '2021 SparkyPotato'
author = 'SparkyPotato'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
	'breathe'
]

breathe_default_project = 'Cinder'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['Templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
html_theme_path = ['.']
html_theme_options = {
    'style_nav_header_background': '#ff8c00',
    'collapse_navigation': True
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['Static']

import subprocess, os

def ConfigureDoxygen(inputDir, outputDir):
    with open('Doxyfile.in', 'r') as file:
        filedata = file.read()

    filedata = filedata.replace('@DOXYGEN_INPUT_DIR@', inputDir)
    filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', outputDir)

    with open('Doxyfile', 'w') as file:
        file.write(filedata)

onRTD = os.environ.get('READTHEDOCS', None) == 'True'

breathe_projects = {}

if onRTD:
    os.chdir(os.path.realpath(os.path.dirname(os.path.dirname(__file__))))
    inputDir = '..'
    outputDir = 'Build'
    ConfigureDoxygen(inputDir, outputDir)
    subprocess.call('doxygen', shell=True)
    breathe_projects['Cinder'] = outputDir + '/xml'
