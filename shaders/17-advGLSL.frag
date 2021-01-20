#version 330 core

out vec4 fragColor;

uniform int showcaseMode;

void main()
{
	switch(showcaseMode) {
		case 0:
			fragColor = vec4(0.5, 0.2, 0.35, 1.0);
			break;

		case 1:
			if (gl_FragCoord.x < 600) {
				fragColor = vec4(1.0, 0.0, 0.0, 1.0);
			}
			else if (gl_FragCoord.x < 1200) {
				fragColor = vec4(0.0, 1.0, 0.0, 1.0);
			}
			else if (gl_FragCoord.x < 1800) {
				fragColor = vec4(0.0, 0.0, 1.0, 1.0);
			}
			else  {
				fragColor = vec4(1.0, 1.0, 1.0, 1.0);
			}
			if (gl_FragCoord.y > 540) {
				fragColor.rgb = vec3(1.0, 1.0, 1.0) - fragColor.rgb;
			}
			break;

		case 2:
			if(gl_FrontFacing){
				fragColor = vec4(0.1, 0.5, 0.35, 1.0);
			}
			else {
				fragColor = vec4(0.4, 0.4, 0.1, 1.0);
			}
	}
}