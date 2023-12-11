//Note this is the 6 textures version, hence, you will need to recompile Irrlicht if you want
//to use it.

//Can be reduced to a 4 textures version if you sample the baseX instead of the baseZ texture
//and the normalX instead of the normalZ texture.

//And can be reduced further more if you use a single normal map, and sample it from every side
//everytime, leaving space for a shadowmap, the shader does the rest.

//By the way, lighting is performed in worldspace, because otherwise, the normal calculation
//and the rotation support could be too complex.

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float specularPower;
uniform vec3 lightDirection;
uniform float camDepth;

uniform sampler2D baseX;
uniform sampler2D baseY;
//uniform sampler2D baseZ;
uniform sampler2D normalX;
uniform sampler2D normalY;
//uniform sampler2D normalZ;

varying vec2 TexCoordX;
varying vec2 TexCoordY;
varying vec2 TexCoordZ;
varying vec3 ViewDir;
varying mat3 tangentSpace;

void main( void )
{
   float Fog = gl_FragCoord.z/gl_FragCoord.w;
   Fog/=camDepth;

   vec3 normal = tangentSpace[2];
   
   vec3 n = normal;
   n*=n;
   
   vec4 col = texture2D(baseX,TexCoordX)*n.x+
              texture2D(baseY,TexCoordY)*n.y+
              texture2D(baseX,TexCoordZ)*n.z;
   
   vec4 nrm = texture2D(normalX,TexCoordX)*n.x+
              texture2D(normalY,TexCoordY)*n.y+
              texture2D(normalX,TexCoordZ)*n.z;
   
   vec3 realNormal = normalize((2.0*nrm.xyz-1.0)*tangentSpace);
   vec3 lightDir = normalize(lightDirection);
   vec3 viewDir = normalize(ViewDir);
   
   float NdL = max(0.0,dot(realNormal,lightDir));
   
   vec3 reflVect = normalize(reflect(lightDir,realNormal));
   float RdL = max(0.0,dot(reflVect,viewDir));
      
   vec4 fVambient,fVdiffuse,fVspecular;
   
   fVambient = col*ambient;
   fVdiffuse = diffuse*col*NdL;
   fVspecular = specular*pow(RdL,specularPower)*col;
   
   gl_FragColor = mix(fVambient+fVdiffuse+fVspecular,gl_Fog.color,Fog);
       
}