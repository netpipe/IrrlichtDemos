-- Simple follower script
-- By Christian Clavet, for the IRB project
-- Demonstrate the languages capabilities, and the dialogs

-- Set the defaults
local following=false
local tx_follow=""
local tx_notfollow=""

function setProperties()
-- Using the new properties attributes, can set the NPC combat attributes
	setProperty("life",50) -- current life of the NPC
	setProperty("maxlife",50) -- current Maximum life of this NPC
	setProperty("experience",10) -- given experience to attacker if killed
	setProperty("mindamage",2) -- minimum damage inflicted
	setProperty("maxdamage",5) -- maximum damage inflicted
	setProperty("hurtresist",50) -- resistance to attacks in %. If obtained, trigger the "hurt" animation then return to idle, breaking the current attack
end

function setStrings()
-- Using the "getLanguage" command, we can get international strings in our game!
-- text by default is english 
  tx_notfollow = "Do you want that I stop following you?"
  tx_follow = "Do you want that I follow you?"

-- Support for Portugese  
  if (getLanguage()=='pt-br') then
   tx_notfollow = "Voce quer que eu pare de seguir voce?"
   tx_follow = "Voce quer que eu te sigo?"
  end
  
 -- Support for French 
  if (getLanguage()=='fr-ca') then
   tx_notfollow = "Voulez-vous que j'arrete de vous suivre?"
   tx_follow = "Me permettez-vous de vous suivre?"
  end

 -- Support for German
  if (getLanguage()=='de-ge') then
   tx_notfollow = "Willst du, dass ich aufhoren folgenden du?"
   tx_follow = "Willst du, daB ich Sie folgen?"
  end
end


function onClicked()

-- Will ask the player if it want it to follow him
-- There a is following "mode" that decide the type of question to ask
  playSound("peasant/peasant_hello.ogg",false)
  if (following) then
    showDialogQuestion(tx_notfollow)
   else 
    showDialogQuestion(tx_follow)
 end

function onAnswer()
-- Here if we get the answer, decide what to do.
  answer = getAnswer()
  
  if (answer and following) then
    following=false
	return
  end
  if (answer and not following) then
    following=true
	return
  end
end

function onUpdate()
  if (following) then
	name = getName()..": "..getProperty("life").."/"..getProperty("maxlife")
	local x,y,z = getObjectPosition("player")
	if (getPropertie("life") == 0) then setEnabled(false) end
	  chaseObject("player",0.8,160,400)
	  if(distanceFrom(x,y,z) < 144) then 
		setObjectLabel(name)
		showObjectLabel()
	  end
    else
      hideObjectLabel()
    end
  end
end

function onLoad()
	setProperties()
	setStrings()
	setName("Follower")
 end