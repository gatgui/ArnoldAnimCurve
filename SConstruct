import sys
import glob
import excons
from excons.tools import arnold

ARGUMENTS["static"] = "1"
env = excons.MakeBaseEnv()

SConscript("gmath/SConstruct")

prjs = [
  {"name": "agAnimCurve",
   "type": "dynamicmodule",
   "incdirs": ["gmath/include"],
   "defs": ["GMATH_STATIC"],
   "ext": arnold.PluginExt(),
   "srcs": glob.glob("*.cpp"),
   "libs": ["gmath"],
   "custom": [arnold.Require]
  }
]

excons.DeclareTargets(env, prjs)

Default(["agAnimCurve"])
