from matplotlib import pyplot as plt
import numpy as np
import seaborn as sns
import statsmodels.formula.api as smf


def quantile_regplot(fit_reg=True, color=None, *args, **kwargs):
    x, y, data = kwargs["x"], kwargs["y"], kwargs["data"]
    sns.regplot(
        *args,
        **kwargs,
        fit_reg=False,
        color=color,
    )

    if fit_reg:
        model = smf.quantreg(f"Q('{y}') ~ Q('{x}')", data)
        res = model.fit(q=0.5)
        m = res.params[f"Q('{x}')"]
        b = res.params["Intercept"]

        m_ci = res.conf_int().loc[f"Q('{x}')"].tolist()
        b_ci = res.conf_int().loc["Intercept"].tolist()

        center_x = np.mean([data[x].min(), data[x].max()])
        center_y = m * center_x + b

        xs = sorted(set(data[x]) | {center_x})
        ys = [m * x_ + b for x_ in xs]
        y1 = [min(m_ * (x_ - center_x) + center_y for m_ in m_ci) for x_ in xs]
        y2 = [max(m_ * (x_ - center_x) + center_y for m_ in m_ci) for x_ in xs]

        plt.gca().plot(
            xs,
            ys,
            color=color,
        )
        plt.gca().fill_between(
            xs,
            y1,
            y2,
            alpha=0.2,
            color=color,
        )
