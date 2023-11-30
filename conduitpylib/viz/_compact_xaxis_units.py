import re

from bidict import frozenbidict
from matplotlib import axis
import numpy as np

from ..utils import round_to_multiple, splice

_si_prefixes = frozenbidict(
    {
        "Y": 24,  # yotta
        "Z": 21,  # zetta
        "E": 18,  # exa
        "P": 15,  # peta
        "T": 12,  # tera
        "G": 9,  # giga
        "M": 6,  # mega
        "k": 3,  # kilo
        "": 0,  # base
        "m": -3,  # milli
        "u": -6,  # micro
        "n": -9,  # nano
        "p": -12,  # pico
        "f": -15,  # femto
        "a": -18,  # atto
        "z": -21,  # zepto
        "y": -24,  # yocto
    },
)


def compact_xaxis_units(
    ax: axis.Axis,
    base_unit: str = "s",
    regex_pattern_template: str = r"\((.*?){base_unit}\)",
) -> None:
    r"""Adjusts the units on the x-axis of a matplotlib axis object to a more
    compact form.

    This function finds the multiplier (such as 10^3, 10^6, etc.) used by
    matplotlib for the axis, identifies the current unit prefix (like k for
    kilo, M for mega), and then consolidates these into a new, more compact unit
    prefix. This is particularly useful for graphs where the axis values are
    very large or very small, and a more readable unit is desired.

    Parameters
    ----------
    ax : axis.Axis
       Axis object whose x-axis units are to be compacted inplace.
    base_unit : str, default "s"
        The base unit (without prefix) for the axis.
    regex_pattern_template : str, default r"\((.*?){base_unit}\)"
        A regular expression template used to identify and extract the unit
        prefix from the axis label.

        The template must contain `{base_unit}` as a
        placeholder for the actual base unit. Default matches parenthesized expressions like "(ks)" for kiloseconds.
    """
    regex_pattern = regex_pattern_template.format(base_unit=base_unit)
    regex = re.compile(regex_pattern)

    # force population of
    fig = ax.get_figure()
    fig.canvas.draw()

    # get unit multiplier chosen by matplotlib
    offset_text = ax.xaxis.get_offset_text()
    if offset_text is None:  # handle unit (i.e., non) multiplier
        return
    offset_string = offset_text.get_text()
    if offset_string == "":  # handle unit (i.e., non) multiplier
        return
    offset_amount = float(offset_string)

    assert str(offset_amount).count("1") == 1  # power of 10

    # get unit prefix and multiplier in axis label
    old_label_string = ax.get_xlabel()
    (old_prefix_match,) = regex.finditer(old_label_string)
    old_prefix = old_prefix_match.group(1)
    old_multiplier_pow = _si_prefixes[old_prefix]
    old_multiplier = 10.0**old_multiplier_pow

    # calculate new prefix from net multiplier
    new_multiplier = offset_amount * old_multiplier
    approx_pow = round_to_multiple(np.log10(new_multiplier), multiple=1)
    new_multiplier_pow = round_to_multiple(approx_pow, multiple=3)
    leftover_pow = approx_pow - new_multiplier_pow

    new_prefix = _si_prefixes.inv[new_multiplier_pow]
    if leftover_pow:
        new_prefix = f"10^{leftover_pow}" + new_prefix

    ax.xaxis.get_offset_text().set(visible=False)  # remove offset axis scale
    old_prefix_span = old_prefix_match.span(1)
    new_label_string = splice(
        old_label_string,
        old_prefix_span,
        new_prefix,
    )
    ax.set_xlabel(new_label_string)
