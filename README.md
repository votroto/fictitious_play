# Fictitious play
Solves a **matrix game** using the _terrible_ **fictitious-play** algorithm.

```console
$ cat payoff
5 7 4
3 5 6
6 4 5
$ cat payoff | .fictitious_play 
0.251 0.247 0.502
0.252 0.253 0.495
```