//Triplanar coordinate maps with normal maps support
//by Santiago A. Navascues

//Use at your own risk. I make no warranties. 
//If you want to use it, give the proper credits

uniform mat4 matWorld;
uniform vec3 viewPos;
uniform float tileSize;

varying vec2 TexCoordX;
varying vec2 TexCoordY;
varying vec2 TexCoordZ;
varying vec3 ViewDir;
varying mat3 tangentSpace;
     
void main( void )
{
   gl_Position = ftransform();
   vec3 worldPos = (matWorld * gl_Vertex).xyz;//So we obtain the world position
   
   TexCoordX = worldPos.zy/tileSize;//here are our texture coordinates...
   TexCoordY = worldPos.xz/tileSize;
   TexCoordZ = worldPos.xy/tileSize;
   
   //binormal and tangent become normal dependant
   
   vec3 xtan,ytan,ztan;
   vec3 xbin,ybin,zbin;
   
   xtan = vec3(0,0,1);//tangent space for the X aligned plane
   xbin = vec3(0,1,0);
   
   ytan = vec3(1,0,0);//tangent space for the Y aligned plane
   ybin = vec3(0,0,1);
   
   ztan = vec3(1,0,0);//tangent space for the Z aligned plane
   zbin = vec3(0,1,0);
   
   vec3 n = normalize(gl_Normal);
   n*=n;

   vec3 worldBinormal = normalize(xbin*n.x+ybin*n.y+zbin*n.z);//Average Binormal
   vec3 worldTangent = normalize(xtan*n.x+ytan*n.y+ztan*n.z);//Average Tangent

   ViewDir = worldPos-viewPos;
   
   //This is done so it can be rotated freely
   
   worldTangent = (matWorld*vec4(worldTangent,1)).xyz;
   worldBinormal = (matWorld*vec4(worldBinormal,1)).xyz;
   
   //For some odd reason i cannot yet to explain, the normal is oddly altered so the X axis gets always positive
   //Lucky enough, tangent x binormal is by definition the normal, and this solves the issue...

   vec3 worldNormal = normalize(cross(worldTangent,worldBinormal));
   
   tangentSpace[0] = worldTangent;
   tangentSpace[1] = worldBinormal;
   tangentSpace[2] = worldNormal;
}