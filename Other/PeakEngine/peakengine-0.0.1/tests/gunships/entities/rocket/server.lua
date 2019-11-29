
timetolive = 10000

function create(newentity, component)
	-- Get entity info
	entity = newentity;
	script = component:getScript();
	
	body = entity:getPhysicsComponent():getBody(1);
	geom = entity:getPhysicsComponent():getGeom(2);
	geom:getCollisionInfo().bounce = 0;
	geom:getCollisionInfo().callback1.script = script;
	geom:getCollisionInfo().callback1.functionname = "collision1";
	geom:getCollisionInfo().callback2.script = script;
	geom:getCollisionInfo().callback2.functionname = "collision2";
end

function update(msecs)
	speed = body:getLinearVelocity();
	body:setAngularVelocity(peak.Vector3D(0, 0, 0));
	timetolive = timetolive - msecs;
	if timetolive <= 0 then
		peak.Game:get():deleteEntity(entity);
	end
	if target ~= nil then
		posdifference = peak.Vector3D(targetbody:getPosition().x - body:getPosition().x,
			targetbody:getPosition().y - body:getPosition().y,
			targetbody:getPosition().z - body:getPosition().z);
		rotation = peak.Quaternion(posdifference:getRotation());
		currentrotation = body:getRotation();
		
		body:setRotation(currentrotation:slerp(rotation, 0.01));
		
		speed = peak.Vector3D(0, 0, 70);
		speed = body:getRotation():rotateVector(speed);
		speed.x = - speed.x;
		speed.y = - speed.y;
		body:setLinearVelocity(speed);
	end
end

function initialize(x, y, z, rw, rx, ry, rz)
	offset = peak.Vector3D(0, 0, 4);
	rotation = peak.Quaternion(rw, rx, ry, rz);
	offset = rotation:rotateVector(offset);
	body:setPosition(peak.Vector3D(x - offset.x, y - offset.y, z + offset.z));
	body:setRotation(peak.Quaternion(rw, rx, ry, rz));
	
	-- Get target
	helicount = peak.Game:get():getEntityCount("heli");
	if helicount == 0 then
		-- No target?
	else
		-- Look for best target
		-- TODO: Also look at distance
		bestrotation = 100;
		for i=0,(helicount-1) do
			heli = peak.Game:get():getEntity("heli", i);
			helibody = heli:getPhysicsComponent():getBody(1);
			posdifference = peak.Vector3D(helibody:getPosition().x - body:getPosition().x,
				helibody:getPosition().y - body:getPosition().y,
				helibody:getPosition().z - body:getPosition().z);
			rotation = posdifference:getRotation();
			currentaxis = peak.Vector3D(0, 0, 1);
			currentaxis = body:getRotation():rotateVector(currentaxis);
			currentaxis.x = -currentaxis.x;
			currentrotation = currentaxis:getRotation();
			rotation = peak.Vector3D(rotation.x - currentrotation.x, rotation.y - currentrotation.y, rotation.z - currentrotation.z);
			while rotation.x > math.pi do
				rotation.x = rotation.x - 2 * math.pi;
			end
			if rotation.y < 0 then
				rotation.y = -rotation.y;
			end
			if rotation.y < bestrotation then
				target = peak.Game:get():getEntity("heli", i);
				targetbody = target:getPhysicsComponent():getBody(1);
				bestrotation = rotation.y;
			end
		end
	end
end

function collision1(collisioninfo)
	-- Place big impressive explosion here :)
	
	-- Test whether we have hit a helicopter
	hit = nil;
	body1 = collisioninfo.geom1:getBody();
	if body1 ~= nil then
		entity1 = body1:getEntity();
		if entity1:getName() == "heli" then
			hit = entity1;
		end
	end
	body2 = collisioninfo.geom2:getBody();
	if body2 ~= nil then
		entity2 = body2:getEntity();
		if entity2:getName() == "heli" then
			hit = entity2;
		end
	end
	
	if hit ~= nil then
		print("Helicopter hit!");
		hit:getServerScriptComponent():getScript():callFunctionI("hit", 30);
	end
	
	-- Delete rocket
	peak.Game:get():deleteEntity(entity);
	
	return 0;
end

function collision2(collisioninfo)
end

