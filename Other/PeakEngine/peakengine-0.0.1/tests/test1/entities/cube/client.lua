
entity = nil;

function create(newentity)
	entity = newentity;
	if entity:isLocal() and (peak.Game:get():getRegisteredEntity("jump") == nil) then
		peak.Game:get():registerEntityActionInput(newentity, "jump");
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
end

function destroy()
	if sound ~= nil then
		sound:delete();
	end
end

function graphics_created(graphics)
	-- Create camera
	ball = graphics:getSceneNode(3);
	sound = peak.SoundEngine:get():play2D("sounds/FromE.wav", false, true, false);
	if sound ~= nil then
		sound:attachToSceneNode(ball, true);
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
	if action == "jump" then
		entity:getVariables():setInteger("jump", state);
	end
end

