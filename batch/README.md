#How to make SS Babies

Instructions (no quotes):
  1.  ". copy.sh" to copy files from the parent directory.  Lots of other changes happen here too.
  2.  ". submit25ns.sh" to make the babies.  Modify the file to decide which babies get made. 
  3.  ". mergeNew.sh" to merge all the babies.  

Some technical details:
  -  These jobs run using xrootd at any site nationally.  The sites can be specified in condorFileTemplate.  If xrootd is broken, toggle the flag in submit25ns.sh.
  -  You will not be able to "make" locally without copying main.C to be main.cc.  
