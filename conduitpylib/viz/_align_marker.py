import typing

from matplotlib import markers as mpl_markers
from matplotlib.path import Path as mpl_Path


# adapted from https://stackoverflow.com/a/26726237
def align_marker(
    marker: str,
    halign: typing.Literal["left", "center", "right"] = "center",
    valign: typing.Literal["top", "middle", "bottom"] = "middle",
    pad: float = 1.0,
) -> mpl_Path:
    """Create markers with specified alignment.

    Parameters
    ----------
    marker : a valid marker specification.
      See mpl.markers
    halign : string, float {'left', 'center', 'right'}
      Specifies the horizontal alignment of the marker. *float* values
      specify the alignment in units of the markersize/2 (0 is 'center',
      -1 is 'right', 1 is 'left').
    valign : string, float {'top', 'middle', 'bottom'}
      Specifies the vertical alignment of the marker. *float* values
      specify the alignment in units of the markersize/2 (0 is 'middle',
      -1 is 'top', 1 is 'bottom').

    Returns
    -------
    marker_array : numpy.ndarray
      A Nx2 array that specifies the marker path relative to the
      plot target point at (0, 0).

    Notes
    -----
    The mark_array can be passed directly to ax.plot and ax.scatter, e.g.::

        ax.plot(1, 1, marker=align_marker('>', 'left'))
    """

    if isinstance(halign, str):
        halign = {
            "right": -pad,
            "middle": 0.0,
            "center": 0.0,
            "left": pad,
        }[halign]

    if isinstance(valign, str):
        valign = {
            "top": -pad,
            "middle": 0.0,
            "center": 0.0,
            "bottom": pad,
        }[valign]

    # Define the base marker
    bm = mpl_markers.MarkerStyle(marker)

    # Get the marker path and apply the marker transform to get the
    # actual marker vertices (they should all be in a unit-square
    # centered at (0, 0))
    m_arr = bm.get_path().transformed(bm.get_transform()).vertices

    # Shift the marker vertices for the specified alignment.
    m_arr[:, 0] += halign / 2
    m_arr[:, 1] += valign / 2

    return mpl_Path(m_arr, bm.get_path().codes)
