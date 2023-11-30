import typing


class UnequalSentinel:
    """Sentinel value that is unequal to everything, including itself."""

    def __eq__(self: "UnequalSentinel", other: typing.Any) -> bool:
        return False

    def __ne__(self: "UnequalSentinel", other: typing.Any) -> bool:
        return True
