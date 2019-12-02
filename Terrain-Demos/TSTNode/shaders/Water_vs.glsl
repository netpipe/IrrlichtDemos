/*
 * Copyright (c) 2007, elvman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY elvman ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

uniform mat4	View;
uniform mat4	WorldViewProj;  // World * View * Projection transformation
uniform mat4	WorldReflectionViewProj;  // World * Reflection View * Projection transformation

uniform float	WaveLength;

uniform float	Time;
uniform float	WindForce;
uniform vec2	WindDirection;

void main(void)
{	
	// transform position to clip space
	gl_Position = gl_Vertex * WorldViewProj;

	// calculate vawe coords
	gl_TexCoord[0] = gl_MultiTexCoord0/WaveLength + vec4(Time*WindForce*WindDirection,0,0);
	
	gl_TexCoord[1] = gl_Vertex * WorldViewProj; //refraction texcoords
	gl_TexCoord[2] = gl_Vertex * WorldReflectionViewProj; //reflection texcoords
	
	// position of the pixel
	gl_TexCoord[3] = gl_Vertex * View;
}