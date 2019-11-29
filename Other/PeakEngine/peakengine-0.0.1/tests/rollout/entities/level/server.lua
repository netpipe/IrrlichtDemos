
entity = nil;
script = nil;

body = nil;

floortiles = {};
floortilecount = 0;

function loadLevel(name)
	-- Create floor plane
	floor = peak.Cube:new();
	floor:init(peak.Game:get():getWorld(), 10000, 0.1, 10000);
	floor:setPosition(peak.Vector3D(0, -5, 0));
	floor:getCollisionInfo().callback1.script = script;
	floor:getCollisionInfo().callback1.functionname = "fallingcollision";
	floortiles[0] = floor;
	floortilecount = floortilecount + 1;
	
	-- Create start plane
	start = peak.Cube:new();
	start:init(peak.Game:get():getWorld(), 100, 1000, 0.1);
	start:getCollisionInfo().callback1.script = script;
	start:getCollisionInfo().callback1.functionname = "startcollision";
	startset = 0;
	-- Create finish plane
	finish = peak.Cube:new();
	finish:init(peak.Game:get():getWorld(), 100, 1000, 0.1);
	finish:getCollisionInfo().callback1.script = script;
	finish:getCollisionInfo().callback1.functionname = "finishcollision";
	finishset = 0;
	
	-- Load level
	file = peak.FileSystem:get():openFile("levels/"..name, peak.EFM_Read, false);
	line = file:readLine();
	z = 0
	while line ~= "" do
		print("Line: "..line);
		local width = line:len();
		for i = 1,width do
			c = string.char(line:byte(i));
			floor = nil;
			if c == "S" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 1;
			end
			if c == "=" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 0;
				if finishset == 0 then
					finish:setPosition(peak.Vector3D(0, 0, z));
					finishset = 1;
				end
			end
			if c == "-" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 0;
				if startset == 0 then
					start:setPosition(peak.Vector3D(0, 0, z));
					startset = 1;
				end
			end
			if c == "O" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 1;
			end
			if c == "J" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 1;
				floor:getCollisionInfo().callback2.script = script;
				floor:getCollisionInfo().callback2.functionname = "jumpcollision";
			end
			if c == "s" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 0;
				floor:getCollisionInfo().callback2.script = script;
				floor:getCollisionInfo().callback2.functionname = "speedcollision";
			end
			if c == "F" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 0.1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:getCollisionInfo().friction = 1;
			end
			if c == "C" then
				floor = peak.Cube:new();
				floor:init(peak.Game:get():getWorld(), 2, 1, 2);
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0.5, z));
				floor:getCollisionInfo().friction = 1;
			end
			if floor ~= nil then
				floortiles[floortilecount] = floor;
				floortilecount = floortilecount + 1;
			end
		end
		line = file:readLine();
		z = z + 2
	end
	file:delete();
end

function create(newentity, component)
	-- Get entity info
	entity = newentity;
	script = component:getScript();
	
	entity:getVariables():set("levelname", "level1");
	loadLevel("level1");
end

function destroy()
	for i=0,floortilecount - 1 do
		floortiles[i]:delete();
	end
end

function update(msecs)
end

function jumpcollision(collisioninfo)
	body = collisioninfo.geom1:getBody();
	if body == nil then
		body = collisioninfo.geom2:getBody();
	end
	if body ~= nil then
		otherentity = body:getEntity();
		if otherentity ~= nil then
			if otherentity:getName() == "ball" then
				vel = body:getLinearVelocity();
				vel.y = 7;
				body:setLinearVelocity(vel);
			end
		end
	end
end

function speedcollision(collisioninfo)
	body = collisioninfo.geom1:getBody();
	if body == nil then
		body = collisioninfo.geom2:getBody();
	end
	if body ~= nil then
		otherentity = body:getEntity();
		if otherentity ~= nil then
			if otherentity:getName() == "ball" then
				body:addForce(peak.Vector3D(0, 0, 90));
			end
		end
	end
end

function fallingcollision(collisioninfo)
	body = collisioninfo.geom1:getBody();
	if body == nil then
		body = collisioninfo.geom2:getBody();
	end
	if body ~= nil then
		otherentity = body:getEntity();
		if otherentity ~= nil then
			if otherentity:getName() == "ball" then
				-- Respawn player at older location
				-- Kill player if there aren't any lives left.
			end
		end
	end
	
	return 1;
end

function startcollision(collisioninfo)
	body = collisioninfo.geom1:getBody();
	if body == nil then
		body = collisioninfo.geom2:getBody();
	end
	if body ~= nil then
		otherentity = body:getEntity();
		if otherentity ~= nil then
			if otherentity:getName() == "ball" then
				-- Start player timer
				script = otherentity:getServerScriptComponent():getScript();
				script:callFunction("starttimer");
			end
		end
	end
	
	return 0;
end

function finishcollision(collisioninfo)
	body = collisioninfo.geom1:getBody();
	if body == nil then
		body = collisioninfo.geom2:getBody();
	end
	if body ~= nil then
		otherentity = body:getEntity();
		if otherentity ~= nil then
			if otherentity:getName() == "ball" then
				-- End player timer
				script = otherentity:getServerScriptComponent():getScript();
				script:callFunction("stoptimer");
				script:callFunctionI("set_input", 0);
			end
		end
	end
	
	return 0;
end

