import numpy as np
import pandas as pd

from ..utils import pickle_cache


@pickle_cache()
def wrangle_longitudinal_deltas(merged_df: pd.DataFrame) -> pd.DataFrame:
    # https://stackoverflow.com/a/40629420
    df_finalized_observations = merged_df.sort_values(
        "Update", ascending=False
    ).drop_duplicates(["Process Instance UUID"])

    df_finalized_observations["Dominance"] = (
        df_finalized_observations["Num Try Puts Attempted"]
        / df_finalized_observations["Net Flux Through Duct"]
    )

    df_finalized_observations[
        "Fraction Messages Dropped"
    ] = df_finalized_observations["Fraction Try Puts Dropped"]

    df_finalized_observations["Simstep Period Inlet (ns)"] = (
        df_finalized_observations["Simstep Period Inlet (s)"] * 1e9
    )

    df_finalized_observations["Num Messages Per Pull"] = (
        df_finalized_observations["Net Flux Through Duct"]
        / df_finalized_observations["Num Pulls Attempted"]
    )

    df_finalized_observations["Num Messages Per Laden Pull"] = (
        df_finalized_observations["Net Flux Through Duct"]
        / df_finalized_observations["Num Pulls That Were Laden Immediately"]
    )

    df_finalized_observations["Bunchiness"] = (
        df_finalized_observations["Num Messages Per Laden Pull"]
        / df_finalized_observations["Num Messages Per Pull"]
    ) - 1

    df_finalized_observations[
        "Delivery Clumpiness"
    ] = df_finalized_observations["Bunchiness"]
    df_finalized_observations["Intermittancy"] = df_finalized_observations[
        "Bunchiness"
    ]
    df_finalized_observations["Bunching"] = df_finalized_observations[
        "Bunchiness"
    ]

    df_finalized_observations["Latency, Log10 Updates"] = np.log10(
        df_finalized_observations["Latency Simsteps Inlet"]
    )

    df_finalized_observations["ms Per Update"] = (
        df_finalized_observations["Simstep Period Inlet (ns)"] / 10**6
    )

    df_finalized_observations["Delivery Failure, %"] = (
        df_finalized_observations["Fraction Messages Dropped"] * 100
    )
    df_finalized_observations["% Msgs Lost"] = df_finalized_observations[
        "Delivery Failure, %"
    ]

    df_finalized_observations[
        "Msgs Received Per Sent"
    ] = df_finalized_observations["Num Messages Per Pull"]

    df_finalized_observations[
        "Messages Received Per Second"
    ] = df_finalized_observations["Net Flux Through Duct"] / (
        df_finalized_observations["Outlet-Seconds Elapsed"]
    )
    df_finalized_observations[
        "Messages Sent Per Second"
    ] = df_finalized_observations["Num Try Puts Attempted"] / (
        df_finalized_observations["Inlet-Seconds Elapsed"]
    )

    df_finalized_observations[
        "Messages Received Per Second"
    ] = df_finalized_observations["Net Flux Through Duct"] / (
        df_finalized_observations["Outlet-Seconds Elapsed"]
    )
    df_finalized_observations[
        "Messages Sent Per Second"
    ] = df_finalized_observations["Num Try Puts Attempted"] / (
        df_finalized_observations["Inlet-Seconds Elapsed"]
    )

    return df_finalized_observations
