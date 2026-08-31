import pandas as pd


def rename_allocation(df: pd.DataFrame) -> pd.DataFrame:
    df = df.copy()
    df["Allocation"] = df["Allocation"].map(
        {
            "With lac-417": "Incl. lac-417",
            "Sans lac-417": "Excl. lac-417",
        }
    )
    return df
