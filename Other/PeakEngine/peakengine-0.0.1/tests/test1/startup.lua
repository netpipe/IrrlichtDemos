
peak.MenuManager:get():activateMenu("main");

--helloworld = peak.SoundFactory:get():getHelloWorld();
--sound = peak.SoundEngine:get():play2D(helloworld);
sound = peak.SoundEngine:get():play2D("sounds/FromE.ogg");
if sound ~= nil then
	sound:setPosition(peak.Vector3D(5, 0, 0));
end

overlay = peak.TextureOverlay:new("wood.png");
overlay:setPosition(peak.Vector2D(0.8, 0.8));
overlay:setSize(peak.Vector2D(0.1, 0.1));
overlay:setRotation(0.1);

overlay2 = peak.RectangleOverlay:new(peak.Color(255, 0, 0, 0),
	peak.Color(255, 0, 0, 50),
	peak.Color(255, 0, 0, 150),
	peak.Color(255, 0, 0, 255), overlay);
overlay2:setPosition(peak.Vector2D(0.35, 0.35));
overlay2:setSize(peak.Vector2D(0.3, 0.3));
overlay2:setRotation(0.5);

overlay3 = peak.LabelOverlay:new("Label Overlay!", peak.Color(255, 0, 0, 255),
	"haettenschweiler.png", overlay);
overlay3:setPosition(peak.Vector2D(0, 0));
overlay3:setSize(peak.Vector2D(1, 1));

--[[if not peak.Game:get():init(true) then
	peak.Game:get():init(peak.Address("127.0.0.1"));
end--]]

