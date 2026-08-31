import pandas as pd


def count_outliers(data):
    """Calculate the number of outliers in a pandas column using the IQR
    method."""

    # Calculate Q1 and Q3
    Q1 = data.quantile(0.25)
    Q3 = data.quantile(0.75)

    # Calculate IQR
    IQR = Q3 - Q1

    # Define bounds for outliers
    lower_bound = Q1 - 1.5 * IQR
    upper_bound = Q3 + 1.5 * IQR

    # Count outliers
    return (
        (data < lower_bound)
        | (data > upper_bound)
    ).sum()


def count_proportion_outliers(data):
    return count_outliers(data) / len(data)


def count_nonoutliers(data):
    return len(data) - count_outliers(data)
