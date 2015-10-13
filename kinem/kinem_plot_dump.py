import slideMaker as sm
import fnmatch
import os
import utils

#export PYTHONPATH=$PYTHONPATH:/home/users/cgeorge/software/slideMaker/

t="alex"

sm.initSlides(me="Alex",themeName=t,opts="--themecolor 51,51,179")

for file in os.listdir('.'):
  if fnmatch.fnmatch(file, '*.pdf'):
    print file
    sm.addSlide(p1=file)

sm.writeSlides("kinem_plot_dump.tex",  opts="--compile --copy")
