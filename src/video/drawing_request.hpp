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

#ifndef HEADER_SUPERTUX_VIDEO_DRAWING_REQUEST_HPP
#define HEADER_SUPERTUX_VIDEO_DRAWING_REQUEST_HPP

#include <memory>
#include <string>
#include <vector>

#include <stdint.h>

#include "math/rectf.hpp"
#include "math/sizef.hpp"
#include "math/vector.hpp"
#include "video/color.hpp"
#include "video/drawing_context.hpp"
#include "video/font.hpp"
#include "video/glutil.hpp"

#ifdef HAVE_OPENGL
#include "gl/gl_texture.hpp"
#endif

class Surface;

enum RequestType
{
  SURFACE, SURFACE_PART, TEXT, GRADIENT, FILLRECT, INVERSEELLIPSE, DRAW_LIGHTMAP, GETLIGHT, LINE, TRIANGLE
};

struct DrawingRequestData
{
  virtual bool has_surface() { return false; }
  
  virtual ~DrawingRequestData()
  {}
};

struct TexturedDrawingRequestData : public DrawingRequestData
{
  virtual bool has_surface() override { return true; }
  
  TexturedDrawingRequestData() :
    surface()
  {}
  
  const Surface* surface;

private:
  TexturedDrawingRequestData(const TexturedDrawingRequestData&) = delete;
  TexturedDrawingRequestData& operator=(const TexturedDrawingRequestData&) = delete;
};

struct SurfaceRequest : public TexturedDrawingRequestData
{
  SurfaceRequest()
  {}

private:
  SurfaceRequest(const SurfaceRequest&) = delete;
  SurfaceRequest& operator=(const SurfaceRequest&) = delete;
};

struct SurfacePartRequest : public TexturedDrawingRequestData
{
  SurfacePartRequest() :
    srcrect(),
    dstsize()
  {}

  Rectf srcrect;
  Sizef dstsize;

private:
  SurfacePartRequest(const SurfacePartRequest&) = delete;
  SurfacePartRequest& operator=(const SurfacePartRequest&) = delete;
};

struct TextRequest : public DrawingRequestData
{
  TextRequest() :
    font(),
    text(),
    alignment()
  {}

  const Font* font;
  std::string text;
  FontAlignment alignment;

private:
  TextRequest(const TextRequest&);
  TextRequest& operator=(const TextRequest&);
};

struct GradientRequest : public DrawingRequestData
{
  GradientRequest()  :
    top(),
    bottom(),
    size(),
    direction(),
    region()
  {}

  Color top;
  Color bottom;
  Vector size;
  GradientDirection direction;
  Rectf region;
};

struct FillRectRequest : public DrawingRequestData
{
  FillRectRequest() :
    color(),
    size(),
    radius()
  {}

  Color  color;
  Vector size;
  float  radius;
};

struct InverseEllipseRequest : public DrawingRequestData
{
  InverseEllipseRequest() :
    color(),
    size()
  {}

  Color  color;
  Vector size;
};

struct LineRequest : public DrawingRequestData
{
  LineRequest() :
    color(),
    dest_pos()
  {}

  Color  color;
  Vector dest_pos;
};

struct TriangleRequest : public DrawingRequestData
{
  TriangleRequest() :
    color(),
    pos2(),
    pos3()
  {}

  Color  color;
  Vector pos2, pos3;
};

struct DrawingRequest
{
  Target target;
  RequestType type;
  Vector pos;

  int layer;
  DrawingEffect drawing_effect;
  float alpha;
  Blend blend;
  float angle;
  Color color;

  DrawingRequestData* request_data;

  DrawingRequest() :
    target(),
    type(),
    pos(),
    layer(),
    drawing_effect(),
    alpha(),
    blend(),
    angle(0.0f),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    request_data()
  {}
  
  bool operator<(const DrawingRequest& other) const
  {
#ifdef HAVE_OPENGL
    if (request_data && request_data->has_surface() && other.request_data && other.request_data->has_surface())
      return (layer < other.layer) || ((layer == other.layer) && (static_cast<GLTexture&>(*reinterpret_cast<TexturedDrawingRequestData*>(request_data)->surface->get_texture()) < static_cast<GLTexture&>(*reinterpret_cast<TexturedDrawingRequestData*>(other.request_data)->surface->get_texture())));
    else
#endif
      return layer < other.layer;
  }
};

struct GetLightRequest : public DrawingRequestData
{
  GetLightRequest() : color_ptr() {}

  Color* color_ptr;

private:
  GetLightRequest(const GetLightRequest&) = delete;
  GetLightRequest& operator=(const GetLightRequest&) = delete;
};

#endif

/* EOF */
