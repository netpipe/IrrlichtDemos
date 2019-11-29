
function show()
	peak.GraphicsEngine:get():getStandardCamera():setPosition(0, 0, -1.9);
	heli = peak.MeshSceneNode:new("apache.3ds");
	heli:setMaterial("heli");
	heli:setPosition(peak.Vector3D(0, 0, 0));
	helirotation = peak.RotationController:new();
	helirotation:setSceneNode(heli);
	helirotation:setRotationSpeed(peak.Vector3D(0, 0.5, 0));
	rotor = peak.MeshSceneNode:new("mainrotor.3ds", false, heli);
	rotor:setMaterial("heli");
	rotor:setPosition(peak.Vector3D(0, 0.137, 0.249));
	rotor:setScale(peak.Vector3D(0.001, 0.001, 0.001));
	rotorrotation = peak.RotationController:new();
	rotorrotation:setSceneNode(rotor);
	rotorrotation:setRotationSpeed(peak.Vector3D(0, 10, 0));
	auxrotor = peak.MeshSceneNode:new("auxrotor.3ds", false, heli);
	auxrotor:setMaterial("heli");
	auxrotor:setPosition(peak.Vector3D(0.100, 0.135, 1.205));
	auxrotor:setScale(peak.Vector3D(0.001, 0.001, 0.001));
	auxrotorrotation = peak.RotationController:new();
	auxrotorrotation:setSceneNode(auxrotor);
	auxrotorrotation:setRotationSpeed(peak.Vector3D(10, 0, 0));
end

function hide()
	auxrotorrotation:delete();
	auxrotor:delete();
	rotorrotation:delete();
	rotor:delete();
	helirotation:delete();
	heli:delete();
end

function menu_action(name)
	print("Menu action: "..name);
	if name == "startgame" then
		-- Start game
		if not peak.Game:get():init(true) then
			peak.Game:get():init(peak.Address("127.0.0.1"));
		end
	end
end
