
File structure and architecture
===================================

The program and GitHub site are structured and organised for flexible and easy use. The key components you may interact with are:

- :class:`Simulation`: the user entry-point to the program. This class sets parameters and constructs the :class:`Model` from them, controlling the simulation flow.
- :class:`Model`: the heart of the program. This class manages the interacting components of the model and defines the model steps. 
- :class:`Record`: controls the generation of and recording to output files.
- :file:`sets.h`: contains the default parameter sets.
- :class:`InputParams`: defines the parameters needed to construct :class:`Simulation`.
- :file:`Params.h`: defines the substructures for parameters, such as :struct:`InherParams` to use in :func:`Simulation::set_inheritance()`
- ``test``: directory in the GitHub site which contains test data and sample testing scripts, as well as a plotting/data visualisation library (in Python).

The program architecture has been designed such that the Simulation class will provide easy interaction for new users, and more advanced users may interact directly with the Model and Record classes, with the potential to extend these. The ``set_inheritance()`` method in Simulation may also be easily overloaded, as we expect this section of the program to be extended and interface with other programs in the future. 