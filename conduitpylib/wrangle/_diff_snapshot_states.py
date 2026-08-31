import typing

import numpy as np
import pandas as pd

from ..utils import pickle_cache


# this operation is slow, so put it on its own to minimize cache disruption
@pickle_cache()
def diff_snapshot_states(
    merged_df: pd.DataFrame,
    max_snapshot: int,
    treatment_column: typing.Optional[str] = None,
) -> pd.DataFrame:
    df_blurry_snapshots = merged_df[
            merged_df["Has Execution Blur"].astype(bool)
            # exclude excess, unintended snapshots from runs that took a while to shut down
            # (i.e., from at the 6 minute mark and beyond)
            & (merged_df["Snapshot"] <= max_snapshot)
        ]

    df_snapshot_diffs = df_blurry_snapshots.groupby(
        [
            "Process Instance UUID",
            "Snapshot",
            # subsequent items aren't meaningful to groupby
            # but are just included so they pass through untouched
            "Async Mode",
            "Num Nodes",
            "Allocated Tasks Per Node",
            "Cpus Per Node",
            "Num Processes",
            "Log Num Processes",
            "Num Simels Per Cpu",
            "Replicate",
            "proc",
            "Hostname",
            "Num Inlets",
            "Num Outlets",
            "Execution Instance UUID",
            "Num Threads",
            *(
                [treatment_column] if treatment_column is not None else []
            ),
        ],
        as_index=False,
    ).aggregate(
        {
            "Has Execution Blur": "size",  # dummy column for assert below
            "Num Puts Attempted": np.ptp,
            "Num Try Puts Attempted": np.ptp,
            "Num Blocking Puts": np.ptp,
            "Num Try Puts That Succeeded": np.ptp,
            "Num Puts That Succeeded Eventually": np.ptp,
            "Num Blocking Puts That Succeeded Immediately": np.ptp,
            "Num Puts That Succeeded Immediately": np.ptp,
            "Num Puts That Blocked": np.ptp,
            "Num Dropped Puts": np.ptp,
            "Num Reads Performed": np.ptp,
            "Num Reads That Were Fresh": np.ptp,
            "Num Reads That Were Stale": np.ptp,
            "Num Revisions Pulled": np.ptp,
            "Num Try Pulls Attempted": np.ptp,
            "Num Blocking Pulls": np.ptp,
            "Num Blocking Pulls That Blocked": np.ptp,
            "Num Revisions From Try Pulls": np.ptp,
            "Num Revisions From Blocking Pulls": np.ptp,
            "Num Pulls Attempted": np.ptp,
            "Num Pulls That Were Laden Eventually": np.ptp,
            "Num Blocking Pulls That Were Laden Immediately": np.ptp,
            "Num Blocking Pulls That Were Laden Eventually": np.ptp,
            "Num Pulls That Were Laden Immediately": np.ptp,
            "Num Try Pulls That Were Laden": np.ptp,
            "Num Try Pulls That Were Unladen": np.ptp,
            "Net Flux Through Duct": np.ptp,
            "Num Round Trip Touches Inlet": np.ptp,
            "Num Round Trip Touches Outlet": np.ptp,
            # why are these missing?
            #     'Row Initial Timepoint (ns) Inlet' : np.ptp,
            #     'Row Initial Timepoint (ns) Outlet' : np.ptp,
            "Row Final Timepoint (ns) Inlet": np.ptp,
            "Row Final Timepoint (ns) Outlet": np.ptp,
            "Runtime Seconds Elapsed Inlet": np.mean,
            "Runtime Seconds Elapsed Outlet": np.mean,
        }
    )
    assert {2} == set(df_snapshot_diffs["Has Execution Blur"])

    return df_snapshot_diffs
