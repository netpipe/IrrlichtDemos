
function menu_action(name)
	print("Menu action: "..name);
	if name == "startgame" then
		-- Start game
		if not peak.Game:get():init(true) then
			peak.Game:get():init(peak.Address("127.0.0.1"));
		end
	end
end
