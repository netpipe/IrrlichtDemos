
graphics = peak.GraphicsEngine:get();
camera = nil;

model = nil;

function create_client()
end

function destroy_client()
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
