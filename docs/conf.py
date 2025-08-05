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
#import os
#import sys
#sys.path.insert(0, os.path.abspath('.'))

# -- Project information -----------------------------------------------------

project = 'GDSiMS'
copyright = '2025, Sara Perez Vizan'
author = 'Sara Perez Vizan'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ["breathe", "exhale", "sphinx_copybutton", "sphinx_toolbox.collapse"]

# Configuration for the breathe extension
# Which directory to read the Doxygen output from
breathe_projects = {"GDSiMS":"./doxygen/xml"}
breathe_default_project = "GDSiMS"

# Configuration for the exhale extension
exhale_args = {
    "containmentFolder": "./api",
    "doxygenStripFromPath": "..",
    "rootFileName": "library_root.rst",
    "rootFileTitle": "Code Documentation",
    "createTreeView":        True,
    "treeViewIsBootstrap":   True,
    "exhaleExecutesDoxygen": False
}

# Tell sphinx what the primary language being documented is.
primary_domain = "cpp"

# Tell sphinx what the pygments highlight language should be.
highlight_language = "cpp"

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_book_theme'
html_theme_options = {
    "repository_url": "https://github.com/AceRNorth/GeneralMetapop",
    "use_repository_button": True
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static', 'table_style.css']