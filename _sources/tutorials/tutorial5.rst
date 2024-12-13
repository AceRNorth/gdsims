
5. Customising the gene drive release schedule
==============================================

.. _tutorial-5.1:

5.1 Setting multiple release times
----------------------------------

By default, the program uses a single gene drive release time at ``driver_start`` (we can see this in the previous runs). We can also choose to use multiple release times through the advanced options menu. This can be useful when we have a specific release schedule we want to test. 

We can create a text file named ``params_set17.txt``, which contains the same parameters as set 1, which we used in Tutorial :ref:`tutorial-1`, except we change the last parameter in the file, ``set_label``, to 17. To speed this up, we can use the one in the ``docs/exercises`` directory directly.

.. collapse:: Parameters

    .. code-block:: 
        :caption: docs/exercises/params_set17.txt

        2
        1000
        50
        0.05
        0.125
        100
        9
        0.06666666666666667
        10
        0.025
        0.2
        0.95
        200
        10000
        5
        0.01
        0.2
        0
        0
        0
        0
        0
        0
        100000
        0
        0
        0
        0
        0
        1000
        1
        200
        1
        17

Run the program with a custom run, entering this filepath, and open the advanced options menu.
Now, we can choose option ``5 - Multiple gene drive release times``.

.. image:: ../images/tut5_adv_options.png

This will prompt us to enter another filename or filepath for the release times. This text file should contain all chosen release times (as days relative to the simulation) as below:

.. collapse:: Release times

    .. code-block:: 
        :caption: docs/exercises/rel_times.txt

        200
        300
        400

The drive mosquitoes will be released at all selected release sites at each release time. The method of release site selection will vary depending on the method of release chosen - more details in the :doc:`../user_guide/user_guide_root`. For this run, new release sites will be randomly chosen upon each release time.

.. caution:: 

    As before, successfully setting a custom option means overwriting the default parameter - in this case the single release time at ``driver_start``.

Once we create this file and enter the filepath, all that's left to do is run the program!

.. image:: ../images/tut5_confirm.png

And with this, we have effectively run the pre-defined set 17! 
We can look at the totals plot and see for ourselves the spikes at each release time:

.. image:: ../images/tut5_totals_plot.png
    :scale: 80 %
