local a = 0

function setProperties()
-- Using the new properties attributes, can set the NPC combat attributes. 
	setEnemy() -- this is an ennemy
	setProperty("life",10) -- current life of the NPC
	setProperty("maxlife",10) -- current Maximum life of this NPC
	setProperty("experience",5) -- given experience to attacker if killed
	setProperty("mindamage",2) -- minimum damage inflicted
	setProperty("maxdamage",4) -- maximum damage inflicted
	setProperty("hurtresist",50) -- resistance to attacks in %. If obtained, trigger the "hurt" animation then return to idle, breaking the current attack
	setProperty("hit_prob",60)
	setProperty("dodge_prob",12)
end

-- "onClicked" will trigger is the dynamic object is being clicked on by the user
function onClicked()
	print("Lua character is being clicked on!")
end

-- new game function
-- "onCollision" will trigger if the dynamic object got a collision with something
function onCollision()
end

-- "step" will trigger at each time interval (around 1/4 second)
function onUpdate()
  name = getName()..": "..getProperty("life").."/"..getProperty("maxlife")
  if (getPropertie("life") == 0) then 
	hideObjectLabel()
	return
  end
  local x,y,z = getObjectPosition("player")
   if(distanceFrom(x,y,z) < 288 and getPlayerLife()>0) then 
    if(distanceFrom(x,y,z) < 75) then
	  --print("Frog now attack!")
	  setAnimation("attack")
	else
      chaseObject("player",0.8,75,800)
    end
	setObjectLabel(name)
    showObjectLabel()
  else
    hideObjectLabel()
    walkRandomly()
  end
end

-- "onLoad" will trigger when the dynamic object is initialized (only once)
function onLoad()
	setProperties()
	blocked = false
end
