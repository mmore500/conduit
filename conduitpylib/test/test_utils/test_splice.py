from conduitpylib.utils import splice


def test_splice_removal_only():
    assert splice("Hello world", (5, 11)) == "Hello"


def test_splice_insertion_only():
    assert splice("123456789", (3, 3), "ABC") == "123ABC456789"


def test_splice_removal_and_insertion():
    assert splice("Hello world", (0, 5), "Goodbye") == "Goodbye world"


def test_splice_empty_string():
    assert splice("", (0, 0), "Hello") == "Hello"


def test_splice_full_replacement():
    assert splice("Hi", (0, 2), "Hello") == "Hello"


def test_splice_out_of_bounds():
    assert splice("Hello", (0, 15)) == ""


def test_splice_negative_indices():
    splice("Hello", (-3, -1))
