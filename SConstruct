import sys
import glob
import excons
from excons.tools import arnold

arniver = arnold.Version(asString=False)
if arniver[0] < 4 or (arniver[0] == 4 and (arniver[1] < 2 or (arniver[1] == 2 and arniver[2] < 12))):
  print("agAnimCurve requires at least Arnold 4.2.12.0")
  sys.exit(1)

env = excons.MakeBaseEnv()

prjs = [
  {"name": "agAnimCurve",
   "type": "dynamicmodule",
   "incdirs": ["gmath/include"],
   "defs": ["GMATH_STATIC"],
   "ext": arnold.PluginExt(),
   "srcs": glob.glob("src/*.cpp") + ["gmath/src/lib/curve.cpp",
                                     "gmath/src/lib/polynomial.cpp",
                                     "gmath/src/lib/vector.cpp"],
   "install": {"": "src/agAnimCurve.mtd"},
   "custom": [arnold.Require]
  }
]

excons.DeclareTargets(env, prjs)

Default(["agAnimCurve"])
