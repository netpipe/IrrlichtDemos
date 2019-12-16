
peak.Plugin:addPlugin("plugins/libArizona");

peak.MenuManager:get():activateMenu("main");

--helloworld = peak.SoundFactory:get():getHelloWorld();
--sound = peak.SoundEngine:get():play2D(helloworld);
--[[sound = peak.SoundEngine:get():play2D("sounds/FromE.ogg");
if sound ~= nil then
	sound:setPosition(peak.Vector3D(5, 0, 0));
end--]]

peak.GraphicsEngine:get():saveMessages();

--[[if not peak.Game:get():init(true) then
	peak.Game:get():init(peak.Address("127.0.0.1"));
end--]]

