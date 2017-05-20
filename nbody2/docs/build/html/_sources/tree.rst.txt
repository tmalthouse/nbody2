.. default-domain:: c

********
TreeNode
********

`tree.c <https://github.com/tmalthouse/nbody2/blob/master/nbody2/tree.c>`_
`tree.h <https://github.com/tmalthouse/nbody2/blob/master/nbody2/tree.h>`_

.. type:: TreeNode

A node in the tree structure. Every part of the tree---root, branches, and all---consists of TreeNodes

.. member:: TreeNode* TreeNode.nodes

An array of child nodes, which may or may not exist.

.. member:: uint TreeNode.level

The depth of the node. The root is at level 0.

.. member:: vec3 TreeNode.divs

The dividing lines of the node, which bisect it along each axis. Each element of this can be considered a plate orthogonal to that axis. For example, if ``TreeNode.divs.x = 5``, there is a plane, parallel to the y and z axes, located 5 units away from the origin. In the node shown below, the planes form the divs.

.. image:: lev_0_div.jpg

.. member:: vec3 TreeNode.max

This is the point furthest from the origin of the node.

.. member:: vec3 TreeNode.min

This is the closest point to the origin in the node.

.. member:: Body** TreeNode.bodies

This is an array of pointers to the bodies contained within the node.

.. member:: uint TreeNode.nbodies

This is the number of bodies in the previously-mentioned array.

.. member:: double TreeNode.mass

This is the total mass of the node

.. member:: vec3 TreeNode.ctr_mass

This is the position of the center of mass of the node

.. member:: uint TreeNode.capacity

This is the capacity of the :data:`TreeNode.bodies` array.

.. member:: bool TreeNode.initialized

This flag describes whether a given node has been initialized (has child nodes.) An uninitialized node is usually empty.

.. function:: void sum_mass(TreeNode *node)

This function sums up all the masses in a given node, and places the result at node->mass

.. function:: bool should_open_node(TreeNode *node, vec3 pos)

This function uses an algorithm to determine whether a node is distant enough to be considered as a single unit, or needs to be split open.

.. function:: TreeNode build_tree(Body *bodies, uint count)

This function builds a tree for a given array of bodies. This function may take a while, but only needs to be called once.

.. function:: void update_tree(TreeNode *node)

This function goes through the bodies in a given tree, reassigning them to new nodes if they have moved.

.. void free_node(TreeNode *node);

This function deletes a tree structure, traversing all the branches and freeing all allocated structures.

Private Functions
#################

These functions are only accessible from the `tree.c`_ file, and are documented here for reference.

.. function:: uint8_t coord_to_index(vec3 coord, vec3 divs)

This function maps a coordinate to an index for :data:`TreeNode.nodes`, given a position and the TreeNode.divs planes.

.. function:: vec3 max_point(Body *bodies, uint count)

Given an array of bodies, this function finds the most single dimension. This is used to build the initial tree.

.. function:: vec3 node_min_point(vec3 min, vec3 div, uint8_t index)

This function returns the minimum point for a subnode at a given index, where its parent node has the given minima and divs. Essentially, this function is the inverse of :func:`coord_to_index` above.

.. function:: vec3 center_of_mass(TreeNode *node)

This function calculates the center of mass of a node.

.. update_node(TreeNode *node)

This function powers the :func:`update_tree` function from above. It is called recursively to rebuild the tree at each timestep.
