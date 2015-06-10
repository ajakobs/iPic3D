#cd ../build.xeonphi
procs=8

#for threads in 1 2 4 8 16 32 60 120 240 # 1 process
#for threads in 1 2 4 8 16 30 60 120 	# 2 processes
#for threads in 1 2 4 8 15 30 60	# 4 processes
for threads in 1 2 4 7 14 28 		# 8 processes

do
	export OMP_NUM_THREADS=$threads
	bsub -K < $IPIC_HOME/env/marenostrum/mn3_m2x_native.lsf
	#$IPIC_HOME/env/marenostrum/mn3_m2x_native.lsf
	mkdir -p $IPIC_HOME/../results/m2x/${procs}_p/${threads}_t
	sleep 10s
	mv output_booster.out $IPIC_HOME/../results/m2x/${procs}_p/${threads}_t/${procs}P${threads}T_output_booster.out
	mv output_cluster.out $IPIC_HOME/../results/m2x/${procs}_p/${threads}_t/${procs}P${threads}T_output_cluster.out
	mv *.err $IPIC_HOME/../results/m2x/${procs}_p/${threads}_t 
	mv *.out $IPIC_HOME/../results/m2x/${procs}_p/${threads}_t 
done
