

-- This function is called when the GAME initalizes and the executable is on the server pc. Be aware that you can be server and client at a time.
function create_server()
	-- Creates a plane, so the entities aren't going to fall down
	plane = peak.Plane:new();
	plane:create(peak.Vector3D(0, 1, 0));
	planebody = peak.Body:new();
	planebody:create(peak.Game:get():getWorld(), plane);
	planebody:setRotation(peak.Quaternion(peak.Vector3D(0, -90.0 / 180.0 * math.pi, 0)));
	rotation = planebody:getRotation():getEuler();
	print("Rotation: "..rotation.x.."/"..rotation.y.."/"..rotation.z);


	-- This is a pavillion level mesh, it'll test concave trimeshes.
	-- levelmesh = peak.Game:get():createEntity("pavilion", 0);
	-- levelmesh:setPosition(peak.Vector3D(0, 0.2, 0));
	
	-- This is a terrain, which is NOT loaded from a heightmap -> it is loaded from a normal model file
	terrain = peak.Game:get():createEntity("terrain", 0);
	terrain:setPosition(peak.Vector3D(0, 0, 0));

	-- The main player, the player receives the whole input atm
	player = peak.Game:get():createEntity("player", 0);
	player:setPosition(peak.Vector3D(0, 10, 0));

	-- Loads an animated cube, to test animations
	knight = peak.Game:get():createEntity("knight", 0);
	
	-- Loads a few cubes for benchmarking
	for x=0,4 do
		for y=0,4 do
			--for z=0,4 do
				cube = peak.Game:get():createEntity("cube", 0);
				cube:setPosition(peak.Vector3D(x * 4, y * 4 + 2, -10));
			--end
		end
	end
		
	
	
	-- NO MORE EXISTING OBJECTS AND OTHER THINGS , WHICH AREN'T TESTED ANYMORE:
	-- Loads a truck (made by Sam_b)
	--truck = peak.Game:get():createEntity("truck", 0);
	--truck:setPosition(peak.Vector3D(0, 5, 50));
	
		-- Loads a sphere
	-- TODO: Add sphere support
    --for a=0,150 do	
        	--sphere = peak.Game:get():createEntity("sphere", 0);
       		--sphere:setPosition(peak.Vector3D((math.random() * 100) - (math.random() * 100), 50 + 300 * a / 3, (math.random() * 100) - (math.random() * 100)));
   	--end
   	
   	--camera = peak.GraphicsEngine:get():getStandardCamera();

	-- This would test links between two different objects, but that was replaced by a free cam, because that's more comfortable for testing.
	--link = peak.DirectLink:new();
	--link:create(cube:getProperty("position"), camera:getProperty("target"));
	--end
	
		-- Loads a few cubes, the cubes have manipulated physics -> they jump very high when they have contact to the ground
	--for y=0,15 do
		--cube = peak.Game:get():createEntity("flyingcube", 0);
		--cube:setPosition(peak.Vector3D(0, y * 3 + 200, 0));
	--end
	
	-- The following two entities were made to test convex trimeshes. Convex trimeshes aren't as precise as concave trimeshes.
	-- Loads the chair entity and places it on the top of the pavillion
	-- chair = peak.Game:get():createEntity("chair", 0);
	-- chair:setPosition(peak.Vector3D(0, 47, -9));

	-- Loads the table and places the table on the top of the level pavillion mesh
	-- table = peak.Game:get():createEntity("table", 0);
	-- table:setPosition(peak.Vector3D(0,  52,  4.9));
	
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
	-- The main player, the player receives the whole input atm
	player = peak.Game:get():createEntity("player", id);
	player:setPosition(peak.Vector3D(0, 10, 0));
end

