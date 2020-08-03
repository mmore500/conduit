import sys
import json
from keyname import keyname as kn
import pandas as pd

def load_json(filename):
    with open(filename) as json_file:
        data = json.load(json_file)
    return data

rows = []

for filename, entry in [
        (filename, load_json(filename))
        for filename in sys.argv[1:]
    ]:
    for benchmark in entry['benchmarks']:
        rows.append({
            'Mesh' : benchmark['name'].split('/')[0],
            'Implementation' : kn.unpack(filename)['impl'],
            'Threads' : benchmark['threads'],
            'Processes' : kn.unpack(filename)['procs'],
            'Statistic' : benchmark['aggregate_name'],
            'Wall Nanoseconds' : benchmark['real_time'],
            'CPU Nanoseconds' : benchmark['cpu_time'],
            'Latency' : benchmark['Latency'],
            'Lossiness' : benchmark['Lossiness'],
        })

pd.DataFrame(rows).to_csv(
    'consolidated.csv',
    index=False,
)
