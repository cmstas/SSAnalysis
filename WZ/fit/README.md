* We can derive a WZ and ttZ SF simultaneously using the nbtags distribution, so

1. Run the WZ CR script in `../` and copy the `h1D_nbtags.root` file into this directory. Note that Nbtags==0 should be relaxed, and Njets should be fully relaxed (except for baseline selection)
2. `. install.sh` to use an older HiggsCombine because someone broke it and now all I get is segfaults :((((((
3. `python fit.py` to get the plots
4. Put into analysis and get profit
