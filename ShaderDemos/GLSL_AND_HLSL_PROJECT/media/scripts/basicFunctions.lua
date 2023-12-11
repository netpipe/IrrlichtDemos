--================================================
--read colors name (this allows to call setAmbientLight(COLOR_NAME) instead setAmbientLight(r,g,b) for example)
dofile("../media/scripts/colors.lua")

--================================================
dofile("../media/scripts/rain.lua")

--================================================IRRRPG BUILER INTERNAL USE ONLY

--store original global params (ambient light, fog, ..) before start gameplay
function IRBStoreGlobalParams()
  IRBOriginalAmbientLightR,IRBOriginalAmbientLightG,IRBOriginalAmbientLightB = getAmbientLight()
  IRBOriginalFogR,IRBOriginalFogG,IRBOriginalFogB = getFogColor()
  IRBOriginalFogStart,IRBOriginalFogEnd = getFogRange()
  IRBOriginalCameraPosX,IRBOriginalCameraPosY,IRBOriginalCameraPosZ = getCameraTarget()
end
--restore original global params after gameplay (used when you click Stop in Editor)
function IRBRestoreGlobalParams()
  setAmbientLight(IRBOriginalAmbientLightR,IRBOriginalAmbientLightG,IRBOriginalAmbientLightB)
  setFogColor(IRBOriginalFogR,IRBOriginalFogG,IRBOriginalFogB)
  setFogRange(IRBOriginalFogStart,IRBOriginalFogEnd)
  setCameraTarget(IRBOriginalCameraPosX,IRBOriginalCameraPosY,IRBOriginalCameraPosZ)
  setRain(0)
end


--================================================DYNAMIC OBJECT FUNCTIONS

--set a global name to the objet
function setObjectName(name)
  setGlobal(name,objName)
end


-- name -> object to chase
-- speed -> walk speed
-- near -> nearest distance to chasse, no chase if distanceFrom is less than near value
-- far -> farest distance to chasse, no chase if distanceFrom is more than near value
function chaseObject(name, speed, near, far)
  local x,y,z = getObjectPosition(name)
  if(speed == nil) then speed = 0.8 end
  y = 0;
  --lookToObject(name)
  -- Alway look at it target even if it's not moving.
  lookAt(x,y,z)
    
  if(far == nil) then far = 600 end
  if(near == nil) then near = 36 end
  
  if( (distanceFrom(x,y,z) < far) and (distanceFrom(x,y,z) > near) ) then
  	if (hasReached()) then 
  		walkTo(x,y,z);
		--move(speed)
		animation = "walk"
	end
  end
  
  if ((distanceFrom(x,y,z) < near) and animation=="walk") then
	setAnimation("idle")
	animation = "idle"
  end

end

--Creates an randomly walk movement
--@param radius defines the limits of the walkable area. default 1
--@param speed defines the walk speed. default 0.01
function walkRandomly(radius, speed)
  if( randomPointX == nil or randomPointZ == nil ) then
	IRBOriginalPositionX,IRBOriginalPositionY,IRBOriginalPositionZ = getPosition()
	IRBOriginalRotationX,IRBOriginalRotationY,IRBOriginalRotationZ = getRotation()
    randomPointX = IRBOriginalPositionX
    randomPointZ = IRBOriginalPositionZ
  end
  
  if(radius == nil) then radius = 250 end
  if(speed == nil) then speed = 0.8 end

  if(distanceFrom(randomPointX,0,randomPointZ) < 80) then
    randomPointX = IRBOriginalPositionX + math.random(-radius,radius)
    randomPointZ = IRBOriginalPositionZ + math.random(-radius,radius)
  end
  
  if(distanceFrom(IRBOriginalPositionX,IRBOriginalPositionY,IRBOriginalPositionZ) > radius) then
    randomPointX = IRBOriginalPositionX
    randomPointZ = IRBOriginalPositionZ
  end
  if (hasReached()) then
    res = math.random(0,10)
	if (res>1) then 
	    walkidle=true
		setAnimation('idle')
	else
	    walkidle=false
		walkTo(randomPointX,0,randomPointZ)
	    randomPointX = IRBOriginalPositionX + math.random(-radius,radius)
        randomPointZ = IRBOriginalPositionZ + math.random(-radius,radius)
	end	
  end
end

--with this function you can schedule an function call, for example:
--if you want to schedule a print('hello world') call with a 10 seconds delay you have to do this:
--programAction(10, print, 'hello world')
--this way the print('hello world') will be called after 10 seconds counting by now
function programAction(time, action, ...)
  IRBProgrammedAction = action
  IRBProgrammedActionParams = {...}
  IRBProgrammedActionTime = os.clock() + time
end

--update programmed action if it exist
function CustomDynamicObjectUpdateProgrammedAction()
  if(IRBProgrammedAction ~= nil) then
    if(os.clock() >= IRBProgrammedActionTime) then
      --call the function with max of 10 params
      IRBProgrammedAction(IRBProgrammedActionParams[1],
			  IRBProgrammedActionParams[2],
			  IRBProgrammedActionParams[3],
			  IRBProgrammedActionParams[4],
			  IRBProgrammedActionParams[5],
			  IRBProgrammedActionParams[6],
			  IRBProgrammedActionParams[7],
			  IRBProgrammedActionParams[8],
			  IRBProgrammedActionParams[9],
			  IRBProgrammedActionParams[10])
      IRBProgrammedAction = nil
      IRBProgrammedActionParams = nil
      IRBProgrammedActionTime = nil
    end
  end
end

function hasActionProgrammed()
  return IRBProgrammedAction ~= nil
end

--walk to an object
function walkToObject(otherObj)
  DynamicObjectWalkTarget = otherObj
end

--this is a custom Update function called at every frame like step
--you can create custom functions that needs to be updated (like walkTo) and hide it 
--from the final user by putting the code here
--IMPORTANT: This funcion is not called if the object is disabled!
function CustomDynamicObjectUpdate()
  --update walkTo
  if(DynamicObjectWalkTarget ~= nil) then
    lookToObject(DynamicObjectWalkTarget)
    move(5.0)
    if(distanceFrom(DynamicObjectWalkTarget) < 60) then 
      DynamicObjectWalkTarget=nil 
      setFrameLoop(0,0)--set initial animation when done
    end
  end
  
  --update sound listener position (always on player)
  setSoundListenerPosition( getObjectPosition("player") )
end

function enableObject()
  setEnabled(true)
end

function disableObject()
  setEnabled(false)
end

--================================================PLAYER FUNCTIONS

function decreasePlayerLife(points)
  setPlayerLife(getPlayerLife() - points)
  if(getPlayerLife() < 0) then setPlayerLife(0) end
end

function increasePlayerLife(points)
  setPlayerLife(getPlayerLife() + points)
end

function decreasePlayerMoney(points)
  setPlayerMoney(getPlayerMoney() - points)
  if(getPlayerMoney() < 0) then setPlayerMoney(0) end
end

function increasePlayerMoney(points)
  setPlayerMoney(getPlayerMoney() + points)
end

--================================================SOUND FUNCTIONS
function playSound(soundName, looped)
  playSound2D(soundName,looped)
end

function emitSound(soundName, looped)
  x,y,z = getObjectPosition(objName)
  playSound3D(soundName,looped, x,y,z)
end


--================================================TIME FUNCTIONS
--sleep for a while
--@param sleepTime seconds to sleep
function sleep(sleepTime)
  targetTime = os.clock() + sleepTime
  while os.clock() < targetTime do end
end