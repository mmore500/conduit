import pathlib

from iterdub import iterdub as ib
from iterpop import iterpop as ip
from keyname import keyname as kn
import numpy as np
from slugify import slugify
import statsmodels.formula.api as smf


def make_regression_row(
    *, data, independent_variable, dependent_variable, regression, row_filter
):
    filtered_data = data[data.apply(eval(row_filter), axis=1)]

    regfun = {
        "Quantile Regression over Means": smf.quantreg,
        "Quantile Regression over Medians": smf.quantreg,
    }[regression]
    model = regfun(
        f"Q('{dependent_variable}') ~ Q('{independent_variable}')",
        filtered_data,
    )
    fit_model = model.fit()

    slope = fit_model.params[f"Q('{independent_variable}')"]
    intercept = fit_model.params["Intercept"]

    slope_ci_lb, slope_ci_ub = (
        fit_model.conf_int().loc[f"Q('{independent_variable}')"].tolist()
    )
    intercept_ci_lb, intercept_ci_ub = (
        fit_model.conf_int().loc["Intercept"].tolist()
    )

    p = fit_model.pvalues.loc[f"Q('{independent_variable}')"]

    # normalize to "control", i.e., lowest num processes observed
    effect_size_normalization_data = data[
        data[independent_variable] == data[independent_variable].min()
    ][dependent_variable]
    effect_size_normalization_factor = {
        "Quantile Regression over Means": lambda x: x.mean(),
        "Quantile Regression over Medians": lambda x: x.median(),
    }[regression](effect_size_normalization_data)
    relative_effect_size = slope / effect_size_normalization_factor
    relative_effect_size_ci_lb = slope_ci_lb / effect_size_normalization_factor
    relative_effect_size_ci_ub = slope_ci_ub / effect_size_normalization_factor
    relative_effect_size_ci_width = (
        relative_effect_size_ci_ub - relative_effect_size_ci_lb
    )

    is_significant = p < 0.05 if np.isfinite(p) else None

    res = {
        "Independent Variable": independent_variable,
        "Dependent Variable": dependent_variable,
        "Dependent Variable Slug": slugify(dependent_variable),
        "Cpus Per Node": ib.dub(data["Cpus Per Node"]),
        "Num Simels Per Cpu": ip.pophomogeneous(data["Num Simels Per Cpu"]),
        "Slope Estimate": slope,
        "Slope Estimate 95% CI Lower Bound": slope_ci_lb,
        "Slope Estimate 95% CI Upper Bound": slope_ci_ub,
        "Absolute Effect Size": slope,
        "Absolute Effect Size 95% CI Lower Bound": slope_ci_lb,
        "Absolute Effect Size 95% CI Upper Bound": slope_ci_ub,
        "Absolute Effect Size 95% CI Width": slope_ci_ub - slope_ci_lb,
        "Relative Effect Size": relative_effect_size,
        "Relative Effect Size 95% CI Lower Bound": relative_effect_size_ci_lb,
        "Relative Effect Size 95% CI Upper Bound": relative_effect_size_ci_ub,
        "Relative Effect Size 95% CI Width": relative_effect_size_ci_width,
        "Intercept Estimate": intercept,
        "Intercept Estimate 95% CI Lower Bound": intercept_ci_lb,
        "Intercept Estimate 95% CI Upper Bound": intercept_ci_ub,
        "R^2": fit_model.rsquared,
        "p": fit_model.pvalues.loc[f"Q('{independent_variable}')"],
        "Significant?": is_significant,
        "Significant Effect Sign": (
            "-"
            if is_significant and slope < 0
            else "+"
            if is_significant and slope > 0
            else "0"
            if is_significant is not None
            else None
        ),
        "n": len(filtered_data),
        "Filter": row_filter,
        "Num Processes": ib.dub(filtered_data["Num Processes"]),
        "Num Processes Prettyprint": (
            "/".join(
                filtered_data["Num Processes"]
                .sort_values()
                .astype(str)
                .unique()
            )
        ),
        "Regression Model": regression,
        "Regression Model Slug": slugify(regression),
        "Statistic": {
            "Quantile Regression over Means": "mean",
            "Quantile Regression over Medians": "median",
        }[regression],
    }

    # dump regression summary to file
    summary_filename = kn.pack(
        {
            **{
                "a": "regression_summary",
                "ext": ".txt",
            },
            **{
                slugify(k): slugify(str(v))
                for k, v in res.items()
                if k
                in [
                    "Independent Variable",
                    "Dependent Variable",
                    "Cpus Per Node",
                    "Num Simels Per Cpu",
                    "Regression Model",
                ]
            },
        }
    )

    pathlib.Path("outplots").mkdir(parents=True, exist_ok=True)
    with open(f"outplots/{summary_filename}", "w") as file:
        print(fit_model.summary(), file=file)

    return res
