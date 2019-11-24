
function create(newentity)
	entity = newentity;
end

function destroy()
	light:delete();
end

function graphics_created(graphics)
	model = graphics:getSceneNode(3);
	light = peak.Light:new(model);
end

function update(msecs)
end

