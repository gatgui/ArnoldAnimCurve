import sys
import glob
import excons
from excons.tools import arnold

arniver = arnold.Version(asString=False)
if arniver[0] < 4 or (arniver[0] == 4 and (arniver[1] < 2 or (arniver[1] == 2 and arniver[2] < 12))):
  print("Arnold 4.2.12.0 or above required")
  sys.exit(1)

prefix = excons.GetArgument("prefix", "gf_")
name = "%sanim_curve" % prefix

env = excons.MakeBaseEnv()

def ArnoldMtd(target, source, env):
   with open(str(target[0]), "w") as o:
      with open(str(source[0]), "r") as i:
         for l in i.readlines():
            o.write(l.replace("${prefix}", prefix))
   return 0

env["BUILDERS"]["ArnoldMtd"] = Builder(action=ArnoldMtd, suffix=".mtd", src_suffix=".mtd.in")
if sys.platform != "win32":
   env.Append(CPPFLAGS=" -Wno-unused-parameter")
else:
   env.Append(CPPDEFINES=["_CRT_SECURE_NO_WARNINGS"]) # strcpy 
   env.Append(CPPFLAGS=" /wd4100") # unreferenced format parameter

mtd = env.ArnoldMtd("src/%s.mtd" % name, "src/anim_curve.mtd.in")

prjs = [
  {"name": name,
   "type": "dynamicmodule",
   "prefix": "arnold",
   "incdirs": ["gmath/include"],
   "defs": ["GMATH_STATIC", "PREFIX=\\\"%s\\\"" % prefix],
   "ext": arnold.PluginExt(),
   "srcs": glob.glob("src/*.cpp") + ["gmath/src/lib/curve.cpp",
                                     "gmath/src/lib/polynomial.cpp",
                                     "gmath/src/lib/vector.cpp"],
   "install": {"arnold": mtd},
   "custom": [arnold.Require]
  }
]

excons.DeclareTargets(env, prjs)

excons.EcosystemDist(env, "anim_curve.env", {name: ""}, name=name)

Default([name])
