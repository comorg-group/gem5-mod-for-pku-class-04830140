./build/ALPHA/gem5.opt --debug-flag=Branch configs/spec/se.py --bench=bzip2 --cpu-type=inorder --caches -I100000
NOW=$(date +"%F_%H_%M_%S")
LOGFILE="stat-$NOW.txt"
cat ./m5out/stats.txt | grep branchPred > ./m5out/$LOGFILE