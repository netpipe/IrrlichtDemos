#pragma once

#include "ExampleFramework.hpp"
#include <irrString.h>
#include <irrTypes.h>
#include <irrBullet.h>

class IRigidBody;

class CVehicleExample : public CExampleFramework {

 public:

  CVehicleExample();
  virtual ~CVehicleExample();

  virtual void runExample();

  virtual bool OnEvent(const irr::SEvent& event);

  void createTerrain();
  void createBoxes();
  void createVehicle();
  void createGround();

 private:
  IRigidBody *tank;
  IRaycastVehicle *vehicle;
  irr::u32 rows;
  irr::u32 columns;
};
