*****************
Setting up Cinder
*****************

Dependencies
============

Compiling Cinder requires the following dependencies to be installed:

* :ref:`A suitable C++ toolchain <Toolchain>`
* :ref:`CMake <CMake>`
* :ref:`Ninja <Ninja>`
* :ref:`Python <Python>`
* :ref:`Doxygen <Doxygen>`
* :ref:`Sphinx <Sphinx>`

Doxygen and Sphinx are only required for building the documentation (what you're seeing now). See :ref:`Build`.

.. _Toolchain:

Getting a C++ toolchain
-----------------------

On Linux
^^^^^^^^^

You should already have GCC pre-installed.

On Mac
^^^^^^^

Open a Terminal instance by searching for it in Spotlight. Then enter the following command:

.. code-block:: none

   xcode-select --install

On Windows
^^^^^^^^^^^

#. Download Visual Studio 2019 Community from `here <https://visualstudio.microsoft.com>`_.
#. In the installer, make sure to select the 'Desktop Development with C++' component.

.. _CMake:

Getting CMake
-------------

Download and install the correct binary distribution of CMake for your platform from `here <https://cmake.org/download/>`_.

.. _Ninja:

Getting Ninja
-------------

Download and install Ninja from `here <https://ninja-build.org>`_.

.. _Python:

Getting Python
--------------

Download Python from `here <https://www.python.org/downloads/>`_.

.. _Doxygen:

Getting Doxygen
---------------

Doxygen is not required if you do not want to build the documentation.
Download the right version of Doxygen from `here <https://www.doxygen.nl/download.html>`_.

.. _Sphinx:

Getting Sphinx
--------------

Sphinx is not required if you do not want to build the documentation.

#. Open a terminal window. On Mac, search it in Spotlight, and on Windows, search for 'Command Prompt'.
#. Enter the following command (requires Python):

.. code-block:: none

   pip install -U Sphinx

Cloning
=======

If you have Git simply call:

.. code-block:: none

   git clone https://github.com/SparkyPotato/Cinder --recursive

Or else, you can head over to `GitHub <https://github.com/SparkyPotato/Cinder>`_,
and download the repository by clicking on 'Code' and then 'Download ZIP'.

.. _Build:

Building
========

.. note::
   If you do not wish to build the documentation, you do not need to install Doxygen or Sphinx.
   To disable documentation building, add the option ``-nodoc`` while invoking ``Build.py``.

#. Open another terminal window, and navigate into the ``Cinder`` directory you just cloned.
   On Windows, you might need to open a Developer Command Prompt. Just search for this in the Start Menu.
#. To quickly get up and running, just call:

.. code-block:: none

   python3 Build.py

If you want to know all the options you can set in the build script, call:

.. code-block:: none

   python3 Build.py -h

.. note::
   If an instance of Visual Studio is opened, CMake generation could fail.
   Just keep running the build script until it works, or close Visual Studio.
