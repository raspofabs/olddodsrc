#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "gbuffer.h"
#include "win32\glhack.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
extern float NewValue( float in );
int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
		GBuffer *gb = 0;

		double lastTime = 0.0;
		double frameTime = 0.1;
    while (!glfwWindowShouldClose(window))
    {
			double newTime = glfwGetTime();
			double diff = newTime - lastTime;
			if( diff < frameTime ) {
				double waitTime = frameTime - diff;
				sleep_ms(waitTime * 1000);
			}
			lastTime = glfwGetTime();

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
				if( !gb ) {
					gb = new GBuffer();
					gb->Init(width,height);
				}
				gb->BindForWriting();
				gb->BindForReading();

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(NewValue( (float) glfwGetTime() * 50.f), 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
