# randompi
Approximating pi using random numbers

I learned this technique from [Matt Parker's video](https://www.youtube.com/watch?v=RZBhSi_PwHU) and stole the coprimacy test from [here](http://math.stackexchange.com/q/1107938).

## Usage
To make running easier, I have written `run.sh` to do things automatically.
### Examples
To run in parallel, use the following syntax:
```shell
./run.sh parallel <CPUs> <trials>
```
To run in serial (you don't really need `run.sh` for this), use this syntax instead:
```shell
./run.sh <trials>
```
`run.sh` does not recompile the program if it has been compiled already. To manually recompile, just use `make` and `make parallel`.
