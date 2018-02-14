echo "[sh:cleanrun.sh] Removing existing files in [results] folder"
rm -R results
echo "[sh:cleanrun.sh] MkDir results"
mkdir results
./a.out
