void registerWindow(){
	env->clear();
	runloop=true;
	receiver.getKeyboardEvents=false;
	receiver.getMouseEvents=false;
	receiver.getEnterPress=false;

	IGUIImage* loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
	globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-80));
	globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-40));
	globaltxtbox3= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
	globaltxtbox4= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
	env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-15, (driver->getScreenSize().Height)/2+80),0,5,L"Register");
	env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
	env->addButton(rect<s32>((driver->getScreenSize().Width)/2-10,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,4,L"Cancel");
	env->addStaticText(L"Confirm Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
	env->addStaticText(L"Email", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));
	env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-60, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-60));
	env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-100, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-100));
	while(device->run() && driver && runloop)
		if (device->isWindowActive()) 
		{
			driver->beginScene(true, true, SColor(0,122,65,171));
			env->drawAll();
			driver->endScene();
		}

}