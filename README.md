## Number-Partition-Problem
Various approaches to this NP-Complete problem. Implemented in C.

### Karmarkar-Karp heuristic
- Repeatedly take the largest two elements remaining in the set, insert their difference into the set, and remove the elements.
- My implementation uses a max heap
- [Original Paper](http://ijcai.org/Proceedings/09/Papers/096.pdf)

### Repeated random
- Repeatedly generate random solution to the problems, as determined by the representation.

### Hill climbing
- Generate a random solution to the problem, and then attempt to improve it through moves to better neighbours.

### Simulated annealing
- Generate a random solution to the problem, and then attempt to improve it through moves to neighbours, that are now always better.

### Dynamic Programming
- Discussion of pseudo-polynomial time algorithm
