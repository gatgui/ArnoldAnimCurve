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
