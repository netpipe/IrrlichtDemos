
function create_server()
	--plane = peak.Plane:new();
	--plane:init(peak.Game:get():getWorld(), peak.Vector3D(0, 1, 0));
	
	newship = peak.Game:get():createEntity("heli", 0);
	newship:getServerScriptComponent():getScript():callFunctionI("set_team", 1);
end
function destroy_server()
end

function client_connected(id)
	newship = peak.Game:get():createEntity("heli", id);
	newship:getServerScriptComponent():getScript():callFunctionI("set_team", 1 + math.mod(id, 2));
end

