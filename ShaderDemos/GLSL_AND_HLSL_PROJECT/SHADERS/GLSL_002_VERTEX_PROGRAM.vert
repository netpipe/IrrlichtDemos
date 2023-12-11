
 //  - VERTEX PROGRAM -  by Jacques Andre Pretorius 2015 
 // "Vectrotek Shader V2.1" 

 // You will see that lighting is not done in here because we do it in the Fragment Shader..
 // To keep open the possibility of doing more in here in future we do as little as possible here..

 uniform mat4 M01World;    // Calculated in the Appcode..
 varying vec3 Tangent;     // Calculated in the Appcode..
 varying vec3 Binormal;    // Calculated in the Appcode..
 varying vec2 UVCoordsXY;
 varying vec3 VNormal;
 varying vec3 VertexGLPosition;

 void main()
  {UVCoordsXY        = gl_MultiTexCoord0.xy; // There is a lot that can be done with these such as more UV channels for shadowing etc..
   Tangent           = gl_MultiTexCoord1;    // Acuired here and  passed on to the FRAGMENT SHADER.. (must be passed through here..)
   Binormal          = gl_MultiTexCoord2;
   VNormal           = gl_Normal;            // Unseen normal passed from the User App.. Must happen here..
   VNormal           = vec3(M01World * vec4(VNormal.xyz, 0 )).xyz;            // Looks like we got away with this..
   Tangent           = normalize(vec3(M01World * vec4(Tangent.xyz, 0 )).xyz); // "M01World" is our "Golden Thread"..
   Binormal          = normalize(vec3(M01World * vec4(Binormal.xyz,0)).xyz);
   // Forget about getting the Binormal on the fly in the shader..
   // It just doesn't work that way (quadratics produce Pos and Neg results and we have no clue which result is the right one)..
   gl_Position       = gl_ModelViewProjectionMatrix * gl_Vertex;
   // This ensures that Object "TRANSLATION", Rotation and "SCALE" is considered..
   // With "Physically Based Lighting" this becomes all important!..
   VertexGLPosition  = vec3(M01World * vec4(gl_Vertex.xyz,1)).xyz;
  }

