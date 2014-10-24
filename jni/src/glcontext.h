#ifndef _GLCONTEXT_H_
#define _GLCONTEXT_H_

void glcontext_initialize();

void glcontext_load_shader();

void glcontext_load_texture();

void glcontext_surface_created();

void glcontext_surface_changed(int width, int height);

void glcontext_draw_frame();

#endif /* _GLCONTEXT_H_ */
