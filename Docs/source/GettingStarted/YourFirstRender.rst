*****************************
Your first render with Cinder
*****************************

Cinder uses an easy to understand and write YAML format for project and scene serialization.
You can go through all the scenes used in this documentation in the ``Cinder/Projects`` folder.

We will be rendering the Cornell Box in this introduction to the Cinder CLI Tool.

Cinder Projects and Scenes
==========================

To render something with Cinder, you need to have two things: a project, and a scene.
The two are closely related, but both have different uses.

Projects
--------

A project is used to setup *how* you want Cinder to render scenes. It decides what type of renderer to use, and what kind of images to output.
It also contains a list of scenes to be rendered, and the output image of each of them.

Scenes
------

A scene is exactly what the name suggests. It consists of a camera, objects, their consisting geometry, materials, and lights.

Rendering the Cornell Box
=========================

The Cornell Box is a famous test which determines the accuracy of rendering software, as it is a real place, 
and has all the relevant data captured. You can check the data out at `Cornell <https://www.graphics.cornell.edu/online/box/data.html>`_.

Rendering the Cornell Box with Cinder is a fairly simple task. 

Our first step is to find the Cinder binaries.
If you used the Python build script, you should find the Binaries in ``Build/{Config}/Binaries``, where ``{Config}``
is the configuration you chose while building (this is Release by default).
If you built it directly with CMake, they will be in ``{BuildDir}/Binaries``, where ``{BuildDir}`` is the CMake output directory.

.. warning::
    If you're on Windows and decide to move the Cinder executable outside the directory, ensure that you
    also move ``CinderAPI.dll`` along with it, and keep them both in the same folder. On Linux, ensure you do not delete or move
    ``libCinder.so`` out of ``Build/{Config}/Libraries``

Assuming the binaries are in their default directory, you just need to run the following command in a terminal to begin rendering.
Ensure that the terminal's working directory is the root of the Cinder project.

.. code-block:: none

    Build/Release/Binaries/cinder Projects/CornellBox/Project.yaml

.. note::
    You will have to replace all the path separators ('/') with '\\' on Windows.

After the render is complete, you should see a file ``CornellBox.png`` in ``Projects/CornellBox/``.
