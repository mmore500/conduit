from conduitpylib.utils import round_to_multiple


def test_round_to_multiple():
    # Normal cases
    assert round_to_multiple(10.5, 2) == 10
    assert round_to_multiple(15.3, 5) == 15
    assert round_to_multiple(2.6, 1) == 3

    # Edge cases
    assert round_to_multiple(0, 1) == 0
    assert round_to_multiple(-5.3, 2) == -6
    assert round_to_multiple(1000000.7, 1000) == 1000000
