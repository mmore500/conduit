import seaborn as sns

from conduitpylib.utils import seaborn_monkeypatched_kdecache


def test_seaborn_monkeypatch_kdecache() -> None:
    geyser = sns.load_dataset("geyser")

    with seaborn_monkeypatched_kdecache():
        sns.kdeplot(data=geyser, x="waiting", y="duration", hue="kind")
        sns.kdeplot(data=geyser, x="waiting", y="duration", hue="kind")

        cache_info = sns._statistics.KDE.__call__.cache_info
        assert cache_info["hits"] > 0 and cache_info["misses"] > 0

        prehits = cache_info["hits"]
        sns.kdeplot(data=geyser, x="waiting", y="duration")
        assert cache_info["hits"] == prehits

        sns.kdeplot(data=geyser, x="waiting", y="duration")
        assert cache_info["hits"] > prehits

    assert not hasattr(sns._statistics.KDE.__call__, "cache_info")
    sns.kdeplot(data=geyser, x="waiting", y="duration", hue="kind")
