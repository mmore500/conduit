---
title: 'Gala: A Python package for galactic dynamics'
tags:
  - Python
  - astronomy
  - dynamics
  - galactic dynamics
  - milky way
authors:
  - name: Adrian M. Price-Whelan^[Custom footnotes for e.g. denoting who the corresponding author is can be included like this.]
    orcid: 0000-0003-0872-7098
    affiliation: "1, 2" # (Multiple affiliations must be quoted)
  - name: Author Without ORCID
    affiliation: 2
  - name: Author with no affiliation
    affiliation: 3
affiliations:
 - name: Lyman Spitzer, Jr. Fellow, Princeton University
   index: 1
 - name: Institution Name
   index: 2
 - name: Independent Researcher
   index: 3
date: 13 August 2017
bibliography: paper.bib

# Optional fields if submitting to a AAS journal too, see this blog post:
# https://blog.joss.theoj.org/2018/12/a-new-collaboration-with-aas-publishing
aas-doi: 10.3847/xxxxx <- update this with the DOI from AAS once you know it.
aas-journal: Astrophysical Journal <- The name of the AAS journal.
---

# Summary

The forces on stars, galaxies, and dark matter under external gravitational
fields lead to the dynamical evolution of structures in the universe. The orbits
of these bodies are therefore key to understanding the formation, history, and
future state of galaxies. The field of "galactic dynamics," which aims to model
the gravitating components of galaxies to study their structure and evolution,
is now well-established, commonly taught, and frequently used in astronomy.
Aside from toy problems and demonstrations, the majority of problems require
efficient numerical tools, many of which require the same base code (e.g., for
performing numerical orbit integration).

# Statement of need

`Gala` is an Astropy-affiliated Python package for galactic dynamics. Python
enables wrapping low-level languages (e.g., C) for speed without losing
flexibility or ease-of-use in the user-interface. The API for `Gala` was
designed to provide a class-based and user-friendly interface to fast (C or
Cython-optimized) implementations of common operations such as gravitational
potential and force evaluation, orbit integration, dynamical transformations,
and chaos indicators for nonlinear dynamics. `Gala` also relies heavily on and
interfaces well with the implementations of physical units and astronomical
coordinate systems in the `Astropy` package [@astropy] (`astropy.units` and
`astropy.coordinates`).

`Gala` was designed to be used by both astronomical researchers and by
students in courses on gravitational dynamics or astronomy. It has already been
used in a number of scientific publications [@Pearson:2017] and has also been
used in graduate courses on Galactic dynamics to, e.g., provide interactive
visualizations of textbook material [@Binney:2008]. The combination of speed,
design, and support for Astropy functionality in `Gala` will enable exciting
scientific explorations of forthcoming data releases from the *Gaia* mission
[@gaia] by students and experts alike.

# Mathematics

Single dollars ($) are required for inline mathematics e.g. $f(x) = e^{\pi/x}$

Double dollars make self-standing equations:

$$\Theta(x) = \left\{\begin{array}{l}
0\textrm{ if } x < 0\cr
1\textrm{ else}
\end{array}\right.$$

You can also use plain \LaTeX for equations
\begin{equation}\label{eq:fourier}
\hat f(\omega) = \int_{-\infty}^{\infty} f(x) e^{i\omega x} dx
\end{equation}
and refer to \autoref{eq:fourier} from text.

# Citations

Citations to entries in paper.bib should be in
[rMarkdown](http://rmarkdown.rstudio.com/authoring_bibliographies_and_citations.html)
format.

If you want to cite a software repository URL (e.g. something on GitHub without a preferred
citation) then you can do it with the example BibTeX entry below for @fidgit.

<!-- Write your comments here
For a quick reference, the following citation commands can be used:
- `@author:2001`  ->  "Author et al. (2001)"
- `[@author:2001]` -> "(Author et al., 2001)"
- `[@author1:2001; @author2:2001]` -> "(Author1 et al., 2001; Author2 et al., 2002)"
-->

# Figures

<!-- Figures can be included like this:
![Caption for example figure.\label{fig:example}](figure.png)
and referenced from text using \autoref{fig:example}.

Figure sizes can be customized by adding an optional second parameter:
![Caption for example figure.](figure.png){ width=20% } -->

# Acknowledgements

We acknowledge contributions from Brigitta Sipocz, Syrtis Major, and Semyeong
Oh, and support from Kathryn Johnston during the genesis of this project.

# Software Landscape Survey Scrapped from Quality of Service Paper

The parallel and distributed processing capacity of high-performance computing (HPC) clusters continues to grow rapidly and enable profound scientific and industrial innovations [@gagliardi2019international].
These advances in hardware capacity and economy afford great opportunity but also pose a serious challenge: developing approaches to effectively harness it.
As HPC systems scale, it becomes increasingly difficult to write software that makes efficient use of available hardware and also provides reproducible results (or even near-perfectly reproducible results — i.e., up to effects from floating point non-transitivity) consistent with models of computation as being performed a reliable digital machine [@heroux2014toward].

The bulk synchronous parallel (BSP) model, which is prevalent among HPC applications [@dongarra2014applied], illustrates the challenge.
This model segments fragments of computation into sequential global supersteps, with fragments at superstep $i$ depending only on data from strictly preceding fragments $<i$, often just $i-1$.
Computational fragments are assigned across a pool of available processing components.
The BSP model assumes perfectly reliable messaging: all dispatched messages between computational fragments are faithfully delivered.
In practice, realizing this assumption introduces overhead costs: secondary acknowledgment messages to confirm delivery and mechanisms to dispatch potential resends as the need arises.
Global synchronization occurs between supersteps, with computational fragments held until their preceding superstep has completed [@valiant1990bridging].
This ensures that computational fragments will have at hand every single expected input, including those required from fragments located on other processing elements, before proceeding.
So, supersteps only turn over once the entire pool of processing components has completed their work for that superstep.
Put another way, all processing components stall until the most laggardly component catches up.
In a game of double dutch with several jumpers, this would be like slowing the tempo to whoever is most slow-footed each particular turn of the rope.

Heterogeneous computational fragments, with some easy to process and others much slower, would result in poor efficiency under a naive approach where each processing element handled just one fragment.
Some processing elements with easy tasks would finish early then idle while more difficult tasks carry on.
To counteract such load imbalances, programmers can allow for "parallel slack" by ensuring computational fragments greatly outnumber processing elements or even performing dynamic load balancing at runtime [@valiant1990bridging].

Unfortunately, hardware factors on the underlying processing elements ensure that inherent global superstep jitter will persist: memory access time varies due to cache effects, message delivery time varies due to network conditions, extra processing due to error detection and recovery, delays due to unfavorable process scheduling by the operating system, etc. [@dongarra2014applied].
Power management concerns on future machines will likely introduce even more variability [@gropp2013programming].
Worse yet, as we work with more and more processes, the expected magnitude of the worst-sampled jitter grows and grows — and in lockstep with it, our expected superstep duration.
In the double dutch analogy, with enough jumpers, at almost every turn of the rope someone will need to stop and tie their shoe.
The global synchronization operations underpinning the BSP model further hinder its scalability.
Irrespective of time to complete computational fragments within a superstep, the cost of performing a global synchronization operation increases with processor count [@dongarra2014applied].

Efforts to recover scalability by relaxing superstep synchronization fall under two banners.
The first approach, termed "Relaxed Bulk-Synchronous Programming" (rBSP), hides latency by performing collective operations asynchronously, essentially allowing useful computation to be performed at the same time as synchronization primitives for a single superstep percolate through the collective [@heroux2014toward].
So, the time cost required to perform that synchronization can be discounted, up to the time taken up by computational work at one superstep.
Likewise, individual processes experiencing heavier workloads or performance degradation due to hardware factors can fall behind by up to a single superstep without slowing the entire collective.
However, this approach cannot mask synchronization costs or cumulative performance degradation exceeding a single superstep's duration.
The second approach, termed relaxed barrier synchronization, forgoes global synchronization entirely [@kim1998relaxed].
Instead, computational fragments at superstep $i$ only wait on expected inputs from the subset of superstep $i-1$ fragments that they directly interact with.
Imagine a double-dutch routine where each jumper exchanges patty cakes with both neighboring jumpers at every turn of the rope.
Relaxed barrier synchronization would dispense entirely with the rope.
Instead, players would be free to proceed to their next round of patty cakes as soon as they had successfully patty-caked both neighbors.
With $n$ players, player 0 could conceivably advance $n$ rounds ahead of player $n-1$ (each player would be one round ahead of their right neighbor).
Assuming fragment interactions form a graph structure that persists across supersteps, in the general case before causing the entire collective to slow an individual fragment can fall behind at most a number of supersteps equal to the graph diameter [@gamell2015local].
Even though this approach can shield the collective from most one-off performance degradations of a single fragment (especially in large-diameter cases), persistently laggard hardware or extreme one-off degradations will ultimately still hobble efficiency.
Dynamic task scheduling and migration aim to address this shortcoming, redistributing work in order to "catch up" delinquent fragments [@acun2014parallel].
With our double-dutch analogy, we could think of this something like a team coach temporarily benching a jumper who skinned their knee and instructing the other jumpers to pick up their roles in the routine.

In addition to concerns over efficiency, resiliency poses another inexorable problem to massive HPC systems.
In small scales, it can suffice to assume that failures occur negligibly, with any that do transpire likely to cause an (acceptably rare) global interruption or failure.
At large scales, however, software crashes and hardware failures become the rule rather than the exception [@dongarra2014applied] — running a simulation to completion could even require so many retries as to be practically infeasible.
A typical contemporary approach to improve resiliency is checkpointing: the system periodically records global state then, when a failure arises, progress is rolled back to the most recent global known-good state and runtime restarts [@hursey2007design].
Global checkpoint-based recovery is expensive, especially at scale due to overhead associated with regularly recording global state, losing progress since the most recent checkpoint, and actually performing a global teardown and restart procedure.
In fact, at large enough scales global recovery durations could conceivably exceed mean time between failures, making any forward simulation progress all but impossible [@dongarra2014applied].
The local failure, local recovery (LFLR) paradigm eschews global recovery by maintaining persistent state on a process-wise basis and providing a recovery function to initialize a step-in replacement process [@heroux2014toward,teranishi2014toward].
In practice, such an approach can require keeping running logs of all messaging traffic in order to replay them for the benefit of any potential step-in replacement [@chakravorty2004fault].
Returning once more to the double dutch analogy, LFLR would transpire as something like a handful teammates pulling a stricken teammate aside to catch them up after an amnesia attack (rather than starting the entire team's routine back at the top of the current track).
The intervening jumpers would have to remind the stricken teammate of a previously recorded position then discreetly re-feign some of their moves that the stricken teammate had cued off of between that recorded position and the amnesia episode.

The possibility of multiple simultaneous failure (perhaps, for example, of dozens of processes resident on a single node) poses an even more difficult, although not insurmountable, challenge for LFLR that would likely necessitate even greater overhead.
One approach involves pairing up with a remote "buddy" process.
The "buddy" hangs to the focal process' snapshots and is carbon-copied on all of that process' messages in order to ensure an independently survivable log.
Unfortunately, this could potentially require forwarding all messaging traffic between simulation elements coresident on the focal process to its buddy, dragging inter-node communication into some otherwise trivial simulation operations [@chakravorty2007fault].
Efforts to ensure resiliency beyond single-node failures currently appear unnecessary [@ni2016mitigation].
Even though LFLR saves the cost of global spin-down and spin-up, all processes will potentially have to wait for work lost since the last checkpoint to be recompleted, although in some cases this could be helped along by tapping idle hardware to take over delinquent work from the failed process and help catch it up [@dongarra2014applied].

Still more insidious to the reliable digital machine model, though, are soft errors — events where corruption of data in memory occurs, usually due to environmental interference (i.e., "cosmic rays") [@karnik2004characterization].
Further miniaturization and voltage reduction, which are assumed as a likely vehicle for continuing advances in hardware efficiency and performance, could conceivably worsen susceptibility to such errors [@dongarra2014applied,@kajmakovic2020challenges].
What makes soft errors so dangerous is their potential indetectability.
Unlike typical hardware or software failures, which result in an explicit, observable outcome (i.e., an error code, an exception, or even just a crash), soft errors can transpire silently and lead to incorrect computational results without leaving anyone the wiser.
Luckily, soft errors occur rarely enough to be largely neglected in most single-processor applications (except the most safety-critical settings); however, at scale soft errors occur at a non-trivial rate [@sridharan2015memory,@scoles2018cosmic].
Redundancy (be it duplicated hardware components or error correction codes) can reduce the rate of uncorrected (or at least undetected) soft errors, although at a non-trivial cost [@vankeirsbilck2015soft,@sridharan2015memory].
In some application domains with symmetries or conservation principles, the rate of soft errors (or, at least, silent soft errors) could be also reduced through so-called "skeptical" assertions at runtime [@dongarra2014applied], although this too comes at a cost.

Even if soft errors can be effectively eradicated — or at least suppressed to a point of inconsequentiality — the nondeterministic mechanics of fault recovery and dynamic task scheduling could conceivably make guaranteeing bitwise reproducibility at exascale effectively impossible, or at least an unreasonable engineering choice [@dongarra2014applied].
However, the assumption of the reliable digital machine model remains near-universal within parallel and distributed algorithm design [@chakradhar2010best].
Be it just costly or simply a practical impossibility, the worsening burden of synchronization, fault recovery, and error correction begs the question of whether it is viable to maintain, or even to strive to maintain, the reliable digital machine model at scale.
Indeed, software and hardware that relaxes guarantees of correctness and determinism — a so-called "best-effort model" — have been shown to improve speed [@chakrapani2008probabilistic], energy efficiency [@chakrapani2008probabilistic,@bocquet2018memory], and scalability [@meng2009best].
Discussion around "approximate computing" overlaps significantly with "best-effort computing," although focusing more heavily on using algorithm design to shirk non-essential computation (i.e., reducing floating point precision, inexact memoization, etc.) [@mittal2016survey].
As technology advances, computing is becoming more distributed and we are colliding with physical limits for speed and reliability.
Massively distributed systems are becoming inevitable, and indeed if we are to truly achieve "indefinite scalability" [@ackley2011pursue] we must shift from guaranteed accuracy to best-effort methods that operate asynchronously and degrade gracefully under hardware failure.

The suitability of the best-effort model varies from application to application.
Some domains are clear-cut in favor of the reliable digital machine model --- for example, due to regulatory issues [@dongarra2014applied].
However, a subset of HPC applications can tolerate --- or even harness --- occasionally flawed or even fundamentally nondeterministic computation [@chakradhar2010best].
Various approximation algorithms or heuristics fall into this category, with notable work being done on best-effort stochastic gradient descent for artificial neural network applications [@dean2012large,@zhao2019elastic,@niu2011hogwild,@noel2014dogwild,@rhodes2020real].
Best-effort, real-time computing approaches have also been used in some artificial life models [@ray1995proposal].
Likewise, algorithms relying on pseudo-stochastic methods that tend to exploit noise (rather than destabilize due to it) also make good candidates [@chakrapani2008probabilistic,@chakradhar2010best].
Real-time control systems that cannot afford to pause or retry, by necessity, fall into the best-effort category [@rahmati2011computing,@rhodes2020real].

This work distills best-effort communication from the larger issue of best-effort computing, paying it special attention and generally pretermiting the larger issue.
Specifically, we investigate the implications of relaxing synchronization and message delivery requirements.
Under this model, the runtime strives to minimize message latency and loss but guarantees elimination of neither.
Instead, processes continue their compute work unimpeded and incorporate communication from collaborating processes as it happens to become available.
We still assume that messages, if and when they are delivered, retain contentual integrity.

We see best-effort communication as a particularly fruitful target for investigation.
Firstly, synchronization constitutes the root cause of many contemporary scaling bottlenecks, well below the mark of thousands or millions of cores where runtime failures and soft errors become critical considerations.
Secondly, future HPC hardware is expected to provide more heterogeneous, more variable (i.e., due to power management), and generally lower (relative to compute) communication bandwidth [@gropp2013programming,@acun2014parallel]; a best-effort approach suits these challenges.
A best-effort communication model presents the possibility of runtime adaptation to effectively utilize available resources given the particular ratio of compute and communication capability at any one moment in any one rack.

Complex biological organisms exhibit characteristic best-effort properties: trillions of cells interact asynchronously while overcoming all but the most extreme failures in a noisy world.
As such, bio-inspired algorithms present strong potential to benefit from best-effort communication strategies.
For example, evolutionary algorithms commonly use guided stochastic methods (i.e., selection and mutation operators) resulting in a search process that does not guarantee optimality but typically produces a diverse range of high-quality results.
Indeed, island model genetic algorithms are easy to parallelize and have been shown to perform well with asynchronous migration [@izzo2009parallel].
Likewise, artificial life simulations commonly rely on a bottom-up approach and seek to model life-as-it-could-be evolving in a noisy environment akin to the natural world, yet distinct from it [@bonabeau1994we]. % rather than the implications of certain mechanistic assumptions
Although perfect reproducibility and observability have uniquely enabled digital evolution experiments to ask and answer otherwise intractable questions [@pontes2020evolutionary,@lenski2003evolutionary,@grabowski2013case,@dolson2020interpreting,@fortuna2019coevolutionary,@goldsby2014evolutionary,@covert2013experiments,@zaman2011rapid,@bundy2021footprint,@dolson2017spatial], the reliable digital machine model is not strictly necessary for all such work.
Issues of distributed and parallel computing are of special interest within the the artificial life subdomain of open-ended evolution (OEE) [@ackley2014indefinitely], which studies long-term dynamics of evolutionary systems in order to understand factors that affect potential to generate ongoing novelty [@taylor2016open].
Recent evidence suggests that the generative potential of at least some model systems is --- at least in part --- meaningfully constrained by available compute resources [@channon2019maximum].

Much exciting work on best-effort computing has incorporated bespoke experimental hardware [@chippa2014scalable,@ackley2011homeostatic,@cho2012ersa,@chakrapani2008probabilistic,@rhodes2020real].
However, here, we focus on exploring best-effort communication among parallel and distributed elements within existing, commercially-available hardware.
Existing software libraries, though, do not explicitly expose a convenient best-effort communication interface for such work.
As such, best-effort approaches remain rare in production software and efforts to study best-effort communication must make use of a combination of limited existing support and the development of new software tools.

The Message Passing Interface (MPI) standard [@gropp1996high] represents the mainstay for high-performance computing applications.
This standard exposes communication primitives directly to the end user.
MPI's nonblocking communication primitives, in particular, are sufficient to program distributed computations with relaxed synchronization requirements.
Although explicit, the imperative nature of the MPI protocols enables precise control over execution; unfortunately it also poses a significant expense in terms of programmability.
This cost manifests in terms of reduced programmer productivity and software quality, while increasing domain knowledge requirements and the effort required to tune for performance due to program brittleness [@gu2019comparative,@tang2014mpi].

In response to programmability concerns, many frameworks have arisen to offer useful parallel and distributed programming abstractions.
Task-based frameworks such as Charm++ [@kale1993charm++], Legion [@bauer2012legion], Cilk [@blumofe1996cilk], and Threading Building Blocks (TBB) [@reinders2007intel] describe the dependency relationships among computational tasks and associated data and rely on an associated runtime to automatically schedule and manage execution.
These frameworks assume a deterministic relationship between tasks.
In a similar vein, programming languages and extensions like Unified Parallel C (UPC) [@el2006upc] and Chapel [@chamberlain2007parallel] rely on programmers to direct execution but equip them with powerful abstractions, such as global shared memory.
However, Chapel's memory model explicitly forbids data races and UPC ultimately relies on a barrier model for data transfer.

To bridge these shortcomings, we employ a new software framework, the Conduit C++ Library for Best-Effort High-Performance Computing [@moreno2021conduit].
The Conduit library provides tools to perform best-effort communication in a flexible, intuitive interface and uniform inter-operation of serial, parallel, and distributed modalities.
Although Conduit currently implements distributed functionality via MPI intrinsics, in future work we will explore lower-level protocols like InfiniBand Unreliable Datagrams [@kashyap2006ip,@koop2007high].

# References
