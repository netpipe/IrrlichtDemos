local a = 0

function setProperties()
-- Using the new properties attributes, can set the NPC combat attributes. 
	setEnemy() -- this is an ennemy
	setProperty("life",25) -- current life of the NPC
	setProperty("maxlife",25) -- current Maximum life of this NPC
	setProperty("experience",10) -- given experience to attacker if killed
	setProperty("mindamage",1) -- minimum damage inflicted when hit
	setProperty("maxdamage",3) -- maximum damage inflicted when hit
	setProperty("hurtresist",50) -- resistance to attacks in %. If obtained, trigger the "hurt" animation then return to idle, breaking the current attack
	setProperty("hit_prob",75) -- Hit probability, if too low it will miss it's target, too high, hit him almost each time
	setProperty("dodge_prob",15) -- Dodge probability, will lower the hit probability of the attacker
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
  local x,y,z = getObjectPosition("player")
  name = getName()..": "..getProperty("life").."/"..getProperty("maxlife")
  -- The NPC has been killed. Hide the label and do nothing.
  if (getProperty("life")==0) then
    hideObjectLabel()
	return
  end
  -- The character is alive and is tracking the player
  if(distanceFrom(x,y,z) < 288 and getPlayerLife()>0) then 
    setObjectLabel(name)
    showObjectLabel()
    if(distanceFrom(x,y,z) < 90) then
		setAnimation("attack")
		lookAt(x,y,z)
      else
        chaseObject("player",2,90,800)
      end
  else
    -- Lost the player or killed the player. Goes around
    hideObjectLabel()
    walkRandomly()
  end
end

-- "onLoad" will trigger when the dynamic object is initialized (only once)
function onLoad()
	setProperties()
	blocked = false
end
