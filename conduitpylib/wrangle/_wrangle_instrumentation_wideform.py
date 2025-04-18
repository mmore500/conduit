import pandas as pd

from ..viz._get_defaults import get_default_runtime_indicators


def wrangle_instrumentation_wideform(
    longitudinal_df: pd.DataFrame,
    snapshot_df: pd.DataFrame,
    column_name: str = "Instrumentation",
    snapshot_moniker: str = "Snapshot",
    longitudinal_moniker: str = "Longitudinal",
) -> pd.DataFrame:
    assert not snapshot_df.columns.duplicated().any()

    roll = snapshot_df.copy().groupby("Process Instance UUID").sample(n=1)
    roll[column_name] = snapshot_moniker
    assert not roll.columns.duplicated().any()

    assert not longitudinal_df.columns.duplicated().any()
    fin = longitudinal_df.copy().groupby("Process Instance UUID").sample(n=1)
    fin[column_name] = longitudinal_moniker
    assert not fin.columns.duplicated().any()

    suffixes = (f" {snapshot_moniker}", f" {longitudinal_moniker}")
    cat = pd.merge(
        roll,
        fin,
        how="right",
        on=["Process Instance UUID"],
        suffixes=suffixes,
    ).reset_index(drop=True)
    assert not cat.columns.duplicated().any()

    keep_columns = []
    rename_columns = {}

    col_pairs = [
        (c, c.replace(*suffixes))
        for c in cat.columns
        if suffixes[0] in c and c.replace(*suffixes) in cat.columns
    ]

    for c1, c2 in col_pairs:
        assert c1 != c2
        keep_columns.append(c1)
        assert len(cat[c1]) == len(cat[c2])
        root = c1.replace(suffixes[0], "")
        if (
            cat[c1] != cat[c2]
        ).any() or root in get_default_runtime_indicators():
            keep_columns.append(c2)
        else:
            rename_columns[c1] = root

    return cat[keep_columns].rename(columns=rename_columns).copy()
