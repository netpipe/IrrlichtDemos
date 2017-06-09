Author: Thijs Ferket
License: Irrlicht engine license
Date: 30th March 2012
Irrlicht version: 1.7.2 & 1.8


A Progressive Mesh Buffer / Mesh Simplificator

With this class you can simplify existing meshes by contracting / collapsing edges.
This is very usefull for creating LOD meshes on the fly, instead of creating all meshes in your 3D editor.

Features:
- Contract / Collapse edges
- Texture support
- Bump/normal support
- Algorithms:
    * Random
    * Shortest
    * Melax
    * Quadric


Known bugs / TODO:
- After some edge contractions/collapses, not all edges are unique
- Optimize


Forum topic:
http://irrlicht.sourceforge.net/forum/viewtopic.php?f=6&t=46060