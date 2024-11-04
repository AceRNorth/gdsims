
Installation
============
The C++ program can be installed from the source code on our GitHub repository (link on the icon above). The project uses CMake, a cross-platform tool, to build the software from multiple source files. The instructions for command-line installations are below. 

Requirements
------------
- C++17-compatible compiler or higher (e.g. MinGW g++ :math:`\geq` 8.0)
- CMake :math:`\geq` 3.5.0

.. seealso::
    `CMake <https://cmake.org/download/>`_ 
     CMake download page.
    `CMake documentation <https://cmake.org/cmake/help/latest/>`_
     Documentation for CMake, including a User Interaction Guide.
    `GCC <https://gcc.gnu.org/>`_ 
     GCC, the GNU compiler collection site. GCC includes g++, which is a C++ compiler. This site has links to download sites for different operating systems. 

Windows
-------
.. admonition:: Additional recommended installations 

    - Git Bash

#. Open the Windows command prompt and check CMake and the C++ compiler have correctly been installed. The correct version should be set to run as default.

   .. code-block:: bash

      cmake --version
      g++ --version
    
#. Downloading the files:

   i. Clone the repository via Git Bash (**recommended**):
      
      Install Git Bash and open. Navigate to your chosen directory and clone the repository here.
      
      .. code-block:: bash

         cd C:\Users\MyUser\Projects
         git clone https://github.com/AceRNorth/GeneralMetapop.git

   ii. Alternatively, download the files as a ZIP folder, unzip and move to your chosen directory.

       .. image:: images/install_github_zip.png
          :scale: 75 %

#. Open the Windows command prompt again and navigate to your chosen directory. Then, create a subdirectory called ``build`` where CMake files will be created, and navigate to it. 

   .. code-block:: bash

      cd C:\Users\MyUser\Projects\GeneralMetapop
      mkdir build
      cd build

#. Run CMake to configure the CMake project.

   .. code-block:: bash

      cmake ..

   .. hint::
      This step might not succeed as some Windows systems have issues selecting a generator. You can run ``cmake --help``  to view a list of available generators and then run CMake with one of them, e.g.

      .. code-block:: bash

         cmake -G "MinGW Makefiles" ..

#. Build all CMake targets.

   .. code-block:: bash

      cmake --build .

#. Finally, run the executable file ``gdsimsapp.exe`` that has been created in the build directory.

   .. code-block:: bash

      gdsimsapp.exe

.. caution::
    If you later make changes to the source code (such as to fully randomise results), repeat steps 5 and 6 to re-build and re-compile the program afterwards. 

Linux
-----
These instructions will closely follow those for Windows, with the exception of OS-specific commands and debugging tips. Linux does not require additional installations to clone the repository. 

#. Open the command prompt and check CMake and the C++ compiler have correctly been installed. The correct version should be set to run as default.

   .. code-block:: bash

      cmake --version
      g++ --version

   Some Linux distributions keep the default version set to an older version even after the newer one has been installed. A workaround to set this version will be covered in step 4.

#. Clone the repository: Navigate to your chosen directory and clone the repository here.

   .. code-block:: bash

      cd ./Projects
      git clone https://github.com/AceRNorth/GeneralMetapop.git

#. Navigate to the repository directory. Then, create a subdirectory called ``build`` where CMake files will be created, and navigate to it. 

   .. code-block:: bash

      cd GeneralMetapop
      mkdir build
      cd build

#. Run CMake to configure the CMake project.

   .. code-block:: bash

      cmake ..

   .. hint:: 
      This step might not succeed if the default compiler version doesn't support C++17 and its standard library. You can instead run cmake by setting the new compiler version, e.g.

      .. code-block:: bash

         cmake -D CMAKE_CXX_COMPILER=g++13 ..

   .. tip::
      If at any point you get CMake cache error messages, delete the CMakeCache file in the build directory and run cmake again.

#. Build all CMake targets.

   .. code-block:: bash

      cmake --build .

#. Finally, run the executable file ``gdsimsapp.exe`` that has been created in the build directory.

   .. code-block:: bash

      ./gdsimsapp

.. caution::
    If you later make changes to the source code (such as to fully randomise results), repeat steps 5 and 6 to re-build and re-compile the program afterwards. 