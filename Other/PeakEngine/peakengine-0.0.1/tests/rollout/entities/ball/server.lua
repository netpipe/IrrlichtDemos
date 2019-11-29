
entity = nil;
script = nil;

body = nil;

groundcontact = 1;

timerstarted = 0;
timerstopped = 0;

timer = peak.Timer();

inputenabled = 0;

function create(newentity, component)
	-- Get entity info
	entity = newentity;
	script = component:getScript();
	
	body = entity:getPhysicsComponent():getBody(1);
	geom = entity:getPhysicsComponent():getGeom(2);
	geom:getCollisionInfo().bounce = 0.05;
	geom:getCollisionInfo().callback1.script = script;
	geom:getCollisionInfo().callback1.functionname = "collision1";
	geom:getCollisionInfo().callback2.script = script;
	geom:getCollisionInfo().callback2.functionname = "collision2";
end

function update(msecs)
	if inputenabled == 0 then
		return;
	end
	jumping = entity:getVariables():getInteger("jump");
	if jumping ~= 0 then
		jumping = 0;
		if groundcontact ~= 0 then
			vel = body:getLinearVelocity();
			vel.y = 4;
			body:setLinearVelocity(vel);
		end
	end
	if groundcontact ~= 0 then
		force = 30;
	else
		force = 5;
	end
	forward = entity:getVariables():getInteger("forward");
	if forward ~= 0 then
		body:addForce(peak.Vector3D(0, 0, force));
	end
	back = entity:getVariables():getInteger("back");
	if back ~= 0 then
		body:addForce(peak.Vector3D(0, 0, -force));
	end
	left = entity:getVariables():getInteger("left");
	if left ~= 0 then
		body:addForce(peak.Vector3D(-force, 0, 0));
	end
	right = entity:getVariables():getInteger("right");
	if right ~= 0 then
		body:addForce(peak.Vector3D(force, 0, 0));
	end
	groundcontact = 0;
end

function collision1(collisioninfo)
	return 1;
end

function collision2(collisioninfo)
	groundcontact = 1;
end

function startcountdown()
	entity:getVariables():setInteger("timerenabled", -1);
end
function stopcountdown()
	entity:getVariables():setInteger("timerenabled", 0);
end

function starttimer()
	if timerstarted == 0 then
		timerstarted = 1;
		print("Timer started!");
		entity:getVariables():setInteger("timerenabled", 1);
		timer:start();
	end
end

function stoptimer()
	if timerstopped == 0 and timerstarted == 1 then
		timerstopped = 1;
		print("Timer stopped!");
		entity:getVariables():setFloat("time", timer:getTime());
		entity:getVariables():setInteger("timerenabled", 0);
		timer:stop();
		print("Time: "..timer:getTime().." seconds.");
	end
end

function set_input(enabled)
	inputenabled = enabled;
end

