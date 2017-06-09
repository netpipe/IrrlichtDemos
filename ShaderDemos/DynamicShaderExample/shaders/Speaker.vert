uniform mat4 view_proj_matrix;
uniform mat4 view_matrix;
uniform float time_0_X;
uniform float pulseSharpness;
uniform float bassFrequency;
uniform float amplitude;
uniform float beat;

varying vec2 vTexCoord;
varying vec3 vNormalES;
varying vec3 vViewVec;
varying vec3 vLightDir;

// Sample offsets to generate the normal
const vec2 offY = vec2(0.0, 0.1);
const vec2 offZ = vec2(0.1, 0.0);

// 2 * pi
const float PI2 = 6.2831853;


float getX(vec2 pos, float pulse)
{
   // Speaker shape
   float d = length(pos);
   float x = 15.0 * smoothstep(20.0, 25.0, d) 
            - 0.4 * smoothstep(30.0, 62.0, d) * d;

   // Add the pulse making it stronger towards the center of the speaker
   return x + pulse * sqrt(80.0 - d);
}

void main(void)
{
   vec4 Pos = gl_Vertex;

   // Our model isn't centered around origin, so we'll move it
   // there before doing anything.
   //Pos.y += 3.0;
   //Pos.z -= 6.4;

   float t = fract(beat * time_0_X);

   // Create a pulse function, that is. starts from 0, does a
   // sharp rise to 1 and a less sharp decline back to 0 again,
   // Using f(t) = t * (1 - t)^p will suit us for this.
   // We want this curve to reach a value of 1 at its maximum in
   // the 0 < r < 1 interval. So we'll need to find the maximum
   // value in this interval, invert and multiply with the curve.
   // Those interested in math might want to derive the invMax
   // calculation below themselves by solving for f'(t) = 0
   // and inserting into the function.
   float invMax = (pulseSharpness + 1.0) * pow(1.0 + 1.0 / pulseSharpness, pulseSharpness);
   float pulse = amplitude * invMax * t * pow(1.0 - t, pulseSharpness);

   // Swing the pulse at a certain frequency
   pulse *= sin(bassFrequency * PI2 * time_0_X);

   // Sample at three positions ...
   float x0 = getX(Pos.yz, pulse);
//#define BOMB
#ifdef BOMB
   //float x1 = getX(Pos.yz + offY, pulse);
   //float x2 = getX(Pos.yz + offZ, pulse);
   
#else
   float d = length(Pos.yz + offY);
   float x = 15.0 * smoothstep(20.0, 25.0, d) - 0.4 * smoothstep(30.0, 62.0, d) * d;

   // Add the pulse making it stronger towards the center of the speaker
   float x1 = x + pulse * sqrt(80.0 - d);
   
   d = length(Pos.yz + offZ);
   x = 15.0 * smoothstep(20.0, 25.0, d) - 0.4 * smoothstep(30.0, 62.0, d) * d;

   // Add the pulse making it stronger towards the center of the speaker
   float x2 = x + pulse * sqrt(80.0 - d);
#endif
   Pos.x = x0;

   // .. then form the tangent and binormal vectors
   vec3 tangent  = vec3(x1 - x0, offY);
   vec3 binormal = vec3(x2 - x0, offZ);

   // ... which will give us our normal
   vec3 normal;
   //normal = tangent;
   normal = normalize(cross(tangent, binormal));

   gl_Position = gl_ModelViewProjectionMatrix * Pos;
   
   vTexCoord = 1.0-gl_MultiTexCoord0.yx;
   // Eye-space lighting
   vNormalES = gl_NormalMatrix * normal;
   
   vViewVec  = -vec3(gl_ModelViewMatrix * Pos);

   vLightDir = vec3(gl_ModelViewMatrix * gl_LightSource[0].position);
 
   


}
