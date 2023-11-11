from matplotlib import pyplot as plt
from scipy import stats

from ..utils import get_signif_asterisks


def annotate_spearman(x: str, y: str, **kwargs) -> None:
    data = kwargs.pop("data")
    ax = kwargs.pop("ax", plt.gca())

    # Calculate Spearman correlation and significance
    corr, p_value = stats.spearmanr(data[x], data[y])
    stars = get_signif_asterisks(p_value)

    # Linear regression
    slope, _intercept, _r_value, _p_value, _std_err = stats.linregress(
        data[x], data[y]
    )

    # Annotate the plot
    ax.annotate(
        f"Spear Rho:\n {corr:.2f} {stars}\nn:{len(data)}\nLinReg Slope:\n {slope:.2f}",
        xy=(0.05, 0.95),
        xycoords="axes fraction",
        ha="left",
        va="top",
        bbox=dict(
            alpha=1.0, boxstyle="round,pad=0.5", facecolor="white", zorder=900
        ),
        zorder=1000,
    )
