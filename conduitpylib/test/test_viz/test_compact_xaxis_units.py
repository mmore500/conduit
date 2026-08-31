from matplotlib import pyplot as plt

from conduitpylib.viz import compact_xaxis_units


def test_prefix_removal():
    # setup
    ax = plt.gca()
    ax.ticklabel_format(style="sci", scilimits=(-2, 2)),
    ax.plot([1000, 2000, 3000], [1, 2, 3])
    ax.set_xlabel("Time (ms)")

    # apply
    compact_xaxis_units(ax, base_unit="s")

    # verify
    expected_label = "Time (s)"
    assert ax.get_xlabel() == expected_label

    # clean up
    plt.clf()


def test_prefix_addition():
    # setup
    ax = plt.gca()
    ax.ticklabel_format(style="sci", scilimits=(-2, 2)),
    ax.plot([1000, 2000, 3000], [1, 2, 3])
    ax.set_xlabel("Weight (g)")
    # apply
    compact_xaxis_units(ax, base_unit="g")

    # verify
    expected_label = "Weight (kg)"
    assert ax.get_xlabel() == expected_label

    # clean up
    plt.clf()


def test_prefix_nop():
    # setup
    ax = plt.gca()
    ax.ticklabel_format(style="sci", scilimits=(-2, 2)),
    ax.plot([1, 2, 3], [1, 2, 3])
    ax.set_xlabel("Weight (g)")
    # apply
    compact_xaxis_units(ax, base_unit="g")

    # verify
    expected_label = "Weight (g)"
    assert ax.get_xlabel() == expected_label

    # clean up
    plt.clf()


def test_nanoseconds_to_milliseconds():
    # setup
    ax = plt.gca()
    ax.ticklabel_format(style="sci", scilimits=(-2, 2)),
    ax.plot([1e6, 2e6, 3e6], [1, 2, 3])
    ax.set_xlabel("Time (ns)")

    # apply
    compact_xaxis_units(ax, base_unit="s")

    # verify
    expected_label = "Time (ms)"
    assert ax.get_xlabel() == expected_label

    # clean up
    plt.clf()
