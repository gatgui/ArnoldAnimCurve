import sys
import glob
import excons
from excons.tools import arnold

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
