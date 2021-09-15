# adapted from https://stackoverflow.com/a/1038999
def strip_end(text, suffix):
    """Strips suffix from string.

    Essentially, polyfill for removesuffix in Python 3.9."""
    if suffix and text.endswith(suffix):
        return text[:-len(suffix)]
    return text
