
lumi=6.3
dir=v8.03
for p in ttw ttzh; do for s in ${p}_extr_njh ${p}_extr_njl ${p}_extr_hth ${p}_extr_ll btag jes pu ${p}_stat; do root -b -q printShapes.C'("'$p'","hihi","'$lumi'","sr","'$s'","'$dir'")'; done; done

for p in ww wz xg rares; do for s in btag jes pu ${p}_stat; do root -b -q printShapes.C'("'$p'","hihi","'$lumi'","sr","'$s'","'$dir'")'; done; done

for p in fakes; do for s in fakes_stat fakes_EWK; do root -b -q printShapes.C'("'$p'","hihi","'$lumi'","sr","'$s'","'$dir'")'; done; done

for p in flips; do for s in flips_stat; do root -b -q printShapes.C'("'$p'","hihi","'$lumi'","sr","'$s'","'$dir'")'; done; done

