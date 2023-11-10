import re

import pandas as pd


def find_treat_idx_mapped_col(df: pd.DataFrame) -> str:
    # thanks, chatgpt
    regex_pattern = r"(?:\b\d+\s*=\s*.+?\b)(?:\s*\|\s*(?:\b\d+\s*=\s*.+?\b))*"

    # Use a list comprehension to find matching columns
    matching_columns = df.columns[df.columns.str.match(regex_pattern)]

    if len(matching_columns) == 1:
        return matching_columns[0]
    else:
        raise ValueError(
            f"Too many or too few matching columns: {matching_columns}"
        )
