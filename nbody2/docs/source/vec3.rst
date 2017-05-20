.. default-domain:: c

****
vec3
****


.. type:: vec3

A 3-dimensional vector of doubles, powered by clang vector extensions. Individual elements are accessible with dot (a.x) or array (a[0]) notation.


.. macro:: vec3_0

A zero-vector for the vec3 type


.. macro:: vec3_I

A vec3 v where v.x=1, and v.y=0, v.z=0


.. macro:: vec3_J

A vec3 v where v.x=0, v.y=1, v.z=0

.. macro:: vec3_K

A vec3 v where v.x=0, v.y=0, v.z=1

.. function:: double vabs (vec3 v)

This function gives the absolute value of a given vector v

.. function:: bool vec3_eq (vec3 v, vec3 w)

This function checks two vectors for element equality. Note that because of floating point error, this function is mostly useful for checking if a vector is uninitialized (i.e. the 0 vector.)

.. function:: vec3 vec3_unit (vec3 v)

Given a vector, this function returns the unit vector (i.e. the vector pointing in the same direction with magnitude 1).
