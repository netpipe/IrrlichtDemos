
entity = nil;
ball = nil;
camera = nil;

timerstate = 0;

timer = peak.Timer();
timer:reset();

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
	if entity:isLocal() then
		panel = peak.TextureOverlay:new("panel.png");
		panel:setPosition(peak.Vector2D(0.7, 0.7));
		panel:setSize(peak.Vector2D(0.3, 0.3));
		timeroverlay = peak.LabelOverlay:new("Time: 0", peak.Color(0, 0, 0, 255),
			"haettenschweiler.png", panel);
		timeroverlay:setPosition(peak.Vector2D(0.1, 0.1));
		timeroverlay:setSize(peak.Vector2D(0.8, 0.1));
		labeloverlay = peak.LabelOverlay:new("Lives:", peak.Color(0, 0, 0, 255),
			"haettenschweiler.png", panel);
		labeloverlay:setPosition(peak.Vector2D(0.1, 0.3));
		labeloverlay:setSize(peak.Vector2D(0.8, 0.1));
		liveoverlay = {};
		for i=0,2 do
			liveoverlay[i] = peak.TextureOverlay:new("ball.png", panel);
			liveoverlay[i]:setPosition(peak.Vector2D(0.1 + i * 0.3, 0.5));
			liveoverlay[i]:setSize(peak.Vector2D(0.3, 0.3));
		end
	end
end

function destroy()
	if entity:isLocal() then
		panel:delete();
	end
end

function graphics_created(graphics)
	-- Create camera
	if entity:isLocal() then
		ball = graphics:getSceneNode(3);
		camera = peak.CameraSceneNode:new();
		camera:setPosition(peak.Vector3D(0, 1, -6));
	end
end

function update(msecs)
	if entity:isLocal() then
		newtimerstate = entity:getVariables():getInteger("timerenabled");
		if newtimerstate ~= timerstate then
			if newtimerstate == -1 then
				timerstate = newtimerstate;
				timer:setTime(-3);
				timer:start();
				print("Countdown.");
			end
			if newtimerstate == 0 then
				timer:stop();
				print("Timer: "..timerstate);
				if timerstate == -1 then
					timer:reset();
				else
					timer:setTime(entity:getVariables():getFloat("time"));
				end
				timerstate = newtimerstate;
				print("Stopping.");
			end
			if newtimerstate == 1 then
				timerstate = newtimerstate;
				timer:reset();
				timer:start();
				print("Starting.");
			end
		end
		if camera ~= nil then
			pos = ball:getPosition();
			pos.z = pos.z - 6;
			pos.y = pos.y + 1;
			camera:setPosition(pos);
		end
		
		timeroverlay:setLabel("Time: "..peak.toString(timer:getTime()));
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

