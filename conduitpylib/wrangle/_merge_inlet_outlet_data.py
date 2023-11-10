from keyname import keyname as kn
import numpy as np
import pandas as pd

from ..utils import consolidate_merge, pickle_cache


@pickle_cache()
def merge_inlet_outlet_data(
    df_inlet: pd.DataFrame,
    df_outlet: pd.DataFrame,
) -> pd.DataFrame:
    df = consolidate_merge(
        df_inlet,
        df_outlet,
        on=["Process Instance UUID", "Update"],
        suffixes=(" Inlet", " Outlet"),
        how="outer",
    )
    if all(
        df_inlet["Runtime Seconds Elapsed"]
        == df_outlet["Runtime Seconds Elapsed"]
    ):
        df["Runtime Seconds Elapsed Inlet"] = df["Runtime Seconds Elapsed"]
        df["Runtime Seconds Elapsed Outlet"] = df["Runtime Seconds Elapsed"]

    df = df.astype(
        {
            "Num Inlets": "int64",
            "Num Outlets": "int64",
            "Num Puts Attempted": "int64",
            "Num Try Puts Attempted": "int64",
            "Num Blocking Puts": "int64",
            "Num Try Puts That Succeeded": "int64",
            "Num Puts That Succeeded Eventually": "int64",
            "Num Blocking Puts That Succeeded Immediately": "int64",
            "Num Puts That Succeeded Immediately": "int64",
            "Num Puts That Blocked": "int64",
            "Num Dropped Puts": "int64",
            "Num Round Trip Touches Inlet": "int64",
            "Net Flux Through Duct": "int64",
            "proc": "int64",
            "Snapshot": "int64",
            "Has Execution Blur": "bool",
            "Replicate": "int64",
            "Async Mode": "int64",
            "Num Threads": "int64",
            "Num Processes": "int64",
            "SLURM_NNODES": "int64",
            "SLURM_NTASKS": "int64",
            "SLURM_CPUS_ON_NODE": "int64",
        }
    )


    df["Hostname"] = df.apply(
        lambda row: kn.unpack(row["Source File Inlet"])["_hostname"],
        axis=1,
    )

    df["Num Nodes"] = df["SLURM_NNODES"]
    df["Num Tasks"] = df["SLURM_NTASKS"]
    df["Num Simels Per Cpu"] = df["Num Simulation Elements Per Cpu"]
    df["Num Cpus"] = df["Num Threads"] * df["Num Processes"]
    df["Allocated Tasks Per Node"] = df["Num Tasks"] // df["Num Nodes"]
    df["Cpus Per Node"] = df["Allocated Tasks Per Node"]
    df["Delivery Time Inlet"] = (df["Num Puts Attempted"] - 1) / df[
        "Num Round Trip Touches Inlet"
    ]
    df["Delivery Time Outlet"] = (df["Num Pulls Attempted"] - 1) / df[
        "Num Round Trip Touches Outlet"
    ]
    df["Intermittancy"] = df["Num Pulls That Were Laden Immediately"] / df[
        ["Net Flux Through Duct", "Num Pulls Attempted"]
    ].min(axis=1)
    df["Inlet-Seconds Elapsed"] = (
        df["Num Inlets"] * df["Runtime Seconds Elapsed Inlet"]
    )
    df["Outlet-Seconds Elapsed"] = (
        df["Num Outlets"] * df["Runtime Seconds Elapsed Outlet"]
    )
    df["Latency Simsteps Inlet"] = df["Delivery Time Inlet"]
    df["Latency Simsteps Outlet"] = df["Delivery Time Inlet"]
    df["Simstep Period Inlet (s)"] = (
        df["Inlet-Seconds Elapsed"] / df["Num Puts Attempted"]
    )
    df["Simstep Period Outlet (s)"] = (
        df["Outlet-Seconds Elapsed"] / df["Num Pulls Attempted"]
    )
    df["Latency Walltime Inlet (s)"] = (
        df["Latency Simsteps Inlet"] * df["Simstep Period Inlet (s)"]
    )
    df["Latency Walltime Outlet (s)"] = (
        df["Latency Simsteps Outlet"] * df["Simstep Period Outlet (s)"]
    )
    df["Log Num Processes"] = np.log(df["Num Processes"]) / np.log(4)

    return df
