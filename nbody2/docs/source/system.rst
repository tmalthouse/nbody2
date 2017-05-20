.. default-domain:: c

******
System
******

`system.c <https://github.com/tmalthouse/nbody2/blob/master/nbody2/system.c>`_
`system.h <https://github.com/tmalthouse/nbody2/blob/master/nbody2/system.h>`_

.. type:: System

A ``System`` object represents a collection of celestial bodies and their state at a given time.

.. member:: Body* System.bodies

An array of ``Body`` objects

.. member:: uint System.count

The number of bodies in System.bodies

.. member:: uint64_t time

The current time of the system, in the number of seconds since some epoch (the beginning of the simulation, perhaps?)

.. member:: TreeNode tree

The tree representing the system.

.. function:: void update_system(System *sys)

This function updates the given system by the smallest timestep possible.
