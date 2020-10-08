import networkx as nx
import matplotlib.pyplot as plt
from keyname import keyname as kn

dims = [3, 10, 12, 15, 27, 56, 99]

def make_filename(dim):
    return kn.pack({
        'name'  : 'procon',
        'ndims' : '1',
        'dim0'  : str(dim),
        'ext'   : '.adj'
    })

for dim in dims:
    procon = [(i, i + 1) for i in range(0, dim, 2)]
    G_procon = nx.DiGraph(procon)

    with open("assets/" + make_filename(dim), "w") as file:
        for line in nx.generate_adjlist(G_procon):
            file.write(line + '\n')

