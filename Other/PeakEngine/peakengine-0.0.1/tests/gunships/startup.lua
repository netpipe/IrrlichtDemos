
peak.MenuManager:get():activateMenu("main");

sound = peak.SoundEngine:get():play2D("sounds/bgm2.ogg", false, true);
sound:setVolume(0.3);

--[[if not peak.Game:get():init(true) then
	peak.Game:get():init(peak.Address("127.0.0.1"));
end--]]

