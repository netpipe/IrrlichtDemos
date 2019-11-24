
rotation = peak.Vector3D();
xrotation = 0;
zrotation = 0;

rocketcooldown = 0;

hp = 100;

dead = 0;
killed = 0;

function create(newentity, component)
	-- Get entity info
	entity = newentity;
	script = component:getScript();
	
	body = entity:getPhysicsComponent():getBody(1);
	geom = entity:getPhysicsComponent():getGeom(2);
	--[[geom:getCollisionInfo().bounce = 0;
	geom:getCollisionInfo().callback1.script = script;
	geom:getCollisionInfo().callback1.functionname = "collision1";
	geom:getCollisionInfo().callback2.script = script;
	geom:getCollisionInfo().callback2.functionname = "collision2";--]]
	forwardvar = entity:getVariables():getVariable("forward");
	backvar = entity:getVariables():getVariable("back");
	leftvar = entity:getVariables():getVariable("left");
	rightvar = entity:getVariables():getVariable("right");
	upvar = entity:getVariables():getVariable("up");
	downvar = entity:getVariables():getVariable("down");
	s1var = entity:getVariables():getVariable("shoot1");
	s2var = entity:getVariables():getVariable("shoot2");
end

function update(msecs)
	if s1var:getInteger() == 1 and dead == 1 then
		dead = 0;
		body:setGravity(false);
		respawn();
		hp = 100;
		entity:getVariables():setInteger("hp", 100);
	end
	
	body:setAngularVelocity(peak.Vector3D(0, 0, 0));
	
	if killed == 1 then
		body:setGravity(true);
		body:setAngularVelocity(peak.Vector3D(0, 0, 0));
		killed = 0;
	end
	if dead == 1 then
		return;
	end
	
	-- Movememt
	local forward = forwardvar:getInteger();
	local back = backvar:getInteger();
	local left = leftvar:getInteger();
	local right = rightvar:getInteger();
	local up = upvar:getInteger();
	local down = downvar:getInteger();
	local currentspeed = body:getLinearVelocity();
	currentspeed:rotateXZ(rotation.y);
	local wantedspeed = peak.Vector3D((right - left) * 50, (up - down) * 30, (back - forward) * 50);
	local force = peak.Vector3D((wantedspeed.x - currentspeed.x), (wantedspeed.y - currentspeed.y), (wantedspeed.z - currentspeed.z));
	force:rotateXZ(-rotation.y);
	body:setForce(force);
	
	-- Tilt helicopter when moving
	if zrotation * 4 < left - right then
		zrotation = zrotation + 0.001 * msecs;
	end
	if zrotation * 4 > left - right then
		zrotation = zrotation -  0.001 * msecs;
	end
	if xrotation * 4 < back - forward then
		xrotation = xrotation + 0.001 * msecs;
	end
	if xrotation * 4 > back - forward then
		xrotation = xrotation -  0.001 * msecs;
	end
	
	-- Rotation around y-axis
	if entity:getVariables():wasUpdated("mousemovement") then
		pos = entity:getVariables():getVector2D("mousemovement");
		rotation.y = rotation.y + pos.x / 100;
		rotation.x = rotation.x - pos.y / 100;
	end
	body:setRotation(peak.Quaternion(xrotation, rotation.y, zrotation));
	
	-- Rockets
	rocketcooldown = rocketcooldown + msecs;
	if s2var:getInteger() ~= 0 and rocketcooldown > 1000 then
		-- Fire rocket
		rocket = peak.Game:get():createEntity("rocket", entity:getOwner());
		rocketscript = rocket:getServerScriptComponent():getScript();
		parameters = peak.ParameterList();
		parameters:add(peak.Parameter(body:getPosition().x));
		parameters:add(peak.Parameter(body:getPosition().y));
		parameters:add(peak.Parameter(body:getPosition().z));
		parameters:add(peak.Parameter(body:getRotation().q[0]));
		parameters:add(peak.Parameter(body:getRotation().q[1]));
		parameters:add(peak.Parameter(body:getRotation().q[2]));
		parameters:add(peak.Parameter(body:getRotation().q[3]));
		rocketscript:callFunction("initialize", parameters);
		rocketcooldown = 0;
	end
end

function hit(damage)
	hp = hp - damage;
	entity:getVariables():set("hp", hp);
	print("HP left: "..hp);
	entity:getVariables():setInteger("hp", hp);
	if hp <= 0 then
		dead = 1;
		killed = 1;
	end
end

function set_team(team)
	entity:getVariables():setInteger("team", team);
	respawn();
end

function respawn()
	team = entity:getVariables():getInteger("team");
	spawnpoints = peak.Level:get():getPointGroup("spawnpoints"..team);
	if spawnpoints ~= nil then
		pointindex = math.random(0, spawnpoints:getPointCount() - 1);
		spawnpoint = spawnpoints:getPoint(pointindex);
		body:setPosition(spawnpoints:getPoint(pointindex));
		if spawnpoints:getRotationCount() >= pointindex then
			rotation.y = spawnpoints:getRotation(pointindex).y;
		end
	end
	body:setAngularVelocity(peak.Vector3D(0, 0, 0));
	body:setLinearVelocity(peak.Vector3D(0, 0, 0));
end

function collision1(collisioninfo)
	return 1;
end

function collision2(collisioninfo)
end

