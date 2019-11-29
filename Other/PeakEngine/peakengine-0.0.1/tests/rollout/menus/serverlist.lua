
menu = nil;

bcclient = peak.BroadcastClient();

function create(thismenu)
	menu = thismenu;
end

function show()
	bcclient:setCallback(menu:getScript(), "broadcastcb");
	bcclient:start(27273);
	print("Starting.");
end

function hide()
	bcclient:stop();
end

function menu_action(name, index)
	print("Menu action: "..name);
	if name == "join" then
		-- Join game
		-- TODO
	end
	if name == "choosetype" then
		print("Type "..index.." chosen.");
	end
end

function broadcastcb()
	print("broadcastcb");
end
