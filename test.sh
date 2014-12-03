#!/usr/local/bin/zsh
./build/ALPHA/gem5.opt --debug-flag=Branch configs/example/se.py --cpu-type=inorder --cmd=./tests/test-progs/hello/bin/alpha/linux/hello --caches
NOW=$(date +"%F_%H_%M_%S")
LOGFILE="stat-$NOW.txt"
cat ./m5out/stats.txt | grep execution_unit > ./m5out/$LOGFILE
