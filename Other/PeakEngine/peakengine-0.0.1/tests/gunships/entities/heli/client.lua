
function create(newentity)
	entity = newentity;
	
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("up") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "up");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("down") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "down");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("forward") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "forward");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("back") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "back");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("left") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "left");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("right") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "right");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("shoot1") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "shoot1");
	end
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("shoot2") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "shoot2");
	end
	if entity:isLocal() then
		peak.Game:get():registerEntityMouseInput(newentity);
	end
end

function destroy()
	sound:delete();
	rotorrotation:delete();
	auxrotorrotation:delete();
	if camera ~= nil then
		camcontroller:delete();
		camera:delete();
		hpoverlay:delete();
		teamoverlay:delete();
	end
end

function graphics_created(graphics)
	-- Create camera
	if entity:isLocal() then
		model = graphics:getSceneNode(3);
		camera = peak.CameraSceneNode:new();
		peak.SoundEngine:get():getListener():attachToSceneNode(camera);
		camcontroller = peak.FollowingController:new();
		camcontroller:setSceneNode(camera);
		camcontroller:setTarget(model);
		camcontroller:setDistance(peak.Vector3D(0, 0.8, -3));
		-- Create HUD
		hpoverlay = peak.LabelOverlay:new("Lives:", peak.Color(0, 0, 0, 255),
			"haettenschweiler.png", panel);
		hpoverlay:setPosition(peak.Vector2D(0.05, 0.05));
		hpoverlay:setSize(peak.Vector2D(0.4, 0.1));
		teamoverlay = peak.LabelOverlay:new("Lives:", peak.Color(0, 0, 0, 255),
			"haettenschweiler.png", panel);
		teamoverlay:setPosition(peak.Vector2D(0.05, 0.15));
		teamoverlay:setSize(peak.Vector2D(0.4, 0.1));
	end
	-- Rotation of the rotors
	rotor = graphics:getSceneNode(4);
	rotorrotation = peak.RotationController:new();
	rotorrotation:setSceneNode(rotor);
	rotorrotation:setRotationSpeed(peak.Vector3D(0, 20, 0));
	auxrotor = graphics:getSceneNode(5);
	auxrotorrotation = peak.RotationController:new();
	auxrotorrotation:setSceneNode(auxrotor);
	auxrotorrotation:setRotationSpeed(peak.Vector3D(20, 0, 0));
	-- Engine sound
	sound = peak.SoundEngine:get():play3D("sounds/engine.wav", false, true);
	sound:attachToSceneNode(graphics:getSceneNode(3));
end

function update(msecs)
	if entity:isLocal() then
		hp = entity:getVariables():getInteger("hp");
		hpoverlay:setLabel("HP: "..hp);
		team = entity:getVariables():getInteger("team");
		teamoverlay:setLabel("Team: "..team);
	end
end

function action_input(action, state)
	print("Entity: Action: "..action..", "..state);
	if action == "forward" then
		entity:getVariables():setInteger("forward", state);
	end
	if action == "back" then
		entity:getVariables():setInteger("back", state);
	end
	if action == "left" then
		entity:getVariables():setInteger("left", state);
	end
	if action == "right" then
		entity:getVariables():setInteger("right", state);
	end
	if action == "up" then
		entity:getVariables():setInteger("up", state);
	end
	if action == "down" then
		entity:getVariables():setInteger("down", state);
	end
	if action == "shoot1" then
		entity:getVariables():setInteger("shoot1", state);
	end
	if action == "shoot2" then
		entity:getVariables():setInteger("shoot2", state);
	end
end

function mouse_moved(x, y, z)
	entity:getVariables():set("mousemovement", peak.Vector2D(x, y));
end

