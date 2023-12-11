local NodeID = getNode() - 999;							// get current node id
local posXY  = position(NodeID);						// get relative position in grid of sydney's
//local Ticks  = (si.CurrentTick - si.LastTick) / 5;	// time passed since last call, not really using here anymore
local Ticks  = 15.0;									// constant used
local campos = si.getCameraPosition();					// get postion of the camera
local mypos  = si.getPosition();						// get node position

local Target = si.SydVec - campos;						// target is in between camera and first Sydney,
														// bit of vector math here

Target.normalize();
Target = Target * vector3(50 + posXY.Y, 50 + posXY.Y, 50 + posXY.Y);
Target = Target + campos;

local Offset = vector3(si.SydVec.Z - campos.Z, 0, campos.X - si.SydVec.X);
Offset.normalize();
Offset = Offset * vector3(posXY.X, posXY.X, posXY.X);

Target = Target + Offset;
Target.Y = -10;

local step   = Target - mypos;							// calculate step
local L      = step.getLength();						// distance to travel

if (L <= Ticks)											// within reach of destination ?
{
	si.setPosition( Target );							// stop moving
	if (L > 0) stand();
}
else
{
	local Rot = step.getHorizontalAngle();				// face the camera
	Rot.Y -= 90;
	Rot.Z = 0;
	si.setRotation(Rot);

	step.normalize();									// calculate next step
	step.X = step.X * Ticks;
	step.Y = step.Y * Ticks;
	step.Z = step.Z * Ticks;
	local newpos = mypos + step;
	si.setPosition( newpos );
	walk();
}
