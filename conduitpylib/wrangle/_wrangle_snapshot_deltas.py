import numpy as np
import pandas as pd

from ..utils import pickle_cache

from ._diff_snapshot_states import diff_snapshot_states


@pickle_cache()
def wrangle_snapshot_deltas(
    merged_df: pd.DataFrame,
    max_snapshot: int = 5,
) -> pd.DataFrame:
    df_snapshot_diffs = diff_snapshot_states(merged_df, max_snapshot)

    df_snapshot_diffs["Fraction Messages Delivered"] = (
        df_snapshot_diffs["Num Try Puts That Succeeded"]
        / df_snapshot_diffs["Num Try Puts Attempted"]
    )
    df_snapshot_diffs["Delivery Success Rate"] = (
        df_snapshot_diffs["Num Try Puts That Succeeded"]
        / df_snapshot_diffs["Num Try Puts Attempted"]
    )
    df_snapshot_diffs["Delivery Failure Rate"] = (
        1 - df_snapshot_diffs["Delivery Success Rate"]
    )
    df_snapshot_diffs["Fraction Messages Dropped"] = df_snapshot_diffs[
        "Delivery Failure Rate"
    ]
    df_snapshot_diffs["Fraction Try Pulls That Were Laden"] = (
        df_snapshot_diffs["Num Try Pulls That Were Laden"]
        / df_snapshot_diffs["Num Try Pulls Attempted"]
    )

    df_snapshot_diffs["Round Trip Touches Per Attempted Put"] = (
        df_snapshot_diffs["Num Round Trip Touches Inlet"]
    ) / df_snapshot_diffs["Num Try Puts Attempted"]

    df_snapshot_diffs["Round Trip Touches Per Attempted Pull"] = (
        df_snapshot_diffs["Num Round Trip Touches Outlet"]
    ) / df_snapshot_diffs["Num Try Pulls Attempted"]

    df_snapshot_diffs["Round Trip Touches Per Runtime Nanosecond"] = (
        df_snapshot_diffs["Num Round Trip Touches Outlet"]
    ) / df_snapshot_diffs["Row Final Timepoint (ns) Outlet"]

    df_snapshot_diffs["Latency Simsteps Inlet"] = (
        df_snapshot_diffs["Num Puts Attempted"]
        / df_snapshot_diffs["Num Round Trip Touches Inlet"]
    )
    df_snapshot_diffs["Latency Simsteps Outlet"] = (
        df_snapshot_diffs["Num Pulls Attempted"]
        / df_snapshot_diffs["Num Round Trip Touches Outlet"]
    )
    df_snapshot_diffs["Inlet-Nanoseconds Elapsed"] = (
        df_snapshot_diffs["Num Inlets"]
        * df_snapshot_diffs["Row Final Timepoint (ns) Inlet"]
    )
    df_snapshot_diffs["Outlet-Nanoseconds Elapsed"] = (
        df_snapshot_diffs["Num Outlets"]
        * df_snapshot_diffs["Row Final Timepoint (ns) Outlet"]
    )
    df_snapshot_diffs["Simsteps Elapsed Inlet"] = (
        df_snapshot_diffs["Num Puts Attempted"]
        / df_snapshot_diffs["Num Inlets"]
    )
    df_snapshot_diffs["Simsteps Elapsed Outlet"] = (
        df_snapshot_diffs["Num Pulls Attempted"]
        / df_snapshot_diffs["Num Outlets"]
    )
    df_snapshot_diffs["Simstep Period Inlet (ns)"] = (
        df_snapshot_diffs["Inlet-Nanoseconds Elapsed"]
        / df_snapshot_diffs["Num Puts Attempted"]
    )
    df_snapshot_diffs["Simstep Period Outlet (ns)"] = (
        df_snapshot_diffs["Outlet-Nanoseconds Elapsed"]
        / df_snapshot_diffs["Num Pulls Attempted"]
    )
    df_snapshot_diffs["Latency Walltime Inlet (ns)"] = (
        df_snapshot_diffs["Latency Simsteps Inlet"]
        * df_snapshot_diffs["Simstep Period Inlet (ns)"]
    )
    df_snapshot_diffs["Latency Walltime Outlet (ns)"] = (
        df_snapshot_diffs["Latency Simsteps Outlet"]
        * df_snapshot_diffs["Simstep Period Outlet (ns)"]
    )

    df_snapshot_diffs = df_snapshot_diffs.astype(
        {
            "Num Inlets": "int64",
            "Num Outlets": "int64",
            "proc": "int64",
            "Snapshot": "int64",
            "Replicate": "int64",
            "Async Mode": "int64",
            "Num Threads": "int64",
            "Num Processes": "int64",
            "Allocated Tasks Per Node": "int64",
            "Cpus Per Node": "int64",
            "Num Nodes": "int64",
            "Num Simels Per Cpu": "int64",
        }
    )

    df_snapshot_diffs["Dominance"] = (
        df_snapshot_diffs["Num Try Puts Attempted"]
        / df_snapshot_diffs["Net Flux Through Duct"]
    )

    df_snapshot_diffs["Num Messages Per Pull"] = (
        df_snapshot_diffs["Net Flux Through Duct"]
        / df_snapshot_diffs["Num Pulls Attempted"]
    )

    df_snapshot_diffs["Num Messages Per Laden Pull"] = (
        df_snapshot_diffs["Net Flux Through Duct"]
        / df_snapshot_diffs["Num Pulls That Were Laden Immediately"]
    )

    df_snapshot_diffs["Bunchiness"] = (
        df_snapshot_diffs["Num Messages Per Laden Pull"]
        / df_snapshot_diffs["Num Messages Per Pull"]
    ) - 1
    df_snapshot_diffs["Delivery Clumpiness"] = df_snapshot_diffs["Bunchiness"]
    df_snapshot_diffs["Intermittancy"] = df_snapshot_diffs["Bunchiness"]
    df_snapshot_diffs["Bunching"] = df_snapshot_diffs["Bunchiness"]

    df_snapshot_diffs["Latency, Log10 Updates"] = np.log10(
        df_snapshot_diffs["Latency Simsteps Inlet"]
    )

    df_snapshot_diffs["ms Per Update"] = (
        df_snapshot_diffs["Simstep Period Inlet (ns)"] / 10**6
    )

    df_snapshot_diffs["Delivery Failure, %"] = (
        df_snapshot_diffs["Fraction Messages Dropped"] * 100
    )
    df_snapshot_diffs["% Msgs Lost"] = df_snapshot_diffs["Delivery Failure, %"]

    df_snapshot_diffs["Msgs Received Per Sent"] = df_snapshot_diffs[
        "Num Messages Per Pull"
    ]

    df_snapshot_diffs["Messages Received Per Second"] = df_snapshot_diffs[
        "Net Flux Through Duct"
    ] / (df_snapshot_diffs["Outlet-Nanoseconds Elapsed"] / 10**9)
    df_snapshot_diffs["Messages Sent Per Second"] = df_snapshot_diffs[
        "Num Try Puts Attempted"
    ] / (df_snapshot_diffs["Inlet-Nanoseconds Elapsed"] / 10**9)

    df_snapshot_diffs["Any Messages Dropped"] = df_snapshot_diffs[
        "Fraction Messages Dropped"
    ].astype(bool)

    df_snapshot_diffs["Messages Received Per Second"] = df_snapshot_diffs[
        "Net Flux Through Duct"
    ] / (df_snapshot_diffs["Outlet-Nanoseconds Elapsed"] / 10**9)
    df_snapshot_diffs["Messages Sent Per Second"] = df_snapshot_diffs[
        "Num Try Puts Attempted"
    ] / (df_snapshot_diffs["Inlet-Nanoseconds Elapsed"] / 10**9)

    return df_snapshot_diffs
