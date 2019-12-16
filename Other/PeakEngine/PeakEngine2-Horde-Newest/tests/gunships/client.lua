
graphics = peak.GraphicsEngine:get();
camera = nil;

model = nil;

function create_client()
	--[[for i=1,1000 do
		local tree = peak.ModelSceneNode:new("tree");
		tree:setScale(peak.Vector3D(0.1, 0.1, 0.1));
		tree:setPosition(peak.Vector3D(math.random() * 200 - 100, 0, math.random() * 200 - 100));
	end--]]
	peak.GraphicsEngine:get():saveMessages();
	
	quitlink = peak.EventScriptLink:new();
	quitlink:create(peak.InputManager:get():getEvent("KeyReleased"), peak.Game:get():getClientScript(), "quitgameevent", "quitgame");
end

function destroy_client()
	quitlink:destroy();
	quitlink:delete();
end

function update(msecs)
end

function quitgameevent()
	peak.Game:get():stopGame();
	peak.MenuManager:get():activateMenu("main");
end

