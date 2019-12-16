

-- This function is called when the GAME initalizes and the executable is on the server pc. Be aware that you can be server and client at a time.
function create_server()
	-- Creates a plane, so the entities aren't going to fall down
	plane = peak.Plane:new();
	plane:create(peak.Vector3D(0, 1, 0));
	planebody = peak.Body:new();
	planebody:create(peak.Game:get():getWorld(), plane);

	-- First player
	player = peak.Game:get():createEntity("player", 0);
	player:setPosition(peak.Vector3D(0, 80, 0));
	
	--vehicle = peak.Game:get():createEntity("vehicle", 0);
	--vehicle:setPosition(peak.Vector3D(0, 80, -30));
	
	peak.GraphicsEngine:get():saveMessages();

end

-- This function is called when the GAME shutdowns. (Not the entire game, for example when you go back to the menu to chose another level or something)
function destroy_server()
	plane:destroy();
	plane:delete();
	planebody:destroy();
	planebody:delete();
end

-- This function will be called when an client connects. For example has to be a player for every client to be created.
function client_connected(id)
	-- Other player
	player = peak.Game:get():createEntity("player", id);
	player:setPosition(peak.Vector3D(0, 100, 0));
end

