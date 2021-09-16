import pandas as pd

from .strip_end import strip_end

def consolidate_merge(*args, **kwargs):
    """Drop-in wrapper around pandas.merge that performs an outer merge,
    then consolidates identical columns from the left and right dataframes."""
    assert 'how' in kwargs and kwargs['how'] == 'outer'

    l_suffix = '__TRYCONSOLIDATEME_LEFT__'
    r_suffix = '__TRYCONSOLIDATEME_RIGHT__'

    df = pd.merge(
        *args,
        **{
            **kwargs,
            **{
                'suffixes' : (l_suffix, r_suffix)
            }
        }
    )

    l_suffix_columns = (
        col
        for col in df.columns
        if col.endswith(l_suffix)
    )

    for l_suffix_col in l_suffix_columns:
        base_column_name = strip_end(l_suffix_col, l_suffix)
        r_suffix_col = base_column_name + r_suffix
        assert r_suffix_col in df.columns

        if df[l_suffix_col].equals(df[r_suffix_col]):
            df[base_column_name] = df[l_suffix_col]
            df.drop(
                columns=[l_suffix_col, r_suffix_col],
                inplace=True,
            )
        else:
            requested_l_suffix, requested_r_suffix = kwargs['suffixes']
            df.rename(
                columns={
                    l_suffix_col : base_column_name + requested_l_suffix,
                    r_suffix_col : base_column_name + requested_r_suffix,
                },
                inplace=True,
            )

    return df
