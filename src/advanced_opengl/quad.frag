#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void inverse()
{
	FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}

void grayscale()
{
	FragColor = texture(screenTexture, TexCoords);
	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	FragColor = vec4(average, average, average, 1.0);
}

void weighted_grayscale()
{
	FragColor = texture(screenTexture, TexCoords);
	float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	FragColor = vec4(average, average, average, 1.0);
}

void kernel_effect(float kernel[9])
{
	const float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), // top-left
		vec2( 0.0f, offset), // top-center
		vec2( offset, offset), // top-right
		vec2(-offset, 0.0f),
		// center-left
		vec2( 0.0f, 0.0f),
		// center-center
		vec2( offset, 0.0f),
		// center-right
		vec2(-offset, -offset), // bottom-left
		vec2( 0.0f, -offset), // bottom-center
		vec2( offset, -offset) // bottom-right
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];
	FragColor = vec4(col, 1.0);
}

void sharpen()
{
	float kernel[9] = float[](
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);

	kernel_effect(kernel);
}

void blur()
{
	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	kernel_effect(kernel);
}

void edge_detection()
{
	float kernel[9] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
	);

	kernel_effect(kernel);
}

void main()
{
	// default
	FragColor = texture(screenTexture, TexCoords);
}
