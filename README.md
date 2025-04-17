# OPST
Before compiling and running, please install [sdsl](https://github.com/simongog/sdsl-lite/tree/master).
```bash
./pre-install.sh
```


## Compile and run

```bash 
make OPSTMaximal_MS
```





You can easily specify parameters, for example:

```bash 
./OPSTMaximal_MS -a inputA.txt -b inputB.txt -r 256 
```
-a specifies the input A;
-b specifies the input B;
-r specifies  "the range of (a,b) determines calling the naive or wavelet tree method", the default value is 512.

## Datasets
...


