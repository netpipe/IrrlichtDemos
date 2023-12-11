cmake_minimum_required(VERSION 2.6)
project( libirrExt )

set(
IRR_SOURCES
CJointMesh
CJointSceneNode
)


add_library( irrExt STATIC ${IRR_SOURCES} )

