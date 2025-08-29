.. _gui-root:

Graphical User Interface
========================

GDSiMS also offers a Graphical User Interface (GUI) hosted on the `GDSiMS GUI GitHub repository <https://github.com/AceRNorth/gdsims-gui>`_. This can be installed directly from the repo and is best suited for users with no programming experience. Visit the GUI :ref:`gui-install` page for more details. 

Features:
---------

- Best for beginners and non-programmers
- User-friendly interface
- 'In-house', interactive data visualisation features - selected plots and animations chosen to best illustrate and investigate the model.
- Short and easy installation process, though distribution is currently only available on previously tested systems (Windows 10, 11, ARM-based Macs).
- Options to load pre-defined parameter sets and/or tweak parameters
- Advanced parameter window dialog
- Tooltips on parameter labels provide instant descriptions when hovering over them
- Run the model program directly from the interface and check its progress
- Organised output data directories with descriptive parameter spreadsheet files

Should I use GDSiMS GUI or the model program (GDSiMS)?
------------------------------------------------------
The graphical user interface (GUI) is best suited to beginners to the model and non-programmers. The GUI offers 'in-house' selected data visualisation features which best illustrate the model behaviours. Plots and animations otherwise need to be created from scripts separate to the model program (we provide sample Python scripts).

The GUI is easy and quick to install, though only currently available on Windows 10 and 11 systems and ARM-based Macs. The model program uses cross-platform tools to ensure compatibility with all systems, which, though simplified and documented, makes it a more complex installation process. 

The model program is best suited to advanced users in need of quick custom parameter set changes. The model program also offers a command-line interface (CLI), which may be used by those not familiar with C++, and offers more pre-defined sets compared to the GUI. Directly interacting with the C++ code also provides flexible use cases thanks to its flexible architecture.

For large numbers of simulation repetitions (> 15-20) we advise to use the model program as visualising data for individual simulations will become impractical on the GUI. Summary features for large number of repetitions may be developed in future.

Both are freely available on their respective GitHub repositories, have an open-source licence and aim to have a documentation website with tutorials (tutorials for the GUI are in progress).

.. toctree::
    :maxdepth: 2

    install
    usage
    tutorials