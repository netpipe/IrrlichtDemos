
bcclient = peak.BroadcastClient();

function create(thismenu)
	menu = thismenu;
end

function show()
	bcclient:setCallback(menu:getScript(), "broadcastcb");
	bcclient:start(27273);
	print("Starting.");
	listbox = menu:getListBox(1);
	
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
	bcclient:stop();
	auxrotorrotation:delete();
	auxrotor:delete();
	rotorrotation:delete();
	rotor:delete();
	helirotation:delete();
	heli:delete();
end

function menu_action(name, index)
	print("Menu action: "..name);
	if name == "join" then
		-- Join game
		selected = listbox:getSelected();
		if selected ~= -1 then
			if peak.Game:get():init(bcclient:getServerAddress(selected)) then
				peak.MenuManager:get():activateMenu("");
			end
		end
	end
	if name == "choosetype" then
		print("Type "..index.." chosen.");
	end
end

function broadcastcb()
	listbox:clearItems();
	for i=0,bcclient:getServerCount()-1 do
		listbox:addItem(bcclient:getServerAddress(i):getAddressString(true));
	end
end
