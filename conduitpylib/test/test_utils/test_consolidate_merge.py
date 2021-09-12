#!/bin/python3

import pandas as pd
import pandas.testing as pd_testing
import unittest

from conduitpylib.utils import consolidate_merge

class TestCountHandsWithKOrMoreSets(unittest.TestCase):

    # adapted from https://stackoverflow.com/a/54344148
    def assertDataframeEqual(self, a, b, msg):
        try:
            pd_testing.assert_frame_equal(a, b)
        except AssertionError as e:
            raise self.failureException(msg) from e

    def setUp(self):
        self.addTypeEqualityFunc(pd.DataFrame, self.assertDataframeEqual)

    # adapted from https://pandas.pydata.org/pandas-docs/stable/user_guide/merging.html#brief-primer-on-merge-methods-relational-algebra
    def test_no_identical_column(self):

        left = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['A0', 'A1', 'A2', 'A3'],
            'B': ['B0', 'B1', 'B2', 'B3'],
        })


        right = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
        })

        result = consolidate_merge(left, right, on='key', how='outer')
        expected_result = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['A0', 'A1', 'A2', 'A3'],
            'B': ['B0', 'B1', 'B2', 'B3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
        })

        self.assertEqual(
            result.sort_index(axis=1),
            expected_result.sort_index(axis=1)
        )

    # adapted from https://pandas.pydata.org/pandas-docs/stable/user_guide/merging.html#brief-primer-on-merge-methods-relational-algebra
    def test_one_identical_varied_column(self):

        left = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['A0', 'A1', 'A2', 'A3'],
            'B': ['B0', 'B1', 'B2', 'B3'],
        })


        right = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
            'A': ['A0', 'A1', 'A2', 'A3'],
        })

        result = consolidate_merge(left, right, on='key', how='outer')
        expected_result = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['A0', 'A1', 'A2', 'A3'],
            'B': ['B0', 'B1', 'B2', 'B3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
        })

        self.assertEqual(
            result.sort_index(axis=1),
            expected_result.sort_index(axis=1)
        )

    # adapted from https://pandas.pydata.org/pandas-docs/stable/user_guide/merging.html#brief-primer-on-merge-methods-relational-algebra
    def test_one_identical_uniform_column(self):

        left = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['A', 'A', 'A', 'A'],
            'B': ['B0', 'B1', 'B2', 'B3'],
        })


        right = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
            'A': ['A', 'A', 'A', 'A'],
        })

        result = consolidate_merge(left, right, on='key', how='outer')
        expected_result = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['A', 'A', 'A', 'A'],
            'B': ['B0', 'B1', 'B2', 'B3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
        })

        self.assertEqual(
            result.sort_index(axis=1),
            expected_result.sort_index(axis=1)
        )

    # adapted from https://pandas.pydata.org/pandas-docs/stable/user_guide/merging.html#brief-primer-on-merge-methods-relational-algebra
    def test_custom_labels(self):

        left = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A': ['X', 'X', 'X', 'X'],
            'B': ['B0', 'B1', 'B2', 'B3'],
        })


        right = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'B': ['B0', 'B1', 'B2', 'B3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
            'A': ['Y', 'Y', 'Y', 'Y'],
        })

        result = consolidate_merge(
            left,
            right,
            on='key',
            how='outer',
            suffixes=(" left", " right"),

        )
        expected_result = pd.DataFrame({
            'key': ['K0', 'K1', 'K2', 'K3'],
            'A left': ['X', 'X', 'X', 'X'],
            'A right': ['Y', 'Y', 'Y', 'Y'],
            'B': ['B0', 'B1', 'B2', 'B3'],
            'C': ['C0', 'C1', 'C2', 'C3'],
            'D': ['D0', 'D1', 'D2', 'D3'],
        })

        self.assertEqual(
            result.sort_index(axis=1),
            expected_result.sort_index(axis=1)
        )


if __name__ == '__main__':
    unittest.main()
