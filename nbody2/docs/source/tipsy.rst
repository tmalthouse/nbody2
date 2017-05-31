.. default-domain:: c

***********
Tipsy Files
***********

`tipsy.c <https://github.com/tmalthouse/nbody2/blob/master/nbody2/tipsy.c>`_
`tipsy.h <https://github.com/tmalthouse/nbody2/blob/master/nbody2/tipsy.h>`_

These files provide the ability to read Tipsy files into a System object.

.. function:: System load_tispy(char *file)

This function takes a path to a Tipsy file, and returns a System object that represents it. If the file is invalid, the resulting System object will have a ``NULL`` pointer instead of a body array.

Private Functions and Types
###########################

Documentation for the following types available `here <https://github.com/N-BodyShop/changa/wiki/File-Formats>`_.

.. type:: struct tipsy_head

This represents the binary header of a Tipsy file.

.. type:: struct tipsy_gas

This represents a gas particle in a Tipsy file.

.. type:: struct tipsy_dust

This represents a dust particle in a Tipsy file.

.. type:: struct tipsy_dark

This represents a dark matter particle in a Tipsy file.

.. type:: byte

A byte type. Just an alias for ``uint8_t``. Defined by

``typedef uint8_t byte``

.. function:: vec3 float3_to_vec3 (float *float3)

This function converts a vector from the form used in Tipsy files to the form used in the program.
