
function menu_action(name, index)
	print("Menu action: "..name);
	if name == "choosemap" then
	end
	if name == "host" then
		-- Start game
		if not peak.Game:get():init(true) then
			-- Error dialog
			-- TODO
		else
			peak.Game:get():setVisible(true);
			peak.MenuManager:get():activateMenu("");
		end
	end
end
