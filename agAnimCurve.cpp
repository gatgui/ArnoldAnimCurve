#include <ai.h>
#include <gmath/curve.h>

AI_SHADER_NODE_EXPORT_METHODS(agAnimCurveMtd);

enum AnimCurveParams
{
   p_input = 0,
   p_input_is_frame,
   p_positions,
   p_values,
   p_interpolations,
   p_in_tangents,
   p_in_weights,
   p_out_tangents,
   p_out_weights,
   p_default_interpolation,
   p_pre_infinity,
   p_post_infinity,
};

enum Interpolation
{
   Interp_Step = 0,
   Interp_Linear,
   Interp_Spline
};

static const char* InterpolationNames[] =
{
   "step",
   "linear",
   "spline"
};

enum Infinity
{
   Inf_Constant = 0,
   Inf_Linear,
   Inf_Loop,
   Inf_LoopOffset,
   Inf_Mirror
};

static const char* InfinityNames[] =
{
   "constant",
   "linear",
   "loop",
   "loop_offset",
   "mirror"
};

struct NodeData
{
   gmath::TCurve<float> *curve;
   AtArray *samples;
};

node_parameters
{
   AiParameterFlt("input", 0.0f);
   AiParameterBool("input_is_frame", true); // disregard input param and use options "frame" as input
   AiParameterArray("positions", AiArrayAllocate(0, 0, AI_TYPE_FLOAT));
   AiParameterArray("values", AiArrayAllocate(0, 0, AI_TYPE_FLOAT));
   AiParameterArray("interpolations", AiArrayAllocate(0, 0, AI_TYPE_ENUM)); // can be empty
   AiParameterArray("in_tangents", AiArrayAllocate(0, 0, AI_TYPE_FLOAT)); // interpret as 'slope' -> dy/dx
   AiParameterArray("in_weights", AiArrayAllocate(0, 0, AI_TYPE_FLOAT));  // can be empty
   AiParameterArray("out_tangents", AiArrayAllocate(0, 0, AI_TYPE_FLOAT)); // interpret as 'slope' -> dy/dx
   AiParameterArray("out_weights", AiArrayAllocate(0, 0, AI_TYPE_FLOAT));  // can be empty
   AiParameterEnum("default_interpolation", 0, InterpolationNames);
   AiParameterEnum("pre_infinity", 0, InfinityNames);
   AiParameterEnum("post_infinity", 0, InfinityNames);

   AiMetaDataSetBool(mds, "input_is_frame", "linkable", false);
   AiMetaDataSetBool(mds, "positions", "linkable", false);
   AiMetaDataSetBool(mds, "values", "linkable", false);
   AiMetaDataSetBool(mds, "interpolations", "linkable", false);
   AiMetaDataSetBool(mds, "in_tangents", "linkable", false);
   AiMetaDataSetBool(mds, "in_weights", "linkable", false);
   AiMetaDataSetBool(mds, "out_tangents", "linkable", false);
   AiMetaDataSetBool(mds, "out_weights", "linkable", false);
   AiMetaDataSetBool(mds, "default_interpolation", "linkable", false);
   AiMetaDataSetBool(mds, "pre_infinity", "linkable", false);
   AiMetaDataSetBool(mds, "post_infinity", "linkable", false);

}

node_initialize
{
   NodeData *data = (NodeData*) AiMalloc(sizeof(NodeData));
   data->curve = 0;
   data->samples = 0;
   AiNodeSetLocalData(node, data);
}

node_update
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);
   
   if (data->samples)
   {
      AiArrayDestroy(data->samples);
      data->samples = 0;
   }
   
   bool weighted = false;

   AtArray *p = AiNodeGetArray(node, "positions");
   AtArray *v = AiNodeGetArray(node, "values");
   AtArray *i = AiNodeGetArray(node, "interpolations");
   AtArray *is = AiNodeGetArray(node, "in_tangents");
   AtArray *iw = AiNodeGetArray(node, "in_weights");
   AtArray *os = AiNodeGetArray(node, "out_tangents");
   AtArray *ow = AiNodeGetArray(node, "out_weights");

   Interpolation defi = (Interpolation) AiNodeGetInt(node, "default_interpolation");

   if (p->nelements != v->nelements ||
       p->nelements != is->nelements ||
       p->nelements != os->nelements)
   {
      AiMsgWarning("[agAnimCurve] All input arrays must be of the same length");
      if (data->curve)
      {
         delete data->curve;
         data->curve = 0;
      }
   }
   else if (p->type != AI_TYPE_FLOAT ||
            v->type != AI_TYPE_FLOAT ||
            is->type != AI_TYPE_FLOAT ||
            os->type != AI_TYPE_FLOAT)
   {
      AiMsgWarning("[agAnimCurve] All input arrays must be of type float");
      if (data->curve)
      {
         delete data->curve;
         data->curve = 0;
      }
   }
   else
   {
      bool has_interpolations = false;

      if (i->nelements > 0)
      {
         if (i->nelements == p->nelements && (i->type != AI_TYPE_ENUM || i->type != AI_TYPE_INT))
         {
            has_interpolations = true;
         }
         else
         {
            AiMsgWarning("[agAnimCurve] Invalid interpolations type and/or size. Use 'default_interpolation'");
         }
      }

      if (iw->nelements > 0)
      {
         if (iw->nelements != p->nelements || iw->type != AI_TYPE_FLOAT)
         {
            AiMsgWarning("[agAnimCurve] Invalid input weights type and/or size. Evaluate as non-weigted");
         }
         else if (ow->nelements != p->nelements || ow->type != AI_TYPE_FLOAT)
         {
            AiMsgWarning("[agAnimCurve] Invalid output weights type and/or size. Evaluate as non-weigted");
         }
         else
         {
            weighted = true;
         }
      }

      if (!data->curve)
      {
         data->curve = new gmath::TCurve<float>();
         data->curve->setWeighted(weighted);
      }

      data->curve->removeAll();

      data->curve->setPreInfinity((gmath::Curve::Infinity) AiNodeGetInt(node, "pre_infinity"));
      data->curve->setPostInfinity((gmath::Curve::Infinity) AiNodeGetInt(node, "post_infinity"));

      for (unsigned int k=0; k<p->nelements; ++k)
      {
         size_t idx = data->curve->insert(AiArrayGetFlt(p, k), AiArrayGetFlt(v, k));
         data->curve->setInterpolation(idx, (gmath::Curve::Interpolation)(has_interpolations ? AiArrayGetInt(i, k) : defi));
         data->curve->setInTangent(idx, gmath::Curve::T_CUSTOM, AiArrayGetFlt(is, k));
         data->curve->setOutTangent(idx, gmath::Curve::T_CUSTOM, AiArrayGetFlt(os, k));
         if (weighted)
         {
            data->curve->setInWeight(idx, AiArrayGetFlt(iw, k));
            data->curve->setOutWeight(idx, AiArrayGetFlt(ow, k));
         }
      }      
   }
   
   // Bake curve samples
   if (data->curve && AiNodeGetBool(node, "input_is_frame"))
   {
      AtNode *opts = AiUniverseGetOptions();
      if (AiNodeLookUpUserParameter(opts, "frame") != 0)
      {
         float frame = AiNodeGetFlt(opts, "frame");
         float mstart = frame;
         float mend = frame;
         float fincr = 0.0f;
         int steps = 1;
         int step = 0;
         
         if (AiNodeLookUpUserParameter(opts, "motion_start_frame") != 0 &&
             AiNodeLookUpUserParameter(opts, "motion_end_frame") != 0)
         {
            mstart = AiNodeGetFlt(opts, "motion_start_frame");
            mend = AiNodeGetFlt(opts, "motion_end_frame");
            
            if (mstart > mend || frame < mstart || frame > mend)
            {
               AiMsgWarning("[agAnimCurve] Invalid values \"motion_start_frame\" and \"motion_end_frame\" parameters on options node. Default both to %f", frame);
               mstart = frame;
               mend = frame;
            }
            
            if (mend > mstart)
            {
               steps = 2;
               if (frame > mstart && frame < mend)
               {
                  steps = 3;
               }
            }
            
            if (AiNodeLookUpUserParameter(opts, "motion_steps") != 0)
            {
               int tmp = AiNodeGetInt(opts, "motions_steps");
               if (tmp > 0)
               {
                  steps = tmp;
               }
               else
               {
                  AiMsgWarning("[agAnimCurve] Invalid value for \"motion_steps\" parameter on options node. Default to %d", steps);
               }
            }
            else
            {
               AiMsgWarning("[agAnimCurve] No \"motion_steps\" parameter on options node. Default to %d", steps);
            }
            
            fincr = mend - mstart;
            if (steps > 1)
            {
               fincr /= float(steps - 1);
            }
            
            data->samples = AiArrayAllocate(1, steps, AI_TYPE_FLOAT);
            
            for (float f=mstart; f<=mend; f+=fincr, ++step)
            {
               AiArraySetFlt(data->samples, step, data->curve->eval(f));
            }
         }
         else
         {
            AiMsgWarning("[agAnimCurve] No \"motion_start_frame\" and/or \"motion_end_frame\" parameters on options node. Bake a single sample");
         }
         // bake samples
      }
      else
      {
         AiMsgWarning("[agAnimCurve] No \"frame\" parameter on options node. Cannot bake curve samples");
      }
   }
}

node_finish
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);

   if (data->curve)
   {
      delete data->curve;
   }
   if (data->samples)
   {
      AiArrayDestroy(data->samples);
   }
   AiFree(data);
}

shader_evaluate
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);
   
   float rv = 0.0f;

   if (data->samples)
   {
      rv = AiArrayInterpolateFlt(data->samples, sg->time, 0);
   }
   else if (data->curve)
   {
      rv = data->curve->eval(AiShaderEvalParamFlt(p_input));
   }

   sg->out.FLT = rv;
}

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