//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "video/gl/gl_lightmap.hpp"

#include <SDL_image.h>
#include <algorithm>
#include <assert.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <physfs.h>
#include <sstream>

#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "util/obstackpp.hpp"
#include "video/drawing_context.hpp"
#include "video/drawing_request.hpp"
#include "video/font.hpp"
#include "video/gl/gl_painter.hpp"
#include "video/gl/gl_renderer.hpp"
#include "video/gl/gl_surface_data.hpp"
#include "video/gl/gl_texture.hpp"
#include "video/glutil.hpp"
#include "video/lightmap.hpp"
#include "video/renderer.hpp"
#include "video/surface.hpp"
#include "video/texture_manager.hpp"

#include "util/log.hpp"
#ifdef USE_GLBINDING
#include <glbinding/gl/gl.h>
#else
#include <GL/glew.h>
#endif

inline int next_po2(int val)
{
  int result = 1;
  while(result < val)
    result *= 2;

  return result;
}

GLLightmap::GLLightmap() :
  m_lightmap(),
  m_lightmap_width(),
  m_lightmap_height(),
  m_lightmap_uv_right(),
  m_lightmap_uv_bottom(),
  fbo(0)
{
  m_lightmap_width = SCREEN_WIDTH / s_LIGHTMAP_DIV;
  m_lightmap_height = SCREEN_HEIGHT / s_LIGHTMAP_DIV;
  unsigned int width = next_po2(m_lightmap_width);
  unsigned int height = next_po2(m_lightmap_height);

  m_lightmap.reset(new GLTexture(width, height));

  m_lightmap_uv_right = static_cast<float>(m_lightmap_width) / static_cast<float>(width);
  m_lightmap_uv_bottom = static_cast<float>(m_lightmap_height) / static_cast<float>(height);
  TextureManager::current()->register_texture(m_lightmap.get());

  glGenFramebuffers(1, &fbo);
  
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightmap->get_handle(), 0);
  
  GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (err != GL_FRAMEBUFFER_COMPLETE) {
    log_warning << "Couldn't create FBO: " << err << "\n";
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLLightmap::~GLLightmap()
{
  glDeleteFramebuffers(1, &fbo);
}

void
GLLightmap::start_draw(const Color &ambient_color)
{
  glGetFloatv(GL_VIEWPORT, m_old_viewport); //save viewport
#if 0
  glViewport(m_old_viewport[0], m_old_viewport[3] - m_lightmap_height + m_old_viewport[1], m_lightmap_width, m_lightmap_height);
#else
  // Using an FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, m_lightmap_width, m_lightmap_height);
#endif
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#if 0
#ifdef GL_VERSION_ES_CM_1_0
  glOrthof(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0);
#else
  glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0);
#endif
#else
#ifdef GL_VERSION_ES_CM_1_0
  glOrthof(m_old_viewport[0], SCREEN_WIDTH, SCREEN_HEIGHT, m_old_viewport[1], -1.0, 1.0);
#else
  glOrtho(m_old_viewport[0], SCREEN_WIDTH, SCREEN_HEIGHT, m_old_viewport[1], -1.0, 1.0);
#endif
#endif
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor( ambient_color.red, ambient_color.green, ambient_color.blue, 1 );
  glClear(GL_COLOR_BUFFER_BIT);
}

void
GLLightmap::end_draw()
{
  glDisable(GL_BLEND);
#if 0
  glBindTexture(GL_TEXTURE_2D, m_lightmap->get_handle());
  glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_old_viewport[0], m_old_viewport[3] - m_lightmap_height + m_old_viewport[1], m_lightmap_width, m_lightmap_height);
#else
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, m_lightmap->get_handle());
#endif

  glViewport(m_old_viewport[0], m_old_viewport[1], m_old_viewport[2], m_old_viewport[3]);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#ifdef GL_VERSION_ES_CM_1_0
  glOrthof(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0);
#else
  glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0);
#endif
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_BLEND);

  glClearColor(0, 0, 0, 1 );
  glClear(GL_COLOR_BUFFER_BIT);
}

void
GLLightmap::do_draw()
{
  // multiple the lightmap with the framebuffer
  glBlendFunc(GL_DST_COLOR, GL_ZERO);

  glBindTexture(GL_TEXTURE_2D, m_lightmap->get_handle());

  float vertices[] = {
    0, 0,
    float(SCREEN_WIDTH), 0,
    float(SCREEN_WIDTH), float(SCREEN_HEIGHT),
    0, float(SCREEN_HEIGHT)
  };
  glVertexPointer(2, GL_FLOAT, 0, vertices);

  float uvs[] = {
    0,                 m_lightmap_uv_bottom,
    m_lightmap_uv_right, m_lightmap_uv_bottom,
    m_lightmap_uv_right, 0,
    0, 0
  };
  glTexCoordPointer(2, GL_FLOAT, 0, uvs);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
GLLightmap::draw_surface(const DrawingRequest& request)
{
  GLPainter::draw_surface(request);
}

void
GLLightmap::draw_surface_part(const DrawingRequest& request)
{
  GLPainter::draw_surface_part(request);
}

void
GLLightmap::draw_gradient(const DrawingRequest& request)
{
  GLPainter::draw_gradient(request);
}

void
GLLightmap::draw_filled_rect(const DrawingRequest& request)
{
  GLPainter::draw_filled_rect(request);
}

void
GLLightmap::draw_inverse_ellipse(const DrawingRequest& request)
{
  GLPainter::draw_inverse_ellipse(request);
}

void
GLLightmap::draw_line(const DrawingRequest& request)
{
  GLPainter::draw_line(request);
}

void
GLLightmap::draw_triangle(const DrawingRequest& request)
{
  GLPainter::draw_triangle(request);
}

void
GLLightmap::get_light(const DrawingRequest& request) const
{
  const GetLightRequest* getlightrequest
    = static_cast<GetLightRequest*>(request.request_data);

  float pixels[3];
  for( int i = 0; i<3; i++)
    pixels[i] = 0.0f; //set to black

  float posX = request.pos.x * m_lightmap_width / SCREEN_WIDTH + m_old_viewport[0];
  float posY = m_old_viewport[3] + m_old_viewport[1] - request.pos.y * m_lightmap_height / SCREEN_HEIGHT;
  glReadPixels((GLint) posX, (GLint) posY , 1, 1, GL_RGB, GL_FLOAT, pixels);
  *(getlightrequest->color_ptr) = Color( pixels[0], pixels[1], pixels[2]);
}

/* EOF */
