## Citing
Please cite Conduit as

bibtex:
```
@inproceedings{10.1145/3449726.3463205,
author = {Moreno, Matthew Andres and Papa, Santiago Rodriguez and Ofria, Charles},
title = {Conduit: A C++ Library for Best-Effort High Performance Computing},
year = {2021},
isbn = {9781450383516},
publisher = {Association for Computing Machinery},
address = {New York, NY, USA},
url = {https://doi.org/10.1145/3449726.3463205},
doi = {10.1145/3449726.3463205},
abstract = {Developing software to effectively take advantage of growth in parallel and distributed
processing capacity poses significant challenges. Traditional programming techniques
allow a user to assume that execution, message passing, and memory are always kept
synchronized. However, maintaining this consistency becomes increasingly costly at
scale. One proposed strategy is "best-effort computing", which relaxes synchronization
and hardware reliability requirements, accepting nondeterminism in exchange for efficiency.
Although many programming languages and frameworks aim to facilitate software development
for high performance applications, existing tools do not directly provide a prepackaged
best-effort interface. The Conduit C++ Library aims to provide such an interface for
convenient implementation of software that uses best-effort inter-thread and inter-process
communication. Here, we describe the motivation, objectives, design, and implementation
of the library. Benchmarks on a communication-intensive graph coloring problem and
a compute-intensive digital evolution simulation show that Conduit's best-effort model
can improve scaling efficiency and solution quality, particularly in a distributed,
multi-node context.},
booktitle = {Proceedings of the Genetic and Evolutionary Computation Conference Companion},
pages = {1795–1800},
numpages = {6},
keywords = {high performance computing, best-effort computing},
location = {Lille, France},
series = {GECCO '21}
}
```

APA:
> Moreno, M., Papa, S., & Ofria, C. (2021). Conduit: A C++ Library for Best-Effort High Performance Computing. In Proceedings of the Genetic and Evolutionary Computation Conference Companion (pp. 1795–1800). Association for Computing Machinery.

Chicago:
>  Moreno, Matthew Andres, Santiago Rodriguez, Papa, and Charles, Ofria. "Conduit: A C++ Library for Best-Effort High Performance Computing." . In Proceedings of the Genetic and Evolutionary Computation Conference Companion (pp. 1795–1800). Association for Computing Machinery, 2021.

MLA:
> Moreno, Matthew Andres et al. "Conduit: A C++ Library for Best-Effort High Performance Computing." Proceedings of the Genetic and Evolutionary Computation Conference Companion. Association for Computing Machinery, July 7 2021, 1795-1800.

You can also access metadata to cite Conduit in our [`CITATION.cff` file](https://github.com/mmore500/conduit/blob/master/CITATION.cff).
Formatted citations were generated via <https://bibtex.online>.
