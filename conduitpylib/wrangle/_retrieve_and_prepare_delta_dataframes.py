import typing

from nbmetalog import nbmetalog as nbm
import pandas as pd

from ._find_treat_idx_mapped_col import find_treat_idx_mapped_col
from ._merge_inlet_outlet_data import merge_inlet_outlet_data
from ._wrangle_longitudinal_deltas import wrangle_longitudinal_deltas
from ._wrangle_snapshot_deltas import wrangle_snapshot_deltas


def retrieve_and_prepare_delta_dataframes(
    df_inlet_url: str = "https://osf.io/jgpnv/download",
    df_outlet_url: str = "https://osf.io/ncdfq/download",
    treatment_column: typing.Optional[str] = None,
) -> pd.DataFrame:
    df_inlet = pd.read_csv(
        df_inlet_url,
        compression="gzip",
    )
    nbm.print_dataframe_summary(*eval(nbm.nvp_expr("df_inlet")))

    df_outlet = pd.read_csv(
        df_outlet_url,
        compression="gzip",
    )
    nbm.print_dataframe_summary(*eval(nbm.nvp_expr("df_outlet")))

    df = merge_inlet_outlet_data(df_inlet, df_outlet)

    if treatment_column is not None:
        treat_idx_mapper = {
            val: idx for idx, val in enumerate(df[treatment_column].unique())
        }
        treat_idx_mapped_title = " | ".join(
            f"{idx} = {val}" for val, idx in treat_idx_mapper.items()
        )
        df[treat_idx_mapped_title] = df.apply(
            lambda row: treat_idx_mapper[row[treatment_column]],
            axis=1,
        )

        assert find_treat_idx_mapped_col(df) == treat_idx_mapped_title

    return (
        wrangle_longitudinal_deltas(df),
        wrangle_snapshot_deltas(df),
    )
