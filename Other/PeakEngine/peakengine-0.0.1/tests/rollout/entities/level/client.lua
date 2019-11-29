
entity = nil;

loaded = 0;

floortiles = {};
floortilecount = 0;

function loadLevel(name)
	file = peak.FileSystem:get():openFile("levels/"..name, peak.EFM_Read, false);
	line = file:readLine();
	z = 0
	while line ~= "" do
		width = line:len();
		for i = 1,width do
			c = string.char(line:byte(i));
			floor = nil;
			if c == "S" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 0.1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:setMaterial("checker");
			end
			if c == "=" or c == "-" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 0.1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:setMaterial("line");
			end
			if c == "O" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 0.1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:setMaterial("floor");
			end
			if c == "J" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 0.1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:setMaterial("jump");
			end
			if c == "s" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 0.1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:setMaterial("speed");
			end
			if c == "F" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 0.1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0, z));
				floor:setMaterial("checker");
			end
			if c == "C" then
				floor = peak.CubeSceneNode:new();
				floor:setScale(peak.Vector3D(2, 1, 2));
				floor:setPosition(peak.Vector3D(i * 2 - 1, 0.5, z));
				floor:setMaterial("wood");
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

function create(newentity)
	entity = newentity;
end
function destroy()
	for i=0,floortilecount - 1 do
		floortiles[i]:delete();
	end
end

function update(msecs)
	if loaded == 0 then
		if entity:getVariables():getString("levelname") ~= "" then
			loadLevel(entity:getVariables():getString("levelname"));
			loaded = 1;
		end
	end
end

