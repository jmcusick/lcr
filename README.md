# How to run and display LCR
```
cd Documents/workspace_c++/lcr
```
## Compile the program (if changes)
```
g++ main.cpp
```

## Run the program
```
./a.out
```

View results in prob.txt

Something odd going on here with xquarts, I think graph should just pop up


## Graph the output

```
gnuplot

set terminal svg
set output "out.svg"
load "plotProb.txt"
```

Open out.svg using Gapplin

To rerun:

1. delete out.svg
2. Rerun ```set output``` and ```load``` commands