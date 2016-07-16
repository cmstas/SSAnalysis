dir="../all_points_fix/"
lumi="4.0"
# kine="hihi"

mkdir -p points
mkdir -p plots
mkdir -p tables

cp ../printShapes.C .
cp ../printTotalSyst.C .
cp ../makeTable.C .


for kine in hihi hilow lowlow; do
    for thing in data ttw ttzh wz ww xg rares flips fakes; do
        # f1=../v8.02_4p0_newbins/${thing}_histos_${kine}_${lumi}ifb.root
        f1=../all_points_fix/${thing}_histos_${kine}_${lumi}ifb.root
        echo $f1
        cp $f1 points/
    done
done

for kine in hihi hilow lowlow; do
    for mass in 1400_850 1300_500 1400_300; do
        mgl=$(echo $mass | cut -d'_' -f1)
        mlsp=$(echo $mass | cut -d'_' -f2)
        echo "mgl: $mgl, mlsp: $mlsp"
        f1=$dir/fs_t1tttt_m${mgl}_m${mlsp}_histos_${kine}_${lumi}ifb.root
        echo $f1
        cp $f1 points/
    done
done


for mass in 1400_850 1300_500 1400_300; do
    mgl=$(echo $mass | cut -d'_' -f1)
    mlsp=$(echo $mass | cut -d'_' -f2)
    echo "mgl: $mgl, mlsp: $mlsp"
    root -b -q -l -n makeTable.C"(\"fs_t1tttt_m${mgl}_m${mlsp}\")" | drop 2 > tables/table_${mass}.tex
    (cd tables/ ; pdflatex -interaction=nonstopmode table_${mass}.tex; cp table_${mass}.pdf ../plots/)
done


for r in hihi hilow; do
    for p in ttw ttzh ww wz fakes flips xg rares; do
        root -b -q printTotalSyst.C'("'$p'","'$r'","4.0","points")';
    done;
    for mass in 1400_850 1300_500 1400_300; do
        mgl=$(echo $mass | cut -d'_' -f1)
        mlsp=$(echo $mass | cut -d'_' -f2)
        echo "mgl: $mgl, mlsp: $mlsp"
        root -b -q printTotalSyst.C'("'fs_t1tttt_m${mgl}_m${mlsp}'","'$r'","4.0","points")';
    done;
done
cp points/plots/* plots/
