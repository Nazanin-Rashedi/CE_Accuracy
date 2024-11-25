#!/bin/bash

REL_RAW="/global/rashedi/H2D/rel/res_raw.rel"
REL_AGG="/global/rashedi/H2D/rel/res_aggr.rel"


FA="${HOME}/obj/${HOST}/falg/main_fa"

echo "sum counts of bad queries (qc >= 2) per synopsis"
${FA} -fs '|' -sp "qc >= 2" --group "syn_id, syn" --aggr "cnt: sum(cnt)" --sort cnt ${REL_RAW}

echo "--group syn --aggr cnt:count(*):"
${FA} -fs '|' --group "syn" --aggr "cnt:count(*)" --sort cnt ${REL_AGG}

