#include <ai.h>
#include <cstring>

extern AtNodeMethods *agAnimCurveMtd;

node_loader
{
   if (i == 0)
   {
      node->name = "anim_curve";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_FLOAT;
      node->methods = agAnimCurveMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else
   {
      return false;
   }
}
