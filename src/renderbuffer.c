#include "graphics.h"

void cg_renderbuffer_create(RenderBuffer* r) {
    glGenRenderbuffers(1, &r->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, r->rbo);
}

void cg_renderbuffer_bind(RenderBuffer* r) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, CG_SCREEN_X, CG_SCREEN_Y); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, r->rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Error framebuffer is not complete\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cg_renderbuffer_destroy(RenderBuffer* r) {
    glDeleteBuffers(1, &r->rbo);
}
