
function create_server()
	plane = peak.Plane:new();
	plane:init(peak.Game:get():getWorld(), peak.Vector3D(0, 1, 0));
	
	for y=0,15 do
		cube = peak.Game:get():createEntity("cube", 0);
		cube:getPhysicsComponent():move(peak.Vector3D(math.random() * 0.01, y * 2 + 15, math.random() * 0.01));
	end
end
function destroy_server()
end

function client_connected(id)
	for y=0,9 do
		cube = peak.Game:get():createEntity("cube", id);
		cube:getPhysicsComponent():move(peak.Vector3D(id + math.random() * 0.01, y * 2 + 15, math.random() * 0.01));
	end
end

