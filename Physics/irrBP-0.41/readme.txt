Welcome to IrrBP SDK.
IrrBP is an Irrlicht - Bullet wrapper to simply use the bullet' physics with irrlicht graphics.

> The wrapper is compiled against Irrlicht > 1.7.1 (also trunk versions) and Bullet 2.79. I'm not sure about backward compatibility
> The whole library is compiled under VS 2010.
> You can see the changelog in the "changes.txt" file
> In the precompiled packages, the library are compiled under VS2010.

Thanks for contributions:
- Buck1000 for the trimesh converter (from Irrlicht Forum)
- Cobra for the "shooting" object source (from Irrlicht Forum)
- Going for the soft body help (from Bullet Forum)
- Serengeor for the code::blocks project (from Irrlicht Forum)

Important NOTES for Microsoft Visual Studio (2010) Users:
- Since r58 and 0.41 release all visual-studio projects will be published for vs2010. No backwards compatibility guarantee.
SO, VS/VC++ 2005 AND VS/VC++ 2008 SUPPORT WILL BE DROPPED FROM THIS VERSION.

- Your project won't compile in a different configuration than  "Release", unless you put this line in the preprocessor definitions:
  "_ITERATOR_DEBUG_LEVEL=0" (Workaround so far)

- IF YOU ARE USING A BULLET VERSION > 2.79 AND YOU WANT TO COMPILE YOUR OWN VERSION OF BULLET\IRRBP:
  Since version 2.79, bullet doesn't provide the bullet multithread library project in the package, so you have to build your own one otherwise
  irrBP will !!not!! compile.

- IF YOU ARE USING A BULLET VERSION > 2.76 PLEASE READ THE DOCUMENTATION TIP.