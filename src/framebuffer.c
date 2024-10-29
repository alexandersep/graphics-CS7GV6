#include "graphics.h"

void cg_framebuffer_create(FrameBuffer* f) {
    glGenFramebuffers(1, &f->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, f->fbo);
}

void cg_framebuffer_texture_attach(FrameBuffer* f, Texture* tex) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
}

void cg_framebuffer_bind(FrameBuffer* f) {
    glBindFramebuffer(GL_FRAMEBUFFER, f->fbo);
}

void cg_framebuffer_default_bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cg_framebuffer_destroy(FrameBuffer* f) {
    glDeleteBuffers(1, &f->fbo);
}
