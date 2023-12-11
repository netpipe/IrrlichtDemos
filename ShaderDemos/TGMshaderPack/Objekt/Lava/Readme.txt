All you have to do is to assign the two Textures (LavaTiles.jpg and cloud.tga) as tex1 & 2 to the objekt,
assign the shader material as sseen in the example, and maybe change the way the callback updates the time(slower/faster/FPS independent)

you can change the size of the Lava pattern by changing this two lines in the frag shader code:

"float     p=  texture2D( Texture1,T1*2.0).a;"
"vec4 color =  texture2D( Texture2, T2*2.0);"

(change both 2.0 to some other values). 
 