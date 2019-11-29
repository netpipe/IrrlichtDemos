
const stringc vertBumpShader =
    "uniform vec3 fvLightPosition1;"

    ""
    "uniform float fLightStrength1;"

    ""
    "uniform mat4 matWorldInverse;"
    ""
    "varying vec2 Texcoord;"
    "varying vec3 ViewDirection;"
    "varying vec3 LightDirection1;"
    "varying vec3 nr;"


    "varying vec4 LightDistMultiplier;"
    ""
    "varying float depth;"
    "varying float height;"
    "varying vec2 Texcoord2;"

    "void main( void )"
    "{"
    ""
    "   mat4 LightTransform= gl_ModelViewMatrix;"
    "   LightTransform=LightTransform*matWorldInverse;"
    ""
    "   gl_Position = ftransform();"
    "   Texcoord    = gl_MultiTexCoord0.xy;"
    "   Texcoord2    = gl_MultiTexCoord1.xy;"
    "   height=gl_Vertex.y;"

    "depth=gl_Position.z;"
    "   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;"
    "   vec4 fvLightPos1 = LightTransform * vec4(fvLightPosition1,1.0);"

    ""
    "   vec3 fvViewDirection  =  - fvObjectPosition.xyz;"
    ""
    "   vec3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz);"

    ""

    "nr=gl_Normal;"
    "   vec3 fvNormal         = gl_NormalMatrix * normalize(gl_Normal);"
    ""
    "   vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0.0);"
    "   vec3 fvBinormal =cross(fvTangent,gl_Normal);  "
    "   fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);"
    "   fvBinormal=gl_NormalMatrix*fvBinormal;"
    ""

    "   ViewDirection.x  = dot( fvTangent, fvViewDirection );"
    "   ViewDirection.y  = dot( fvBinormal, fvViewDirection );"
    "   ViewDirection.z  = dot( fvNormal, fvViewDirection );"
    ""
    "   LightDirection1.x  = dot( fvTangent, fvLightDirection1.xyz );"
    "   LightDirection1.y  = dot( fvBinormal, fvLightDirection1.xyz );"
    "   LightDirection1.z  = dot( fvNormal, fvLightDirection1.xyz );"
    ""

    "}";
// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragBumpShader =
    "uniform vec4 fvAmbient;"
    "uniform vec4 fvLight1Color;"


    "uniform float fSpecularPower;"
    "uniform float fSpecularStrength;"


    "uniform sampler2D tex0;"
    "uniform sampler2D tex1;"
    "uniform sampler2D tex2;"
    "uniform sampler2D tex3;"
    "varying vec2 Texcoord;"
    "varying vec2 Texcoord2;"
    "varying vec3 ViewDirection;"
    "varying vec3 LightDirection1;"
    "varying vec3 nr;"

    "varying vec4 LightDistMultiplier;"
    "varying float depth;"
    "varying float height;"



    "void main( void )"
    "{"

    "   vec3  fvLightDirection1 = normalize( LightDirection1 );"

    "   vec3  fvNormal         = texture2D( tex2, Texcoord ).yxz;"
    "   fvNormal.xy*=2.0;"
    "   fvNormal.xy-=1.0;"


    "   fvNormal=normalize(fvNormal);"

    "   float fNDotL1           = clamp(dot(fvNormal, fvLightDirection1),0.0,1.0); "


    "   vec3  fvViewDirection  = normalize( ViewDirection );"
    "   vec3  fvReflection1     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection1 ); "



    "   float fRDotV1          = max( 0.0, dot( fvReflection1, fvViewDirection ) );"

    "   vec4  fvBaseColor      = texture2D( tex0, Texcoord );"
    "   vec4 lmap=texture2D( tex1, Texcoord2 );"
    "   lmap=pow(lmap,vec4(3.0,3.0,3.0,3.0));"
    "   vec4  fvTotalAmbient   = lmap*(fvAmbient * (fvBaseColor));"
    "   float a = 0.5 + 0.5*dot(fvNormal ,normalize(vec3(0.0,0.5,0.15)));"

    "   vec4   fvTotalDiffuse  =(a  *fvLight1Color*3.0+ (1- a )*vec4(0.0,0.0,0.5,0.0))*fvBaseColor ; "
    "   vec4  fvTotalSpecular  = fNDotL1*fvLight1Color * ( pow( fRDotV1, fSpecularPower ) );"

    "   float heightf=1-clamp(pow(height,0.5),0.0,1.0);"
    "   vec4 color=(lmap*(fvTotalAmbient + fvTotalDiffuse+ (fvTotalSpecular*fSpecularStrength)));"
    "   if(color.r>1.0){color.gb+=color.r-1.0;}"
    "   if(color.g>1.0){color.rb+=color.g-1.0;}"
    "   if(color.b>1.0){color.rg+=color.b-1.0;}"

    "   gl_FragColor= color+(((depth/50000.0)+heightf*depth/10000.0*vec4(1.0,0.5,0.25,1.0))*2.0);"
    "}"
    ;
//
//texture2D( tex1, Texcoord2 )+0.75+;
static stringc armvShader =
    "uniform vec3 fvLightPosition;"
    "uniform vec3 fvEyePosition;"
    "uniform mat4 worldMat;"
    "uniform mat4 mInvTWorld;"
    "varying vec2 Texcoord;"
    "varying vec3 ViewDirection;"
    "varying vec3 LightDirection;"
    "varying vec3 nrm;"
    "varying vec3 nrmr;"
    "varying float depth;"


    "void main( void )"
    "{"
    "  gl_Position = ftransform();"
    "  Texcoord    = gl_MultiTexCoord0.xy;"

    " vec4 fvObjectPosition= gl_ModelViewMatrix * gl_Vertex;"


    "nrmr.xyz =gl_Normal.xyz; "


    " "
    "}";
static stringc armfShader =
    "uniform vec4 fvAmbient;"
    "uniform vec4 fvSpecular;"
    "uniform vec4 fvDiffuse;"
    "uniform float fSpecularPower;"

    "uniform sampler2D tex0;"
    "uniform sampler2D tex1;"
    "uniform sampler2D tex2;"
    "varying vec2 Texcoord;"
    "varying vec3 nrm;"
    "varying vec3 nrmr;"


    "void main( void )"
    "{"

    "vec3  fvNormal         = normalize(nrmr);"
    "float fNDotL           = dot( nrmr, vec3(0.0,1.0,0.5) ); "

    "vec4  fvBaseColor      = texture2D( tex0, Texcoord );"


    "vec4  fvTotalAmbient   = vec4(0.25,0.125,0.0,1.0)* fvBaseColor; "
    "vec4  fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; "

    "float fresnel = pow(1.0-dot( nrmr,vec3(0.0,0.0,1.0)),3.6);"
    "gl_FragColor = vec4(fresnel,fresnel,fresnel,fresnel)+( fvTotalAmbient + fvTotalDiffuse );"

    "}";
static stringc coinvShader =
    "uniform vec3 fvLightPosition;"
    "uniform vec3 fvEyePosition;"

    "varying vec2 Texcoord;"
    "varying vec2 Texcoord2;"
    "varying vec3 ViewDirection;"
    "varying vec3 LightDirection;"

    "varying float depth;"


    "uniform mat4 worldMat;"
    "void main( void )"
    "{"
    "  gl_Position = ftransform();"
    "  Texcoord    = gl_MultiTexCoord0.xy;"

    " vec4 fvObjectPosition= gl_ModelViewMatrix * gl_Vertex;"

    "depth=fvObjectPosition.z;"
    " vec3 fvViewDirection  = fvEyePosition - (worldMat*gl_Vertex).xyz;"
    " vec3 fvLightDirection = fvLightPosition - fvObjectPosition.xyz;"


    "vec3 fvNormal = gl_NormalMatrix * gl_Normal; "

    "vec3 fvTangent = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0.0); "
    "vec3 fvBinormal = cross(fvTangent,gl_Normal); "
    "fvTangent = gl_NormalMatrix * cross(fvBinormal, gl_Normal);"
    "fvBinormal = gl_NormalMatrix * fvBinormal; "

    "ViewDirection.x  = dot( fvTangent, fvViewDirection );"
    "ViewDirection.y  = dot( fvBinormal, fvViewDirection );"
    "ViewDirection.z  = dot( fvNormal, fvViewDirection );"

    "LightDirection.x  = dot( fvTangent, fvLightDirection.xyz );"
    " LightDirection.y  = dot( fvBinormal, fvLightDirection.xyz );"
    "  LightDirection.z  = dot( fvNormal, fvLightDirection.xyz );"
    " "
    "}";
static stringc coinfShader =
    "uniform vec4 fvAmbient;"
    "uniform vec4 fvSpecular;"
    "uniform vec4 fvDiffuse;"
    "uniform float fSpecularPower;"

    "uniform sampler2D tex0;"
    "uniform sampler2D tex1;"
    "uniform sampler2D tex2;"
    "uniform sampler2D tex3;"
    "varying vec2 Texcoord;"

    "varying vec3 ViewDirection;"

    "varying vec3 LightDirection;"
    "varying float height;"
    "varying float depth;"


    "void main( void )"
    "{"
    "vec3  fvLightDirection = normalize( LightDirection );"
    "vec3  fvNormal         = normalize( ( texture2D( tex1, Texcoord ) * 2.0 ) - 1.0 ).xyz;"
    "float fNDotL           = dot( fvNormal, fvLightDirection ); "

    "vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal ) * fNDotL ) - fvLightDirection ); "
    "vec3  fvViewDirection  = normalize( ViewDirection );"
    "float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );"

    "vec4  fvBaseColor      = texture2D( tex0, Texcoord );"
    "vec4  fvRefl      = texture2D( tex2, fvReflection.xy );"



    "gl_FragColor = ((depth/5000.0)*vec4(1.0,0.5,0.25,1.0))+((fvBaseColor*fvRefl)+fNDotL*fvBaseColor );"

    "}";
