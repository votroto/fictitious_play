# Fictitious play
Solves a **matrix game** using the _terrible_ **fictitious-play** algorithm.

```console
$ cat payoff
5 7 4
3 5 6
6 4 5
$ cat payoff | .fictitious_play 
 0.25 0.256 0.494
0.248 0.256 0.496
```