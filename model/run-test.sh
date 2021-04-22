#!/usr/bin/env bash

#####
# This script runs the simulation multiple times, varying both the service time
# and the interarrival time.
#####

# USAGE INFORMATION:
# Simulation parameters:
# ./main.out <interarrival avg> <service mean> <lab_alg> <seed> <model>

# set up different testing scenarios
models=(mqms sqms)
algs=(random roundrobin utilbased leastcxns)

static_arr=1800  # static arrival for changing service
static_serv=4049 # static service for changing arrival
end_arr=10000    # arrival time to end looping at
end_serv=10000   # service time to end looping at

# set up spinner
sp="/-\|"
sc=0
spin() {
	printf "\b${sp:sc++:1}"
	((sc == ${#sp})) && sc=0
}
endspin() {
	printf "\r%s\n" "$@"
}

# set up progress bar
# 1. Create ProgressBar function
# 1.1 Input is currentState($1) and totalState($2)
function ProgressBar {
	# Process data
	let _progress=(${1} * 100 / ${2} * 100)/100
	let _done=(${_progress} * 4)/10
	let _left=40-$_done
	# Build progressbar string lengths
	_fill=$(printf "%${_done}s")
	_empty=$(printf "%${_left}s")

	# 1.2 Build progressbar strings and print the ProgressBar line
	# 1.2.1 Output example:
	# 1.2.1.1 Progress : [########################################] 100%
	printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"

}

# loop models
for m in "${models[@]}"; do
	# loop algorithms
	for a in "${algs[@]}"; do
		echo -e "\nRunning ${m} sim with alg ${a}"

		# test arrival variation
		filename=${m}-${a}-arrival.csv
		echo "n_nodes,avg_util,avg_q,avg_jobs,avg_delay,avg_wait,avg_tput,reject" >$filename
		for ((arr = 100; arr <= $end_arr; arr += 100)); do
			ProgressBar ${arr} ${end_arr}
			./main.out $arr $static_serv $a 1234 $m >>$filename
		done

		# test service variation
		filename=${m}-${a}-service.csv
		echo "n_nodes,avg_util,avg_q,avg_jobs,avg_delay,avg_wait,avg_tput,reject" >$filename
		for ((serv = 100; serv <= $end_serv; serv += 100)); do
			ProgressBar ${serv} ${end_serv}
			./main.out $static_arr $serv $a 1234 $m >>$filename
		done
	done
done

echo -e "\nDONE!"
