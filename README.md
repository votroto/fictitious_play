# Fictitious play
Solves a **matrix game** using the _terrible_ **fictitious-play** algorithm.

```console
$ cat payoff
5 7 4
3 5 6
6 4 5
$ cat payoff | .fictitious_play 
0.26 0.21 0.53
0.27 0.23  0.5
```