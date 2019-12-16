
function menu_action(name)
	print("Menu action: "..name);
	if name == "startgame" then
		-- Start game
		if not peak.Game:get():init(true) then
			peak.Game:get():init(peak.Address("89.57.222.133"));
		end
	end
end
