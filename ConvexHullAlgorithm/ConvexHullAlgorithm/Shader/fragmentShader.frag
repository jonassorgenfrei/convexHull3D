#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}