local a = 0

function setProperties()
-- Using the new properties attributes, can set the NPC combat attributes. 
	setEnemy() -- this is an ennemy
	setProperty("life",50) -- current life of the NPC
	setProperty("maxlife",50) -- current Maximum life of this NPC
	setProperty("experience",10) -- given experience to attacker if killed
	setProperty("mindamage",2) -- minimum damage inflicted
	setProperty("maxdamage",5) -- maximum damage inflicted
	setProperty("hurtresist",50) -- resistance to attacks in %. If obtained, trigger the "hurt" animation then return to idle, breaking the current attack
	setProperty("hit_prob",50)
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
  if (getProperty("life") == 0) then 
    setEnabled(false) 
	return
  end
  local x,y,z = getObjectPosition("player")
  if(distanceFrom(x,y,z) < 288) then 
    setObjectLabel(name)
    showObjectLabel()
    if(distanceFrom(x,y,z) < 100) then
	  setAnimation("attack")
	  lookAt(x,y,z)
	else
      chaseObject("player",0.8,100,800)
    end
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
