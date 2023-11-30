from contextlib import contextmanager

from matplotlib import pyplot as plt


@contextmanager
def open_dummy_axis():
    # Save current figure and axis
    original_fig = plt.gcf()
    original_ax = plt.gca()

    dummy_fig = None
    try:
        # Create a new figure for the dummy axis
        dummy_fig = plt.figure()
        dummy_ax = dummy_fig.add_subplot(111)

        # Yield control back to the block of code using the context manager
        yield dummy_ax
    finally:
        # Close the dummy figure and restore the original figure and axis
        if dummy_fig is not None:
            plt.close(dummy_fig)
        plt.figure(original_fig.number)
        plt.sca(original_ax)
