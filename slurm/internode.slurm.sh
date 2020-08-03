#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --mem=2G
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=1
#SBATCH --job-name conduit-internode
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A.out"
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=mmore500@msu.edu

export CONTEXT=icer_internode

zsh <(curl -s https://raw.githubusercontent.com/mmore500/conduit/master/slurm/run.sh\?$(date +%s))
