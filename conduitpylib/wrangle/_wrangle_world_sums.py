import pandas as pd

from ._find_treat_idx_mapped_col import find_treat_idx_mapped_col


def wrangle_world_sums(df_finalized_observations: pd.DataFrame) -> pd.DataFrame:
    df_world_sum = df_finalized_observations.groupby(
        [
            "Replicate",
            "Async Mode",
            "Num Processes",
            "Num Nodes",
            "Num Simels Per Cpu",
            "Allocated Tasks Per Node",
            "Cpus Per Node",
            "Allocation",
            find_treat_idx_mapped_col(df_finalized_observations),
        ],
        as_index=False,
    ).sum()

    df_world_sum["Fraction Messages Utilized"] = (
        df_world_sum["Num Reads That Were Fresh"]
        / df_world_sum["Num Try Puts Attempted"]
    )
    df_world_sum["Fraction Messages Delivered"] = (
        df_world_sum["Num Try Puts That Succeeded"]
        / df_world_sum["Num Try Puts Attempted"]
    )
    df_world_sum["Delivery Failure Rate"] = (
        1.0 - df_world_sum["Fraction Messages Delivered"]
    )
    df_world_sum["Fraction Messages Dropped"] = df_world_sum[
        "Delivery Failure Rate"
    ]
    df_world_sum["Fraction Try Pulls That Were Laden"] = (
        df_world_sum["Num Try Pulls That Were Laden"]
        / df_world_sum["Num Try Pulls Attempted"]
    )
    df_world_sum["Round Trip Touches Per Attempted Pull"] = (
        df_world_sum["Num Round Trip Touches Outlet"]
        / df_world_sum["Num Try Pulls Attempted"]
    )
    df_world_sum["Round Trip Touches Per Attempted Put"] = (
        df_world_sum["Num Round Trip Touches Inlet"]
        / df_world_sum["Num Try Puts Attempted"]
    )
    df_world_sum["Num Inflight Messages"] = (
        2.0 / df_world_sum["Round Trip Touches Per Attempted Put"] - 1
    )
    df_world_sum["Fraction Duct Flux Stepped Through"] = (
        df_world_sum["Num Revisions Pulled"]
        / df_world_sum["Net Flux Through Duct"]
    )
    df_world_sum["Fraction Duct Flux Jumped Over"] = (
        1.0 - df_world_sum["Fraction Duct Flux Stepped Through"]
    )
    df_world_sum["Round Trip Touches Per Runtime Second"] = (
        df_world_sum["Num Round Trip Touches Inlet"]
        / df_world_sum["Runtime Seconds Elapsed Inlet"]
    )
    df_world_sum["Latency Simsteps Inlet"] = (
        df_world_sum["Num Puts Attempted"] - 1
    ) / df_world_sum["Num Round Trip Touches Inlet"]
    df_world_sum["Latency Simsteps Outlet"] = (
        df_world_sum["Num Pulls Attempted"] - 1
    ) / df_world_sum["Num Round Trip Touches Outlet"]

    df_world_sum["Bunchiness"] = (
        df_world_sum["Num Messages Per Laden Pull"]
        / df_world_sum["Num Messages Per Pull"]
    ) - 1
    df_world_sum["Delivery Clumpiness"] = df_world_sum["Bunchiness"]
    df_world_sum["Intermittancy"] = df_world_sum["Bunchiness"]
    df_world_sum["Bunching"] = df_world_sum["Bunchiness"]


    df_world_sum["Simstep Period Inlet (s)"] = (
        df_world_sum["Inlet-Seconds Elapsed"]
        / df_world_sum["Num Puts Attempted"]
    )
    df_world_sum["Simstep Period Outlet (s)"] = (
        df_world_sum["Outlet-Seconds Elapsed"]
        / df_world_sum["Num Pulls Attempted"]
    )
    df_world_sum["Latency Walltime Inlet (s)"] = (
        df_world_sum["Latency Simsteps Inlet"]
        * df_world_sum["Simstep Period Inlet (s)"]
    )
    df_world_sum["Latency Walltime Outlet (s)"] = (
        df_world_sum["Latency Simsteps Outlet"]
        * df_world_sum["Simstep Period Outlet (s)"]
    )

    return df_world_sum
