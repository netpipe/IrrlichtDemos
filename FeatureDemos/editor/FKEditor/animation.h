void changeAnimFrame(IAnimatedMeshSceneNode * node, int frame)
{
   //node->setAnimationSpeed(1);
   if (animated) node->setFrameLoop(frame,frame);  
   //node->setCurrentFrame(frame);
   //node->setFrameLoop(0,node->getEndFrame());
   //node->setAnimationSpeed(0);
}

core::stringw checkModel(scene::IAnimatedMesh* Mesh)
{
    animateType=Mesh->getMeshType();
    if (Mesh->getMeshType()==EAMT_MD2) return " Type: Quake 2\n"; 
    if (Mesh->getMeshType()==EAMT_MD3) return " Type: Quake 3\n";
    if (Mesh->getMeshType()==EAMT_SKINNED) return " Type: skinned mesh\n";
    return " no animation support";
}

s32 ModelPoly(scene::IAnimatedMesh* Mesh)
{
              return Device->getSceneManager()->getMeshManipulator()->getPolyCount(Mesh);
}
