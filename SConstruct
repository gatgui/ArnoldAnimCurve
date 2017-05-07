import sys
import glob
import excons
import excons.config
from excons.tools import arnold

env = excons.MakeBaseEnv()

arniver = arnold.Version(asString=False)
if arniver[0] < 4 or (arniver[0] == 4 and (arniver[1] < 2 or (arniver[1] == 2 and arniver[2] < 12))):
  print("Arnold 4.2.12.0 or above required")
  sys.exit(1)

prefix = excons.GetArgument("prefix", "gf_")
name = "%sanim_curve" % prefix
spl = name.split("_")
maya_name = spl[0] + "".join(map(lambda x: x[0].upper() + x[1:], spl[1:]))
opts = {"PREFIX": prefix, "MAYA_NODENAME": maya_name}

GenerateMtd = excons.config.AddGenerator(env, "mtd", opts)
GenerateMayaAE = excons.config.AddGenerator(env, "mayaAE", opts)
mtd = GenerateMtd("src/%s.mtd" % name, "src/anim_curve.mtd.in")
ae = GenerateMayaAE("maya/%sTemplate.py" % maya_name, "maya/AnimCurveTemplate.py.in")

if sys.platform != "win32":
   env.Append(CPPFLAGS=" -Wno-unused-parameter")
else:
   env.Append(CPPFLAGS=" /wd4100") # unreferenced format parameter

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
   "install": {"arnold": mtd,
               "maya/ae": ae},
   "custom": [arnold.Require]
  }
]

targets = excons.DeclareTargets(env, prjs)

targets[name].extend(mtd)
targets["maya"] = ae

excons.EcosystemDist(env, "anim_curve.env", {name: "", "maya": "/maya/ae"}, name=name)

Default([name])
