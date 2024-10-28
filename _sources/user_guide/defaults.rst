
Default behaviours
==================

This page will list the default behaviours of the program before setting advanced options and some of the key model assumptions. Descriptions of the input parameters can be found in the :class:`InputParams` documentation.

Gene drive inheritance
----------------------

- 6 genotypes (in this order) for every mosquito type: WW, WD, DD, DR, WR and RR composed of a wild-type allele (W), a drive-type allele (D) and a resistant-type allele (R).
- Non-functional resistance

.. seealso::
    Tutorial :ref:`tutorial-3.2`

    Model introduction on gene drive

    :func:`Simulation::set_inheritance`

    Method documentation

Gene drive release
------------------

- Only drive-heterozygous (WD) male mosquitoes will be released.
- Mosquitoes will be released at *every* selected release site for each release time.
- By default, there will only be one release time at ``driver_start``.

.. seealso::
    Tutorial :ref:`tutorial-3.2`

    Model introduction on gene drive

    :class:`RandomGDRelease`

    Class documentation

Patch coordinates
-----------------

- Continuous and randomly generated for *x* and *y* in the range [0, ``side``]. 


Dispersal
---------

- Distance-kernel dispersal
    - No dispersal mortality
    - All neighbouring patches within ``max_disp`` distance are connected to the focal patch
    - Mosquitoes disperse to every connected patch

.. seealso::
    Tutorial :doc:`../tutorials/tutorial6`

    Model introduction on dispersal

    :class:`DistanceKernelDispersal`

    Class documentation

Boundary type
-------------

- Toroid 

Toroidal (or otherwise called periodic) boundary conditions are applied to all edges of the square model area by default.

.. seealso::
    Tutorial :doc:`../tutorials/tutorial4`

    Model introduction on boundary types

    :class:`ToroidalBoundaryStrategy`
    
    Class documentation

Seasonality
-----------

- If ``alpha1`` is zero, no rainfall contribution will be applied, i.e. no seasonality.
- If ``alpha1`` and ``amp`` are non-zero, default rainfall contribution to the carrying capacity will be applied, modelled as a sine wave (equations in Tutorial :ref:`tutorial-7.1`).

.. seealso::
    Tutorial :ref:`tutorial-7.1`

    Model introduction on seasonality

    :class:`SineRainfall`

    Class documentation

Aestivation
-----------

- Only applied if ``psi`` is non-zero.

.. seealso::
    :class:`Aestivation`

    Class documentation