#include "LegEngine.h"
#include <fstream>

char* GetFragmentShaderSource(char* filename)
{
char dlldir[MAX_PATH];
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

char* GetVertexShaderSource(char* filename)
{
char dlldir[MAX_PATH];
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

char *textFileRead(char *fn) 
	{
		FILE *fp;
		char *content = NULL;

		int count=0;

		if(fn == NULL) DebugLog("Failed to find shader");

		if (fn != NULL) {

			fp = fopen(fn,"rt");

			if (fp != NULL) {
										      
        	      		fseek(fp, 0, SEEK_END);
        			count = ftell(fp);
        			rewind(fp);

				if (count > 0) {
					content = (char *)malloc(sizeof(char) * (count+1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
										
			}
		}
	
		return content;
	}



void UseShaders(void)
{
	char *vertexshader,*fragmentshader;

	GLenum shader_vert = 0;
	GLenum shader_frag = 0;
	GLenum shader_prog = 0;
	
	shader_vert = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	shader_frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

	vertexshader = textFileRead("GLSL/VertexShader.c");
	fragmentshader = textFileRead("GLSL/FragmentShader.c");

	const char * vv = vertexshader;
	const char * ff = fragmentshader;

	glShaderSourceARB(shader_vert, 1, &vv,NULL);
	glShaderSourceARB(shader_frag, 1, &ff,NULL);

	free(vertexshader);
	free(fragmentshader);

	glCompileShaderARB(shader_vert);
	glCompileShaderARB(shader_frag);

	shader_prog = glCreateProgramObjectARB();

	glAttachObjectARB(shader_prog,shader_vert);
	glAttachObjectARB(shader_prog,shader_frag);

	glLinkProgramARB(shader_prog);

	glUseProgramObjectARB(shader_prog);

}

void SetLighting()
{
glEnable(GL_LIGHTING); 
}