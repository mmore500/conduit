import pandas as pd


def wrangle_instrumentation_longform(
    longitudinal_df: pd.DataFrame,
    snapshot_df: pd.DataFrame,
    column_name: str = "Instrumentation",
    snapshot_moniker: str = "Snapshot",
    longitudinal_moniker: str = "Longitudinal",
) -> pd.DataFrame:
    snapshot = snapshot_df.copy().groupby("Process Instance UUID").sample(n=1)
    snapshot[column_name] = snapshot_moniker

    longitudinal = (
        longitudinal_df.copy().groupby("Process Instance UUID").sample(n=1)
    )
    longitudinal[column_name] = longitudinal_moniker

    return pd.concat([snapshot, longitudinal])
