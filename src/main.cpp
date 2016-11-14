/*
MIT License

Copyright (c) 2016 Gaetan Guidet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <ai.h>
#include <cstring>

#ifndef PREFIX
#  define PREFIX ""
#endif

extern AtNodeMethods *AnimCurveMtd;

namespace SSTR
{
   AtString input("input");
   AtString input_is_frame_offset("input_is_frame_offset");
   AtString frame("frame");
   AtString linkable("linkable");
   AtString motion_start_frame("motion_start_frame");
   AtString motion_end_frame("motion_end_frame");
   AtString relative_motion_frame("relative_motion_frame");
   AtString motion_steps("motion_steps");
   AtString positions("positions");
   AtString values("values");
   AtString interpolations("interpolations");
   AtString in_tangents("in_tangents");
   AtString in_weights("in_weights");
   AtString out_tangents("out_tangents");
   AtString out_weights("out_weights");
   AtString default_interpolation("default_interpolation");
   AtString pre_infinity("pre_infinity");
   AtString post_infinity("post_infinity");
}

node_loader
{
   if (i == 0)
   {
      node->name = PREFIX "anim_curve";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_FLOAT;
      node->methods = AnimCurveMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else
   {
      return false;
   }
}
