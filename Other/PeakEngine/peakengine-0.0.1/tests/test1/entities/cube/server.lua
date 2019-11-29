
entity = nil;
script = nil;

body = nil;

groundcontact = 1;

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
	jumping = entity:getVariables():getInteger("jump");
	if jumping ~= 0 then
		jumping = 0;
		if groundcontact ~= 0 then
			vel = body:getLinearVelocity();
			vel.y = 8;
			body:setLinearVelocity(vel);
		end
	end
	forward = entity:getVariables():getInteger("forward");
	if forward ~= 0 then
		vel = body:getLinearVelocity();
		vel.z = 5;
		body:setLinearVelocity(vel);
	end
	back = entity:getVariables():getInteger("back");
	if back ~= 0 then
		vel = body:getLinearVelocity();
		vel.z = -5;
		body:setLinearVelocity(vel);
	end
	left = entity:getVariables():getInteger("left");
	if left ~= 0 then
		vel = body:getLinearVelocity();
		vel.x = -5;
		body:setLinearVelocity(vel);
	end
	right = entity:getVariables():getInteger("right");
	if right ~= 0 then
		vel = body:getLinearVelocity();
		vel.x = 5;
		body:setLinearVelocity(vel);
	end
	groundcontact = 0;
end

function collision1(collisioninfo)
	return 1;
end

function collision2(collisioninfo)
	groundcontact = 1;
end

