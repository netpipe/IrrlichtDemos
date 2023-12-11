uniform float fFurLength; 
uniform float fUVScale; 
uniform float fForce; 
uniform vec3  vGravity; 
uniform vec4  vAmbient; 
uniform float fLayer; // 0 to 1 for the level 

uniform vec3 vLightDir; 

//textures 
uniform sampler2D FurTexture; 
uniform sampler2D ColourTexture; 

varying vec3 normal; 

void main() 
{ 
   vec4 diffuseTexture = texture2D( FurTexture,  vec2(gl_TexCoord[0]) ); // Fur Texture - alpha is VERY IMPORTANT! 
   vec4 ColourTexture  = texture2D( ColourTexture, vec2(gl_TexCoord[1]) ); // Colour Texture for patterns 
  
   vec4 FinalColour = ColourTexture; 
   //FinalColour.a = diffuseTexture; 
   //FinalColour.a = diffuseTexture.a; 
   FinalColour = diffuseTexture;

   vec4 ambient = vAmbient* FinalColour; 
   vec4 diffuse = FinalColour; 
   FinalColour = ambient + diffuse * (dot(vLightDir, normal)*0.3); 

   if(fFurLength==0.0f) 
   { 
       FinalColour.a = 1.0f; 
   } 
   else 
      { 
       //FinalColour.a  = diffuseTexture * ( (1.0f-fFurLength)); 
	   //FinalColour.a  = diffuseTexture.a * ( (1.0f-fFurLength)); 
	    FinalColour  = diffuseTexture * ( (1.0f-fFurLength)); 
   } 
    
   gl_FragColor = FinalColour;

}