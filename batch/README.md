#How to make SS Babies

Instructions (no quotes):
  1.  ". copy.sh" to copy files from the parent directory.  Lots of other changes happen here too.
  2.  ". makeAllBabies.sh" to make all the babies.  Just let it run, it will resubmit everything that fails automatically and quit when everything is done.
  3.  ". mergeAll.sh" to merge all the babies.  

Some technical details:
  -  These jobs run using xrootd at any site nationally.  The sites can be specified in condorFileTemplate.  
  -  submitall.sh is the file that does most of the submissions.  If you want to modify what is produced (add or remove samples, etc.), go there.  
  -  On the worker node, condorExecutable sets the environment and calls do.C, which compiles everything and calls run.C, which loops over the event and calls helper_babymaker once per event (this is a weird setup, but designed to be compatible with the parent directory).  
