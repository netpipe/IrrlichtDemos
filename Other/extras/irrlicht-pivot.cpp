//http://irrlicht.sourceforge.net/forum/viewtopic.php?f=1&t=51771&p=301104&hilit=rotation+matrix#p301104

void CustomObject::render()
{
  video::IVideoDriver* driver = SceneManager->getVideoDriver();
 
  if(driver != nullptr)
  {
    driver->setMaterial(_Material);
 
    vector3df currentPosition;
    vector3df currentRotation;
 
    // Holds the pivot point as a vector
    vector3df pivotVector = getPivotPointVector(); // CustomObject::getPivotPointVector() returns a translation vector from the origin to the pivot point
 
    // Check if the pivot point vector is long enough to warrant all those computations.
    if(pivotVector.getLength() > 0.01f) // 0.01f might be too short to be noticable. Change to 0.5f, instead?
    {
      // Rotation/Translation computation matrix
      matrix4 rotationMatrix;
 
      // Offset the origin position by the pivot vector
      currentPosition = -(pivotVector);
 
      // Set the rotation matrix to the object's rotation value
      rotationMatrix.setRotationDegrees(getAbsoluteTransformation().getRotationDegrees());
 
      // rotate the position vector along the rotation matrix
      rotationMatrix.rotateVect(currentPosition);
 
      // Offset the absolute position by the currentPosition (which really holds a translation vector until then).
      currentPosition += getAbsolutePosition();
      currentPosition += pivotVector;
 
      // Get the rotation from the rotation matrix
      currentRotation = rotationMatrix.getRotationDegrees();
    }
    else
    {
      // Rotate on the center position
      currentRotation = getAbsoluteTransformation().getRotationDegrees();
 
      // Get the object world's position.
      currentPosition = getAbsolutePosition();
    }
 
    // Offset the position by half the width and height, so the position is consistent with other 2D engine.
    /* You may not need this code
    currentPosition.X += (getSize().Width / 2);
    currentPosition.Y -= (getSize().Height / 2);  // IV Cartesian quadrant
    */
 
    // Create the final world matrix
    core::matrix4 finalMatrix = matrix4().setTranslation(currentPosition) * matrix4().setRotationDegrees(currentRotation)  * matrix4().setScale(getAbsoluteTransformation().getScale());
 
    driver->setTransform(video::ETS_WORLD, finalMatrix);
 
    driver->drawIndexedTriangleList(_Vertices, 4, _Indices, 2);
 
    driver->setTransform(video::ETS_WORLD, irr::core::IdentityMatrix);
  }
}
