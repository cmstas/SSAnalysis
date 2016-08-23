# sig="fs_t1tttt_m1000_m450"
# orig_dir="v8.04_July28/"
# new_dir="v8.04_July28_singlebin/"
# lumi="12.9"


# mkdir -p $new_dir
# for thing in $sig data ttw ttzh wz ww xg rares fakes flips; do
#     echo $thing;
#     file=${thing}_histos_*_${lumi}ifb.root
#     cp $orig_dir/$file $new_dir/
# done

sig=$1
orig_dir=$2
new_dir=$3
lumi=$4


mkdir -p $new_dir
for thing in $sig data ttw ttzh wz ww xg rares fakes flips; do
    echo $thing;
    file=${thing}_histos_*_${lumi}ifb.root
    # echo "cp $orig_dir/$file $new_dir/"
    cp $orig_dir/$file $new_dir/

done

