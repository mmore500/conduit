import pytest
import pandas as pd

from conduitpylib.utils import pickle_cache


@pickle_cache()
def sample_function(df):
    # A simple function that might perform some operations on a DataFrame
    return df.describe()


@pickle_cache()
def another_function(df):
    # A simple function that might perform some operations on a DataFrame
    return f"{df.describe()}42"


@pytest.fixture
def sample_dataframe():
    # Creating a sample DataFrame to be used in tests
    return pd.DataFrame({"A": [1, 2, 3], "B": [4, 5, 6]})


@pytest.fixture
def another_dataframe():
    # Creating a sample DataFrame to be used in tests
    return pd.DataFrame({"A": [1, 2, 3], "B": [1, 5, 6]})


def test_cache_hit(sample_dataframe: pd.DataFrame):
    sample_function(sample_dataframe)
    sample_function.info["hits"] = 0
    sample_function(sample_dataframe)
    assert sample_function.info["hits"] == 1


def test_cache_integrity(
    sample_dataframe: pd.DataFrame, another_dataframe: pd.DataFrame
):
    cached_result = sample_function(sample_dataframe)

    new_result = sample_function(sample_dataframe)
    assert cached_result.equals(new_result)

    another_result = another_function(another_dataframe)
    assert not new_result.equals(another_result)

    yet_another_result = another_function(sample_dataframe)
    assert not new_result.equals(yet_another_result)


# Additional tests can be added for different scenarios and edge cases
