//uniform mat4 matView;
//uniform bool bViewSpace;

varying vec2 TexCoord;
varying vec3 Normal;
varying vec3 Binormal;
varying vec3 Tangent;

//attribute vec3 rm_Binormal;
//attribute vec3 rm_Tangent;

void main( void )
{
   mat4 matTransform = mat4( 1.0, 0.0, 0.0, 0.0,
                             0.0, 1.0, 0.0, 0.0,
                             0.0, 0.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0 );

   vec3 rm_Tangent = gl_MultiTexCoord1.xyz;
   vec3 rm_Binormal = -gl_MultiTexCoord2.xyz;

   
   //if( bViewSpace )
   //{
   //   matTransform = matView;
   //} // end if ( bViewSpace )

   gl_Position = ftransform();
   TexCoord    = gl_MultiTexCoord0.xy;
   Normal      = ( ( matTransform * vec4( gl_Normal.xyz, 0.0 ) ).xyz + 1.0 ) / 2.0;
   Binormal    = ( ( matTransform * vec4( rm_Binormal.xyz, 0.0 ) ).xyz + 1.0 ) / 2.0;
   Tangent     = ( ( matTransform * vec4( rm_Tangent.xyz, 0.0 ) ).xyz + 1.0 ) / 2.0;
   
}
