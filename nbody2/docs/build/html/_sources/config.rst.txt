.. default-domain:: c

*********************
Configuration Options
*********************

`config.h <https://github.com/tmalthouse/nbody2/blob/master/nbody2/config.h>`_

.. macro:: UNIT_MASS

If this is defined, every body has a mass of one, and :data:`Body.mass` does not exist. This makes calculations faster.

.. macro:: BIG_G

This represents the gravitational constant. If :macro:`UNIT_MASS` is defined it has a value of 1. Otherwise, it defaults to the SI value of :math:`6.67\times10^{-11}`.

.. macro:: EPSILON

This is the *softening parameter*, which reduces statistical anomalies.

.. macro:: T_MAX

This is the maximum timestep. It defaults to 1000 years, although such a large timestep is rare.

.. macro:: TOL_PARAM

This is used to determine a body's ideal timestep.

.. macro:: NODE_OPEN_PARAM

This is used to decide whether to open a node up. A higher value is faster, but less accurate.

.. macro:: INITIAL_NODE_BUFFER

This sets the number of nodes to allocate initially for the node list buffer.
