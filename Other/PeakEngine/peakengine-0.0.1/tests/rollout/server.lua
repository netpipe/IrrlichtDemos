
playerswanted = 1
playersconnected = 0
balls = {};

timer = peak.Timer();

function create_server()
	level = peak.Game:get():createEntity("level", 0);
	
	ball = peak.Game:get():createEntity("ball", 0);
	ball:getPhysicsComponent():move(peak.Vector3D(0, 2, 0));
	balls[0] = ball;
	playersconnected = playersconnected + 1;
	if playersconnected == playerswanted then
		start_countdown();
		timer:stop();
		timer:reset();
		timer:addCallback(3, peak.Game:get():getServerScript(), "start_game");
		print("Callback added.");
		timer:start();
	end
end
function destroy_server()
end

function client_connected(id)
	if playersconnected == playerswanted then
		return;
	end
	ball = peak.Game:get():createEntity("ball", id);
	ball:getPhysicsComponent():move(peak.Vector3D(id * 3, 2, 0));
	balls[playersconnected] = ball;
	playersconnected = playersconnected + 1;
end

function start_countdown()
	print("Starting countdown.");
	for i=0,playersconnected - 1 do
		balls[i]:getServerScriptComponent():getScript():callFunction("startcountdown");
	end
end

function start_game(timer, time)
	print("Starting game.");
	for i=0,playersconnected - 1 do
		balls[i]:getServerScriptComponent():getScript():callFunction("stopcountdown");
		balls[i]:getServerScriptComponent():getScript():callFunctionI("set_input", 1);
	end
end
