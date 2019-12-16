
function menu_action(name)
	print("Menu action: "..name);
	if name == "startgame" then
		-- Start game
		if not peak.Game:get():init(true) then
			peak.Game:get():init(peak.Address("89.53.222.140"));
		end
	end
end

function onMouseMoved(x, y)
	print("Mouse moved.");
end

function onMouseButton(x, y, button, state)
	print("Mouse button!");
end

