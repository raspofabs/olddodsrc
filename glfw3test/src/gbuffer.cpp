#include "gbuffer.h"
#include <stdio.h>
#include "win32/glhack.h"

#define ARRAY_SIZE_IN_ELEMENTS(X) (sizeof(X)/sizeof(X[0]))
GBuffer::GBuffer() {
	m_fbo = 0;
	for( int i = 0; i < GBUFFER_NUM_TEXTURES; ++i ) {
		m_textures[i] = 0;
	}
	m_depthTexture = 0;
}
GBuffer::~GBuffer() {
	//m_fbo;
	//m_textures[GBUFFER_NUM_TEXTURES];
	//m_depthTexture;
}

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, &m_fbo); 
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
    glGenTextures(1, &m_depthTexture);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures) ; i++) {
       glBindTexture(GL_TEXTURE_2D, m_textures[i]);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
       glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                  NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 }; 
    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}


void GBuffer::BindForWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindForReading() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType) {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}
