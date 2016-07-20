# dir="../all_points_fix/"
# lumi="4.0"
dir="../v8.03_fort5qqqqvv/"
lumi="6.3"
sig="fs_t5qqqqvv"
points="1100_300 1200_300"

mkdir -p points/plots/
mkdir -p plots
mkdir -p tables

cp ../printShapes.C .
cp ../printTotalSyst.C .
# cp ../makeTable.C . # don't use regular because I needed to modify it slightly


# for kine in hihi hilow lowlow; do
#     for thing in data ttw ttzh wz ww xg rares flips fakes; do
#         f1=${dir}/${thing}_histos_${kine}_${lumi}ifb.root
#         echo $f1
#         cp $f1 points/
#     done
# done

# for kine in hihi hilow lowlow; do
#     for mass in $points; do
#         mgl=$(echo $mass | cut -d'_' -f1)
#         mlsp=$(echo $mass | cut -d'_' -f2)
#         echo "mgl: $mgl, mlsp: $mlsp"
#         f1=$dir/${sig}_m${mgl}_m${mlsp}_histos_${kine}_${lumi}ifb.root
#         echo $f1
#         cp $f1 points/
#     done
# done


# for mass in $points; do
#     mgl=$(echo $mass | cut -d'_' -f1)
#     mlsp=$(echo $mass | cut -d'_' -f2)
#     echo "mgl: $mgl, mlsp: $mlsp"
#     root -b -q -l -n makeTable.C"(\"${sig}_m${mgl}_m${mlsp}\")" | drop 2 > tables/table_${mass}.tex
#     (cd tables/ ; pdflatex -interaction=nonstopmode table_${mass}.tex; cp table_${mass}.pdf ../plots/)
# done


for r in hihi hilow; do
    for p in ttw ttzh ww wz fakes flips xg rares; do
        root -b -q printTotalSyst.C'("'$p'","'$r'","'$lumi'","points")';
    done;
    for mass in $points; do
        mgl=$(echo $mass | cut -d'_' -f1)
        mlsp=$(echo $mass | cut -d'_' -f2)
        echo "mgl: $mgl, mlsp: $mlsp"
        root -b -q printTotalSyst.C'("'${sig}_m${mgl}_m${mlsp}'","'$r'","'$lumi'","points")';
    done;
done
cp points/plots/* plots/
