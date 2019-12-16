
graphics = peak.GraphicsEngine:get();
camera = nil;

function create_client()
	-- Create camera
	camera = peak.GraphicsEngine:get():getStandardCamera();
	camera:setPosition(peak.Vector3D(0, 6, 15));
	
	plane = peak.Plane:new();
	plane:create(peak.Vector3D(0, 1, 0));
	planebody = peak.Body:new();
	planebody:create(peak.Game:get():getWorld(), plane);
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

