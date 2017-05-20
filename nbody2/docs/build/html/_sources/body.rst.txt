.. default-domain:: c

****
Body
****

`body.c <https://github.com/tmalthouse/nbody2/blob/master/nbody2/body.c>`_
`body.h <https://github.com/tmalthouse/nbody2/blob/master/nbody2/body.h>`_

.. type:: Body

A ``Body`` object represents a celestial body (or other abstract celestial object.) It contains state information, body type, and other pertinent data.

.. member:: uint32_t Body.id

This is a unique 32-bit integer, used to check whether two bodies are the same. Duplicate IDs will lead to inaccurate calculations. By default, IDs are based on the body's position in the main Body array.

.. member:: vec3 Body.pos

This holds the position information

.. member:: vec3 Body.vel

This holds the velocity information

.. member:: BodyType Body.type

This holds information about the class of the body (gas, dust, DM, etc.) Not used at the moment, it will eventually be used for hydrodynamic calculation

.. member:: double Body.mass

This field ONLY exists if the macro UNIT_MASS is undefined. Its purpose should be self-explanatory.

.. member:: vec3 acc

This field holds the acceleration at the last timestep. It shouldn't be useful for anything other than calculating the new timestep.

.. member:: uint64_t tstep

This is the current timestep, in seconds. It will always be a power of 2.

.. function:: uint64_t update_timestep (Body *b, uint64_t cur_time)

This function updates the given body's timestep, using the last step's acceleration as a guide. The algorithm used comes from Noah Muldavin's thesis (2013).

.. function:: void update_body (Body *b, TreeNode *tree)

This function updates the given body by one timestep. The rest of the system is contained in the ``tree`` parameter, which makes the calculations much faster than going through the bodies individually.

Private Functions and Types
###########################

The following entries are only visible from the body.c_ file.
They are just documented here for reference.

.. type:: NodeList

This type represents a simple dynamic array of TreeNode*s (similar to C++'s vector<T>.)

.. function:: void NodeList_append(NodeList *l, TreeNode *n)

This function adds element ``n`` to the end of NodeList ``l`` (allocating more memory if necessary.)

.. function:: uint64_t t_ideal(Body *b)

This function calculates the ideal timestep for a given body, using an algorithm from Muldavin 2013.

.. function:: node_finder(NodeList *l, vec3 pos, TreeNode *tree)

This function walks through the given tree, adding any node that needs to be accounted for to NodeList ``l``.

.. function:: body_acc(TreeNode **nodes, size_t node_count, Body *b)

This function calculates the acceleration on Body ``b``, using the list of nodes found by ``node_finder``.

.. function:: vec3 g_acc(vec3 pos1, vec3 pos2, double m2)

This function calculates the acceleration at ``pos1`` caused by an object at ``pos2`` with mass ``m2``, using the standard equation

:math:`r = pos2-pos1`

:math:`f_{acc} = \frac{G m_2}{(|r|)^2}\hat{r}`

.. function:: vec3 body_acc(TreeNode **nodes, size_t node_count, Body *b)

This function calculates the total acceleration on body ``b``, calling ``g_acc`` on each of the provided nodes and adding all the results together.
