#!/bin/bash

config=${1-uml.xml}
LMC=${LMC-../utils/lmc}
TMP=${TMP:-/tmp}

# create nodes
${LMC} -o $config --node uml1 --net uml1 tcp 
${LMC} -m $config --node uml2 --net uml2 tcp 
${LMC} -m $config --node uml3 --net uml3 tcp 

# configure mds server
${LMC} -m $config  --node uml1 --mds mds1 $TMP/mds1 50000

# configure ost
${LMC} -m $config  --lov lov1 mds1 65536 0 0
${LMC} -m $config  --node uml2 --lov lov1 --ost $TMP/ost1 100000
${LMC} -m $config  --node uml2 --lov lov1 --ost $TMP/ost2 100000
# is this needed?
# ${LMC} -m $config  --node uml2 --mdc MDC_mds1

# create client config
${LMC} -m $config  --node uml3 --mtpt /mnt/lustre mds1 lov1

