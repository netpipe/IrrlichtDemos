
graphics = peak.GraphicsEngine:get();
camera = nil;

function create_client()
	-- Create camera
	camera = peak.CameraSceneNode:new();
	camera:setPosition(peak.Vector3D(0, 6, -15));
	
end

function destroy_client()
	camera:delete();
end

function update(msecs)
end

-- Input functions
function mouse_moved(x, y, wheel)
	print("Mouse moved.");
end
function action_input(action, state)
	if action == "quitgame" then
		peak.Game:get():stopGame();
		peak.MenuManager:get():activateMenu("main");
	end
	print("Action: "..action..", "..state);
end
