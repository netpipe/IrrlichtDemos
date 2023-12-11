-- Big potion script. Give 10HP when taken
-- By Christian Clavet

-- Called when the user click on the object or use the interaction button in proximity
function onClicked()
	-- When this object is clicked it will go inside the player backpack
	moveObjectLoot()
end

-- Called when the USE button is pressed for this item in the inventory GUI
function onUse()
	-- potion is used and give 100HP more HP
	increasePlayerLife(100)
	
	-- Ajust the life if the given potion is over the current maxlife
	if (getPlayerLife() > getObjectProperty("player","maxlife")) then
		setPlayerLife(getObjectProperty("player","maxlife"))
	end
	
end

-- Called when the game start
function onLoad()
	-- Set the display name and show it
	setObjectLabel("Big health potion")
	showObjectLabel()
	destroyAfterUse(true)
end