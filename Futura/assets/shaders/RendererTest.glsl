#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_LightmapTexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_AlbedoColor;
uniform int u_HasTexture;

void main()
{
	vec4 textureColor = u_HasTexture != 0 ? texture(u_Texture, v_TexCoord) : vec4(1.0);
	o_Color = textureColor * u_AlbedoColor;
}
