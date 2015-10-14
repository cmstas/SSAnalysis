import slideMaker as sm
import fnmatch
import os
import utils

#PATH=/nfs-7/texlive/2015/bin/x86_64-linux:$PATH
#export PYTHONPATH=$PYTHONPATH:/home/users/cgeorge/software/slideMaker/

t="madrid"

exprs = {'_mm_OS','_ee_OS','_em_OS','_all_SF','_all_EZ','_all_SS'}
#exprs = {'_all_SF','_all_EZ','_all_SS'}

for expr in exprs:
  sm.initSlides(me="GC",themeName=t,opts="--themecolor 51,51,179")
  for file in os.listdir('.'):
    if fnmatch.fnmatch(file, '*'+expr+'.pdf'):
      print file
      sm.addSlide(p1=file)
  sm.writeSlides("kinem_plot_dump"+expr+".tex",  opts="--compile --copy")
