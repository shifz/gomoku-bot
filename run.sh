#!/bin/bash
# (See https://arc-ts.umich.edu/greatlakes/user-guide/ for command details)
# Set up batch job settings

#SBATCH --job-name=mpi_demo_job
#SBATCH --mail-type=END
#SBATCH --nodes=1
#SBATCH --cpus-per-task=36
#SBATCH --mem-per-cpu=4g
#SBATCH --time=00:30:00
#SBATCH --account=eecs587f20_class
#SBATCH --partition=standard
# Run your program
# (">" redirects the print output of your program,
#  in this case to "output.txt")
rm -rf output.txt
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./gomoku >> output.txt