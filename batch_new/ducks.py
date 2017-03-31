import sys
sys.path.insert(0,"NtupleTools/AutoTwopler")
import run
import params as p
import same_sign as ss

# make instructions
instructions = []
# for ds in ss.d_ds2name["backgrounds"].keys():
#     instructions.append({"executable": ss.executable, "package": ss.package, "analysis": "SS", "dataset": ds, "baby_tag": ss.tag, "type": "BABY", "extra": ""})
for ds in ss.d_ds2name["data"].keys():
    instructions.append({"executable": ss.executable, "package": ss.package, "analysis": "SS", "dataset": ds, "baby_tag": ss.tag, "type": "BABY", "extra": ""})
# for ds in ss.d_ds2name["signals"].keys():
#     instructions.append({"executable": ss.executable, "package": ss.package, "analysis": "SS", "dataset": ds, "baby_tag": ss.tag, "type": "BABY", "extra": ""})
# for ds in ss.d_ds2name["test"].keys():
#     instructions.append({"executable": ss.executable, "package": ss.package, "analysis": "SS", "dataset": ds, "baby_tag": ss.tag, "type": "BABY", "extra": ""})

# todo = "TTW TTZ WZ DY_low DY_high WJets TTG QQWW TTTT WWDPS TTBAR_PH ZZ TG WZZ WWZ WGToLNuG TZQ TTHtoNonBB VHtoNonBB TTZlow GGHtoZZto4L ZZZ WWW TTZnlo TTWnlo TWZ ZG TTZLOW WZLO WWG WZG".strip().split(" ")
# todo = "TTH_scan THQ_scan THW_scan".strip().split(" ")
# todo = "WWZ".strip().split(" ")
# instructions = [inst for inst in instructions if ss.dataset_to_shortname(inst["dataset"]) in todo]


p.dataset_to_shortname = ss.dataset_to_shortname
p.dashboard_name = ss.dashboard_name
p.sweepRoot_scripts = ss.sweepRoot_scripts
p.merging_scripts = ss.merging_scripts
p.baby_merged_dir = ss.baby_merged_dir
p.exit_when_done = True
p.open_datasets = False

run.main(instructions=instructions, params=p)

# instructions = [ 
#         {
#             "dataset": "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
#             "type": "BABY",
#             "extra": "",
#             "baby_tag": "v0.01",
#             "analysis": "MT2",
#             "package": ss.package,
#             "executable": ss.executable,
#             }
#         ]

# p.merging_scripts = ["frank/merge.sh", "frank/merge.C"]
# p.dashboard_name = "AutoTwopler_test"
# p.merge_babies_on_condor = True
# run.main(instructions=instructions, params=p, do_one_iteration=True)
